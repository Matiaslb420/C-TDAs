#include "pila.h"
#include <stdlib.h>

/* Definición del struct pila proporcionado por la cátedra.
 */struct pila {
    void** datos;
    size_t cantidad;  // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/
size_t TAM_INICIAL = 4;

pila_t* pila_crear(){
    size_t tam_inicial = TAM_INICIAL;
    pila_t* pila = malloc(sizeof(pila_t));if (pila == NULL){ return false; }
    pila->datos = malloc(tam_inicial * sizeof(void*));if (pila->datos == NULL){ return false; }
    pila ->cantidad = 0;
    pila ->capacidad = 4;
    return pila;
}
// duplica la capacidad de una pila
// pre: la cantidad de elementos de la pila es igual a su capacidad
// post: la capacidad de la pila se duplica
void pila_crecer(pila_t *pila){
    size_t tam_nuevo = pila->capacidad * 2;
    void** datos_nuevos = realloc(pila->datos,tam_nuevo * sizeof(void*));
    pila->datos = datos_nuevos;
    pila->capacidad = pila->capacidad*2;
}
// divide la capacidad de una pila
// pre: la cantidad de elementos de la pila es un cuarto de su capacidad
// post: la capacidad de la pila se divide
void pila_decrecer(pila_t *pila){
    size_t tam_nuevo = pila->capacidad / 2;
    void** datos_nuevos = realloc(pila->datos, tam_nuevo * sizeof(void*));
    pila->datos = datos_nuevos;
    pila->capacidad = pila->capacidad/2;
}

void pila_destruir(pila_t *pila){
    free(pila->datos);
    free(pila);
}

bool pila_esta_vacia(const pila_t *pila){
    if (pila->cantidad == 0){
        return true;
    }
    return false;
}

bool pila_apilar(pila_t *pila, void* valor){
    if (pila == NULL){
        return false;
    }
    
    if (pila->capacidad == pila->cantidad){
        pila_crecer(pila);
    }

    pila->datos[pila->cantidad] = valor;
    size_t aux = 1;
    pila->cantidad = pila->cantidad + aux;
    return true;
}

void* pila_ver_tope(const pila_t *pila){
    if (pila_esta_vacia(pila)){
        return NULL;
    }
    return pila->datos[pila->cantidad-1];
}

void* pila_desapilar(pila_t *pila){
    if (pila_esta_vacia(pila)){
        return NULL;
    }
    pila->cantidad = pila->cantidad-1;
    if (pila->cantidad*4 <= pila->capacidad && pila->capacidad > 4){
        pila_decrecer(pila);
    }
    return pila->datos[pila->cantidad];
}



