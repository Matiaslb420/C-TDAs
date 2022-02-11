#include <stdlib.h>
#include "cola.h"
#include <stdio.h>

typedef struct nod {
	void* valor;
	struct nod* nodo_siguiente;
} nod_t;


struct cola {
	nod_t* nodo_i;
	nod_t* nodo_f;
};
//crea un nodo y le asigna un valor
nod_t* cola_nodo_crear(void* valor){
	nod_t* nodo = malloc(sizeof(nod_t));
	if (nodo == NULL){
		return NULL;
	}
	nodo->valor = valor;
	return nodo;
}

cola_t* cola_crear(void) {
	cola_t* cola = malloc(sizeof(cola_t));
	if (cola == NULL){
		return NULL;
	}
	cola->nodo_i = NULL;
	return cola;
}

bool cola_esta_vacia(const cola_t *cola){
	return (cola->nodo_i == NULL);
}

bool cola_encolar(cola_t *cola, void* valor){
	nod_t* nuevo_inicio = cola_nodo_crear(valor);
	if (nuevo_inicio == NULL){
		return false;
	}
	if (cola_esta_vacia(cola)){
		cola->nodo_i = nuevo_inicio;
		cola->nodo_i->nodo_siguiente = nuevo_inicio;
	}
	else {
		cola->nodo_f->nodo_siguiente = nuevo_inicio;
	}
	cola->nodo_f = nuevo_inicio;
	return true;
}

void *cola_ver_primero(const cola_t *cola){
	if (cola_esta_vacia(cola)){
		return NULL;
	}
	return cola->nodo_i->valor;
}

void *cola_desencolar(cola_t *cola){
	if (cola_esta_vacia(cola)){
		return NULL;
	}
	void* valor = cola->nodo_i->valor;
	nod_t* proximo_nodo = NULL;
	if (cola->nodo_i != cola->nodo_f || cola->nodo_i == NULL){
		proximo_nodo = cola->nodo_i->nodo_siguiente;
	}
	free(cola->nodo_i);
	cola->nodo_i = proximo_nodo;
	return valor;
}

void cola_destruir(cola_t *cola, void (*destruir_dato)(void *)){
	while (cola->nodo_i != NULL) {
		if (destruir_dato){
			destruir_dato(cola->nodo_i->valor);
		}
		
		nod_t* proximo_nodo = NULL;
		if (cola->nodo_i != cola->nodo_f){
			proximo_nodo = cola->nodo_i->nodo_siguiente;
		}
		free(cola->nodo_i);
		cola->nodo_i = proximo_nodo;
		}
	free(cola);
}

