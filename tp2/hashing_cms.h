#ifndef HASH_H
#define HASH_H

#include <stdbool.h>
#include <stddef.h>

struct hash;

typedef struct hash hash_t;

/* Crea el hash
 */
hash_t* hash_crear_cms();

/* aumenta en uno el contador de apariciones de la clave
 * y devuelve la cantidad de repeticiones minima
 * Pre: La estructura hash fue inicializada
 * Post: Se aumento en 1 en contador
 */
void hash_guardar_cms (hash_t *hash, const char *clave);

/* devuelve el valor que se encuentra en la posicion 
 * de la clave provista
 * Pre: La estructura hash fue inicializada
 * Post: se devuelve el valor
 */
size_t hash_obtener_cms (hash_t *hash, const char *clave);

/* Destruye la estructura
 * Pre: La estructura hash fue inicializada
 * Post: La estructura hash fue destruida
 */
void hash_destruir_cms(hash_t *hash);
#endif  // HASH_H