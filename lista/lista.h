#ifndef _LISTA_H
#define _LISTA_H

#include <stddef.h>
#include <stdbool.h>

// Primitivas de la lista:

typedef struct lista lista_t;
/* se crea una lista

post: retorna el puntero lista
*/
lista_t *lista_crear(void);
/*
pre: la lista fue creada
post:devuelve true si esta vacia, false si no
*/
bool lista_esta_vacia(const lista_t *lista);

/*
inserta algo al inicio de la lista
*/
bool lista_insertar_primero(lista_t *lista, void *dato);
/*
idem arriba pero al ultimo
*/
bool lista_insertar_ultimo(lista_t *lista, void *dato);
/*
borra el primer dato de la lista y lo retorna
*/
void *lista_borrar_primero(lista_t *lista);
/*
retorna el primer dato de la lista
*/
void *lista_ver_primero(const lista_t *lista);
/*
idem arriba pero con el ultimo
*/
void *lista_ver_ultimo(const lista_t* lista);
/*
retorna el largo de la lista
*/
size_t lista_largo(const lista_t *lista);
/*
destruye la lista, se puede pasar la funcion destruir dato opcionalmente,
*/
void lista_destruir(lista_t *lista, void (*destruir_dato)(void *));

//Primitiva del iterador interno
/*
itera de forma interna, esto en teoria es parte de la implementacion asi que no se si debería estar definido acá
*/
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);

//Primitivas del iterador externo

typedef struct lista_iter lista_iter_t;
/*
crea un iterador y devuelve el puntero iterador
*/
lista_iter_t *lista_iter_crear(lista_t *lista);
/*
avanza un dato en el iterador
*/
bool lista_iter_avanzar(lista_iter_t *iter);
/*
devuelve el dato actual en el iterador
*/
void *lista_iter_ver_actual(const lista_iter_t *iter);
/*
devuelve true si el iterador está al final, false si no
*/
bool lista_iter_al_final(const lista_iter_t *iter);
/*
destruye el iterador
*/
void lista_iter_destruir(lista_iter_t *iter);
/*
inserta un dato donde esta el iterador situado
*/
bool lista_iter_insertar(lista_iter_t *iter, void *dato);
/*
borra el dato que está apuntanto actualmente
*/
void *lista_iter_borrar(lista_iter_t *iter);









































#endif 
