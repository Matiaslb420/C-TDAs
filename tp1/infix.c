#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include "strutil.h"
#include "calc_helper.h"
#include "pila.h"

enum precedencia { //para manejar las prioridades
    PARENTESIS = 0,
    ADD_P = 1,
    SUB_P = 1,
    DIV_P = 2,
    MUL_P = 2,
    POT_P = 3,
};

enum precedencia asignar_precedencia(char* op){
        switch (op[0]){
        case '+':
        case '-':
            return  SUB_P;
        case '*':
        case '/':
            return  DIV_P;
        case '^':
            return POT_P;
        default:
            return PARENTESIS;
    }
    return 0;
}

void apilar_op(pila_t* pila, char* elemento){
    char* operador = strdup(elemento);
    pila_apilar(pila, operador);
}

void guardar_op(pila_t* pila, enum oper_type op){
    enum precedencia prioridad = 0;
    char* op_pila = "\0"; //lo que va en la pila se guarda de una como char*
    switch (op){
        case OP_ADD:
            op_pila = "+";
            prioridad = ADD_P;
            break;
        case OP_SUB:
            op_pila = "-";
            prioridad = SUB_P;
            break;
        case OP_MUL:
            op_pila = "*";
            prioridad = DIV_P;
            break;
        case OP_DIV:
            op_pila = "/";
            prioridad = MUL_P;
            break;
        case OP_POW:
            op_pila = "^";
            prioridad = POT_P;
            break;
        default:
            break;
    }
    void* tope = pila_ver_tope(pila);
    if (tope != NULL){
        char* desapilado;
        enum precedencia tope_p = asignar_precedencia((char*)tope);
        while (tope != NULL && tope_p >= prioridad){ 
            if (tope_p == prioridad && tope_p == POT_P){
                break;
            }
            desapilado = (char*)pila_desapilar(pila);                   
            fprintf(stdout,"%s ", desapilado);
            free(desapilado);
            tope = pila_ver_tope(pila);
            if(tope != NULL){
                tope_p = asignar_precedencia((char*)tope);
            }
        }
    }
    apilar_op(pila, op_pila);
}

void cerrar_parentesis(pila_t* pila){
    void* tope = pila_ver_tope(pila);
    if (tope != NULL){
        char* desapilado;
        while(((char*)tope)[0] != '('){
            desapilado = (char*)pila_desapilar(pila);
            fprintf(stdout, "%s ", desapilado);
            free(desapilado);
            tope = pila_ver_tope(pila);
            if (tope == NULL){
                break;
            }
        }
    }
    free(pila_desapilar(pila));
}

int main(){
    char* linea = NULL;
    size_t tam = 0;
    char** strv;
    struct calc_token elemento;
    pila_t* pila_op = NULL;
    while (getline(&linea, &tam, stdin) != EOF){
        pila_op = pila_crear();
        strv = infix_split(linea);
        int i = 0;
        while (strv[i]){
            if (!calc_parse(strv[i], &elemento)){
                break;
            }
            switch (elemento.type){
                case TOK_NUM:
                    fprintf(stdout, "%ld ", elemento.value);
                    break;
                case TOK_OPER:
                    guardar_op(pila_op, elemento.oper.op);
                    break;
                case TOK_LPAREN:
                    apilar_op(pila_op, "(");
                    break;
                case TOK_RPAREN:
                    cerrar_parentesis(pila_op);
                    break;
            }
            i++;
        }
        char* op;
        while(!pila_esta_vacia(pila_op)){
            op = (char*)pila_desapilar(pila_op);
            fprintf(stdout, "%s ", op);
            free(op);
        }
        char* endline = "\n";
        fprintf(stdout, "%s", endline);
        pila_destruir(pila_op);
        free_strv(strv);
    }
    free(linea);
    return 0;
}
