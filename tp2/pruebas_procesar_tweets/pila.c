#include "pila.h"
#include <stdio.h>
#include <stdlib.h>

const size_t tamano_datos = 2;
const size_t cantidad_inicial = 0;
const size_t capacidad_inicial = 2;
const float duplicar = 2.0;
const float reducir = 0.5;

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void **datos;
    size_t cantidad;   // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                   PRIMITIVAS DE LA PILA
 * *****************************************************************/

pila_t *pila_crear(void){
	pila_t *pila = malloc(sizeof(pila_t));
	if (pila == NULL){
		return NULL;
	}
	pila->datos = malloc(tamano_datos* sizeof(void*));
	if(pila->datos == NULL){
		free(pila);
		return NULL;
	}
	pila->cantidad = cantidad_inicial;
	pila->capacidad = capacidad_inicial;
	return pila;
}

bool pila_redimensionar(pila_t *pila,const float operacion){
	void** datos_nuevo = realloc(pila->datos, ((size_t)((float)pila->capacidad * operacion)) * sizeof(void*));
	if (datos_nuevo == NULL){
		return false;
		}	
	pila->capacidad = ((size_t)((float)pila->capacidad * operacion));
	pila->datos = datos_nuevo;
	return true;
}

void pila_destruir(pila_t *pila){
	free(pila->datos);
	free(pila);
}

bool pila_esta_vacia(const pila_t *pila){
	return pila->cantidad == 0;
}

bool pila_apilar(pila_t *pila, void *valor){
	if(pila->cantidad == pila->capacidad){
		if (!pila_redimensionar(pila,duplicar)){
			return false;
		}
	}
	pila->datos[pila->cantidad] = valor;
	pila->cantidad++;
	return true;
}

void *pila_ver_tope(const pila_t *pila){
	if(pila_esta_vacia(pila)){
		return NULL;
		}
	return pila->datos[pila->cantidad -1];
}

void *pila_desapilar(pila_t *pila){
	if(pila_esta_vacia(pila)){
		return NULL;
		}
	if (pila->cantidad <= pila->capacidad/4){
		if(!pila_redimensionar(pila, reducir)){
			return NULL;
			}
		}
	pila->cantidad = pila->cantidad -1;
	return pila->datos[pila->cantidad];
}























