#include <stdlib.h>
#include "lista.h"
#include <stdio.h>
#include <stddef.h>

typedef struct nodo {
	void* valor;
	struct nodo* nodo_siguiente;
} nodo_t;

struct lista {
	nodo_t* nodo_i;
	nodo_t* nodo_f;
	size_t largo;
};

nodo_t* nodo_crear(void* valor){
	nodo_t* nodo = malloc(sizeof(nodo_t));
	if (nodo == NULL){
		return NULL;
	}
	nodo->valor = valor;
	nodo->nodo_siguiente = NULL;
	return nodo;
}

lista_t* lista_crear(void) {
	lista_t* lista = malloc(sizeof(lista_t));
	if (lista == NULL){
		return NULL;
	}
	lista->nodo_i = NULL;
	lista->nodo_f = NULL;
	lista->largo = 0;
	return lista;
}

bool lista_esta_vacia (const lista_t *lista){
	return (lista->largo == 0);
}

bool lista_insertar_primero(lista_t *lista, void *dato){
	nodo_t* proximo = lista->nodo_i;
	nodo_t* nodo_nuevo = nodo_crear(dato);
	if (nodo_nuevo == NULL){
		return false;
	}
	lista->nodo_i = nodo_nuevo;
	lista->nodo_i->nodo_siguiente = proximo;
	lista->largo++;
	if (lista_largo(lista) == 1){
		lista->nodo_f = nodo_nuevo;
	}
	return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato){
	if (lista_esta_vacia(lista)){
		return lista_insertar_primero(lista, dato);
	}
	nodo_t* nodo_nuevo = nodo_crear(dato);
	if (nodo_nuevo == NULL){
		return false;
	}
	nodo_t* nodo_penultimo = lista->nodo_f;
	lista->nodo_f = nodo_nuevo;
	nodo_penultimo->nodo_siguiente = lista->nodo_f;//dgdgsgsgs
	lista->largo++;
	return true;
}

void *lista_borrar_primero(lista_t *lista){
	if (lista_esta_vacia(lista)){
		return NULL;
	}
	void* dato = lista->nodo_i->valor;
	nodo_t* nuevo_inicio = lista->nodo_i->nodo_siguiente;
	free(lista->nodo_i);
	lista->largo--;
	lista->nodo_i = nuevo_inicio;
	if (lista_largo(lista) <= 1){
		lista->nodo_f = nuevo_inicio;
	}
	return dato;
}

void *lista_ver_primero(const lista_t *lista){
	if (lista_esta_vacia(lista)){
		return NULL;
	}
	return lista->nodo_i->valor;
}

void *lista_ver_ultimo(const lista_t* lista){
	if (lista_esta_vacia(lista)){
		return NULL;
	}
	return lista->nodo_f->valor;
}

size_t lista_largo(const lista_t *lista){
	return lista->largo;
}
//rehacer para que no acceda a nodos que no extisten!!! o algo

void lista_destruir(lista_t* lista, void (*destruir_dato)(void *)){
	while (!lista_esta_vacia(lista)){
		nodo_t* nuevo_p = lista->nodo_i->nodo_siguiente;
		if (destruir_dato != NULL){
			destruir_dato(lista->nodo_i->valor);
		}
		free(lista->nodo_i);
		lista->nodo_i = nuevo_p;
		lista->largo--;
	}
	free(lista);
}

void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra){
	nodo_t* nodo_actual = lista->nodo_i;
	for (size_t i=0;i < lista_largo(lista); i++){
		if (!visitar(nodo_actual->valor, extra)){
			return;
		}
		nodo_actual = nodo_actual->nodo_siguiente;
	}
}

struct lista_iter {
	lista_t* lista;
	nodo_t* nodo_anterior;
	nodo_t* nodo_actual;
};

lista_iter_t* lista_iter_crear(lista_t *lista){
	lista_iter_t* iter = malloc(sizeof(lista_iter_t));
	if (iter == NULL){
		return NULL;
	}
	iter->lista = lista;
	iter->nodo_actual = lista->nodo_i;
	iter->nodo_anterior = NULL;
	
	return iter;
}

bool lista_iter_avanzar(lista_iter_t *iter){
	if (!lista_iter_al_final(iter)){
		nodo_t* nodo_siguiente = iter->nodo_actual->nodo_siguiente;
		iter->nodo_anterior = iter->nodo_actual;
		iter->nodo_actual = nodo_siguiente;
		return true;
	}
	return false;
}

void *lista_iter_ver_actual(const lista_iter_t *iter){
	if (lista_iter_al_final(iter)){
		return NULL;
	}
	return iter->nodo_actual->valor;
}

bool lista_iter_al_final(const lista_iter_t *iter){
	return (iter->nodo_actual == NULL);
}

void lista_iter_destruir(lista_iter_t *iter){
	free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato){
	nodo_t* nuevo_nodo = nodo_crear(dato);
	if (nuevo_nodo == NULL){
		return false;
	}
	nodo_t* nodo_siguiente = iter->nodo_actual; //que puede ser NULL
	if (lista_iter_al_final(iter)){
		iter->lista->nodo_f = nuevo_nodo;
	}
	iter->nodo_actual = nuevo_nodo;
	if (!lista_esta_vacia(iter->lista)){
		if (iter->nodo_anterior != NULL){
			iter->nodo_anterior->nodo_siguiente = iter->nodo_actual;
		}
		iter->nodo_actual->nodo_siguiente = nodo_siguiente;
	}
	iter->lista->largo++;
	
	if (iter->nodo_anterior == NULL){
		iter->lista->nodo_i = nuevo_nodo;
	}

	return true;
}

void *lista_iter_borrar(lista_iter_t *iter){
	if (lista_iter_al_final(iter)){
		return NULL;
	}
	nodo_t* nodo_siguiente = iter->nodo_actual->nodo_siguiente;
	if (iter->nodo_anterior != NULL){
		iter->nodo_anterior->nodo_siguiente = nodo_siguiente;
	}
	else{
		iter->lista->nodo_i = nodo_siguiente;
	}
	void* dato = iter->nodo_actual->valor;
	free(iter->nodo_actual);
	iter->nodo_actual = nodo_siguiente;
	iter->lista->largo--;
	if (nodo_siguiente == NULL){
		iter->lista->nodo_f = iter->nodo_anterior;
	}
	return dato;
}



