#define _POSIX_C_SOURCE 200809L
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "strutil.h"
#include "calc_helper.h"

calc_num suma(calc_num num1, calc_num num2){
    return num1 + num2;
}

calc_num resta(calc_num num1, calc_num num2){
    return num2 - num1;
}

calc_num multi(calc_num num1, calc_num num2){
    return num1 * num2;
}

calc_num divi(calc_num num1, calc_num num2){
    return num2 / num1;
}

calc_num pot(double num1, double num2){
    double resultado = pow(num2, num1);
    return (calc_num)resultado;
}

calc_num logarit(calc_num num1, calc_num num2){
    double resultado = (log((double)num2) / log((double)num1));
    return (calc_num)resultado;
}

calc_num raiz(calc_num num1){
    double resultado = sqrt((double)num1);
    return (calc_num)resultado;
}

calc_num tern(calc_num num1, calc_num num2, calc_num num3){
    return num3 ? num2 : num1;
}

int main(){
    char* linea = NULL;
    
    size_t tam = 0;
    char** strv;
    struct calc_token elemento;
    pilanum_t *pila;
    calc_num salida = 0;
    calc_num resultado = 0;
    calc_num num1 = 0;
    calc_num num2 = 0;
    calc_num num3 = 0;
    bool error = false;
    calc_num num_aux;
    while (getline(&linea, &tam, stdin) != EOF){
        error = false;
        pila = pilanum_crear();
        strv = dc_split(linea);

        int i = 0;
        while (strv[i] && !error){
            if (!calc_parse(strv[i], &elemento)){
                error = true;
                break;
            }
            if (elemento.type == TOK_NUM){ //si es numero se apila
                apilar_num(pila, elemento.value);
            }
            else{ 
                if(!desapilar_num(pila, &num1)){
                             error = true;
                             break;
                        }
                switch(elemento.oper.op){  //dudo que esta sea la mejor forma de hacerlo, pero se me hizo bonito
                    case OP_ADD:
                        if(!desapilar_num(pila, &num2)){
                             error = true;
                             break;
                        }
                        resultado = suma(num1, num2);
                        break;
                    case OP_SUB:
                        if(!desapilar_num(pila, &num2)){
                             error = true;
                             break;
                        }
                        resultado = resta(num1, num2);
                        break;
                    case OP_MUL:
                        if(!desapilar_num(pila, &num2)){
                             error = true;
                             break;
                        }
                        resultado = multi(num1, num2);
                        break;
                    case OP_DIV:
                        if(!desapilar_num(pila, &num2) || num1 == 0){
                            error = true;
                            break;
                        }
                        resultado = divi(num1, num2);
                        break;
                    case OP_POW:
                        if(!desapilar_num(pila, &num2) || num1 < 0){
                            error = true;
                            break;
                        }
                        
                        resultado = pot((double)num1, (double)num2);
                        break;
                    case OP_LOG:
                        if(!desapilar_num(pila, &num2) || num1 < 2){
                             error = true;
                             break;
                        }
                        resultado = logarit(num1, num2);
                        break;
                    case OP_RAIZ:
                        if (num1 < 0){
                            error = true;
                            break;
                        }
                        resultado = raiz(num1);
                        break;
                    case OP_TERN:
                        if(!desapilar_num(pila, &num2) || !desapilar_num(pila, &num3)){
                             error = true;
                             break;
                        }
                        resultado = num3 ? num2 : num1;
                        break;
                }
                if (error == true){
                    break;
                }
                apilar_num(pila, resultado);
            }
            i++;
        }
        if (!desapilar_num(pila, &salida)){
            error = true;
        }
        if (error || desapilar_num(pila, &num_aux) ){ //si hay un error o quedan mas de un numero en la pila
            printf("ERROR\n");
        }
        else{
            printf("%ld\n", salida);
        }
        pilanum_destruir(pila);
        free_strv(strv);
    }
    free(linea);
    return 0;
}