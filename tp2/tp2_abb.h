#include <stdbool.h>
#include <stddef.h>
/*
*********************************************************************************
*                   DEFINICION DE LOS TIPOS DE DATOS
*********************************************************************************
*/
typedef struct abb abb_t;

typedef struct abb_iter abb_iter_t;


typedef int (*abb_comparar_clave_t) (const char *, const char *);
typedef void (*abb_destruir_dato_t) (void *);

/*
    crea un arbol binario de busqueda
    post:el ABB fue creado
*/
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);
/*
guarda un elemento en el ABB
pre:el ABB fue creado
post:devuelve true si el elemento se guardo correctamente
devuelve false en el caso contrario
*/
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);
/*
elimina un dato de el ABB
pre:el ABB fue creado
post:devuelve el valor y lo elimina del ABB en caso de que se encuentre
en caso contrario devuelve NULL.
*/
void *abb_borrar(abb_t *arbol, const char *clave);
/*
devuelve un valor de el ABB
pre: el ABB fue creado
post: devuelve el valor en la posicion de la clave,
en caso contrario devuelve NULL
*/
void *abb_obtener(const abb_t *arbol, const char *clave);
/*
confirma la existecia de un valor dentro del ABB
pre: el ABB fue creado
post:devuelve True si el elemento existe en el ABB
devuelve Flase en el caso contrario
*/
bool abb_pertenece(const abb_t *arbol, const char *clave);
/*
devuelve la contidad de elementos en el ABB
*/
size_t abb_cantidad(const abb_t *arbol);
/*
destruye el ABB
*/
void abb_destruir(abb_t *arbol);

/*
*********************************************************************************
*                             ITERADOR INTERNO
*********************************************************************************
*/
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);

/*
*********************************************************************************
*                            ITERADOR EXTERNO
*********************************************************************************
*/

/*
crea un iterador de un ABB.
pre:ABB fue creado.
/post:devuelve un iterador de un ABB.
*/
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);
/*
el iterador avanza al siguiente objeto de la ABB.
pre: el iter fue creado.
post: el iterador avanza al siguiente elemento de la ABB y devuelve true,
en caso de que este en el ultimo elemento no avanza y devuelve false.
*/
bool abb_iter_in_avanzar(abb_iter_t *iter);
/*
muestra el objeto al que esta apuntando actualmente el iterador.
pre: el iterador fue creado.
post: devuelve el objeto apuntado por el iter.
*/
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);
/*
lleva al tier a su ultimo elemento
pre: el iter fue creado.
post: devuelve true si el iter se encuentra en su ultimo elemento.
*/
bool abb_iter_in_al_final(const abb_iter_t *iter);
/* 
destruye el iterador
pre: el iterador fue creado
post: el iterador fue destruido
*/
void abb_iter_in_destruir(abb_iter_t* iter);


