#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include "abb.h"
#include "pila.h"
enum direccion {
    IZQ,
    DER,
};

typedef struct nodo {
    char* clave;
    void* dato;
    struct nodo* hijo_izq;
    struct nodo* hijo_der;
} nodo_t;

struct abb {
    nodo_t* nodo_raiz;
    size_t cant_elem;
    abb_comparar_clave_t comparar;
    abb_destruir_dato_t destruir_dato;
};

struct abb_iter {
    pila_t* pila;
    const abb_t* abb;
};
nodo_t* nodo_crear(const char * clave, void* dato){
    nodo_t* nodo = malloc(sizeof(nodo_t));
    if(nodo == NULL){
        return NULL;
    }
    nodo->clave = strdup(clave);
    nodo->dato = dato;
    nodo->hijo_der = NULL;
    nodo->hijo_izq = NULL;
    return nodo;
}

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
    abb_t * abb = malloc(sizeof(abb_t));
    if (abb == NULL){
        return NULL;
    }
    abb->nodo_raiz = NULL;
    abb->cant_elem = 0;
    abb->comparar = cmp;
    abb->destruir_dato = destruir_dato;
    return abb;
}

nodo_t* abb_encontrar(const abb_t* arbol, const char* clave, nodo_t** padre, enum direccion* lado){
    if(arbol->cant_elem == 0){
        return NULL;
    }
    int cmp = 0;

    nodo_t* nodo_cmp = arbol->nodo_raiz;
    while (true){
        cmp = arbol->comparar(clave, nodo_cmp->clave);
        if (cmp == 0){
            return nodo_cmp;
        }
        *padre = nodo_cmp;
        if (cmp < 0){
            *lado = IZQ;
            if (nodo_cmp->hijo_izq == NULL){
                return NULL;
            }

            nodo_cmp = nodo_cmp->hijo_izq;
        }
        else{
            *lado = DER;
            if (nodo_cmp->hijo_der == NULL){
                return NULL;
            }
            nodo_cmp = nodo_cmp->hijo_der;
        }
    }
    return NULL;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
    nodo_t* padre = NULL;
    enum direccion lado = IZQ;
    nodo_t* nodo = abb_encontrar(arbol, clave, &padre, &lado);
    if (nodo != NULL){
        if (arbol->destruir_dato){
            arbol->destruir_dato(nodo->dato);
        }
        nodo->dato = dato;
        return true;
    }
    nodo_t* nodo_nuevo = nodo_crear(clave, dato);
    if (padre == NULL){ //no hay nodos
        arbol->nodo_raiz = nodo_nuevo;
    }
    else if(lado == IZQ){
        padre->hijo_izq = nodo_nuevo;
    }
    else{
        padre->hijo_der = nodo_nuevo;
    }
    arbol->cant_elem++;
    return true;
}


void borrar_nodo(nodo_t* nodo){
    free(nodo->clave);
    free(nodo);
}

void borrar_sin_hijos(nodo_t* nodo_ant, nodo_t* nodo_act, int hijo_dir){
    borrar_nodo(nodo_act);
    if (nodo_ant == NULL){
        return;
    }
    if (hijo_dir == IZQ){
        nodo_ant->hijo_izq = NULL;
    }
    else if (hijo_dir == DER){
        nodo_ant->hijo_der = NULL; 
    }
}

void borrar_un_hijo(abb_t *arbol, nodo_t* nodo_act, nodo_t* nodo_ant, int hijo_dir){
    if (nodo_ant == NULL){
        arbol->nodo_raiz = nodo_act->hijo_izq ? nodo_act->hijo_izq : nodo_act->hijo_der;
        borrar_nodo(nodo_act);
        return;
    }
    if (nodo_act->hijo_izq != NULL){
        if (hijo_dir == IZQ){
            nodo_ant->hijo_izq = nodo_act->hijo_izq;
        }
        else{
            nodo_ant->hijo_der = nodo_act->hijo_izq;
        }
    }
    else{
        if (hijo_dir  == IZQ){
            nodo_ant->hijo_izq = nodo_act->hijo_der;
        }
        else{
            nodo_ant->hijo_der = nodo_act->hijo_der;
        }
    }
    borrar_nodo(nodo_act);
}

void borrar_dos_hijos(abb_t* arbol, nodo_t* nodo_act){
    bool mayor_de_los_menores = false;
    nodo_t* nodo_reemplazo = nodo_act->hijo_izq;
    nodo_t* nodo_reemplazo_ant = nodo_act;
    while(nodo_reemplazo->hijo_der != NULL){
        mayor_de_los_menores = true;
        nodo_reemplazo_ant = nodo_reemplazo;
        nodo_reemplazo = nodo_reemplazo->hijo_der;
    }
    if (!mayor_de_los_menores){
        if (nodo_reemplazo->hijo_izq != NULL){
            nodo_reemplazo_ant->hijo_izq = nodo_reemplazo->hijo_izq;
        }
        else{
            nodo_reemplazo_ant->hijo_izq = NULL;
        }
    }
    else if (nodo_reemplazo->hijo_izq != NULL){
        nodo_reemplazo_ant->hijo_der = nodo_reemplazo->hijo_izq;
    }
    else{
        nodo_reemplazo_ant->hijo_der = NULL;
    }
    free(nodo_act->clave);
    nodo_act->clave = nodo_reemplazo->clave;
    nodo_act->dato = nodo_reemplazo->dato;
    free(nodo_reemplazo);
}

void *abb_borrar(abb_t *arbol, const char *clave){
    nodo_t* padre = NULL;
    enum direccion lado = IZQ;
    nodo_t* nodo_borrar = abb_encontrar(arbol, clave, &padre, &lado);
    if (nodo_borrar == NULL){
        return NULL;
    }
    void* dato = nodo_borrar->dato;
    if(nodo_borrar->hijo_der == NULL && nodo_borrar->hijo_izq == NULL){
        borrar_sin_hijos(padre, nodo_borrar, lado);
        if(arbol->cant_elem == 1){
            arbol->nodo_raiz = NULL;
        }
    }
    else if(nodo_borrar->hijo_izq != NULL && nodo_borrar->hijo_der != NULL){
        borrar_dos_hijos(arbol, nodo_borrar);
    }
    else{
        borrar_un_hijo(arbol, nodo_borrar, padre, lado);
    }
    arbol->cant_elem--;
    return dato;
}

void *abb_obtener(const abb_t *arbol, const char *clave){
    nodo_t* padre = NULL;
    enum direccion lado = IZQ;
    nodo_t* salida = abb_encontrar(arbol, clave, &padre, &lado);
    return salida ? salida->dato : salida;
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
    nodo_t* padre = NULL;
    enum direccion lado = IZQ;
    return (abb_encontrar(arbol, clave, &padre, &lado) != NULL);
    
}

size_t abb_cantidad(const abb_t *arbol){
    return arbol->cant_elem;
}

void iter_borrar_post(nodo_t* nodo, abb_destruir_dato_t destruir){
    if (!nodo) return;
    iter_borrar_post(nodo->hijo_izq, destruir);
    iter_borrar_post(nodo->hijo_der, destruir);
    if (destruir){
        destruir(nodo->dato);
    }
    borrar_nodo(nodo);
}

void abb_destruir(abb_t *arbol){
    iter_borrar_post(arbol->nodo_raiz, arbol->destruir_dato);
    free(arbol);
}
//******************************************************************************
//                                    ITERADOR INTERNO 
bool iter_in_order(nodo_t* nodo, bool visitar(const char *, void *, void*), void* extra){
    if (!nodo) return true;
    if (!iter_in_order(nodo->hijo_izq, visitar, extra)) return false;
    if (!visitar(nodo->clave, nodo->dato, extra)) return false;
    if (!iter_in_order(nodo->hijo_der, visitar, extra)) return false;
    return true;
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
    iter_in_order(arbol->nodo_raiz, visitar, extra);    
}
//***************************************************************************
//                              ITERADOR EXTERNO

void apilar_in_order(nodo_t* nodo, pila_t* pila){
    if (!nodo) return;
    pila_apilar(pila, nodo);
    apilar_in_order(nodo->hijo_izq, pila);
}

abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
    abb_iter_t* iter = malloc(sizeof(abb_iter_t));
    if (iter == NULL){
        return NULL;
    }
    iter->pila = pila_crear();
    if (iter->pila == NULL){
        return NULL;
    }
    iter->abb = arbol;
    if (arbol->nodo_raiz == NULL){
        return iter;
    }
    apilar_in_order(arbol->nodo_raiz, iter->pila);
    return iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter){
    if(pila_esta_vacia(iter->pila)){
        return false;
    }
    nodo_t* desapilado = pila_desapilar(iter->pila);
    apilar_in_order(desapilado->hijo_der, iter->pila);
    return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
    if (pila_esta_vacia(iter->pila)){
        return NULL;
    }
    return ((nodo_t*)pila_ver_tope(iter->pila))->clave;
}

bool abb_iter_in_al_final(const abb_iter_t *iter){
    return pila_esta_vacia(iter->pila);
}

void abb_iter_in_destruir(abb_iter_t* iter){
    pila_destruir(iter->pila);
    free(iter);
}