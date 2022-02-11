#define _POSIX_C_SOURCE 200809L
#include <stdbool.h>
#include <stddef.h>
#include "tp2_hash.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

enum estado{
    OCUPADO,
    BORRADO,
    VACIO,
};
const float REDUCIR = 0.5;
const float AGRANDAR = 2.0;

const static size_t TAMANO_ARREGLO = 79;
const static long RONDAS_MAX = 10;
const static int VUELTAS = 4;
const static int HASH_IMPROVISADO = 0;
const static int FNV_HASH = 1;
const static int JENKINS = 2;
//const static int HASH_SIMPLE = 3;

//***********************************************************************************
//                              FUNCIONES DE HASHING
size_t hash(char* name){
    size_t length = strlen(name);
    size_t hash_value = 0;
    for (size_t i = 0; i <length; i++){
        hash_value += (size_t)name[i];
        hash_value *= 4268770981;
    }
    return hash_value;
}

uint32_t jenkins_hash(uint8_t* key, size_t length) {
    size_t i = 0;
     uint32_t hash = 0;
      while (i != length) {
        hash += key[i++];
        hash += hash << 10;
        hash ^= hash >> 6;
      }
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    return hash;
}

size_t hash_simple(char* name){
    size_t length = strlen(name);
    size_t hash_value = 0;
    for (size_t i = 0; i <length; i++){
        hash_value += (size_t)name[i];
        hash_value *= 10530229;
    }
    return hash_value;
}

uint64_t fnv_hashing(char* clave){
    uint64_t h = 14695981039346656037U;
    size_t n = strlen(clave);
    for (size_t i = 0; i< n; i++){
        h *= 1099511628211;
        h ^= (uint64_t)clave[i];
    }
    return h;
}
/*
Para las colisiones revisa los elementos consecutivos hasta cierto numero y, si no hay lugar, prueba otro hashing.
Solo que no reposiciona el elemento que ya estaba guardado sinó que se queda buscando lugar para el elemento que quiere guardar desde el inicio.
Reconocemos que esto es una forma de programarlo poco ortodoxa y que no termina de ser cuckoo. Pero es debido a que en su momento no lograbamos hacer funcionar
las funciones de hashing para que guarden las posiciones de forma distribuida. Por lo que, tras mucho esfuerzo, esto es lo que conseguimos luego de prueba y error.
*/                                  

size_t hashear(char *clave,size_t tamano,int metodo ){
    size_t clave_hasheada = 0;
    if (metodo == HASH_IMPROVISADO){
        clave_hasheada = hash_simple(clave) % tamano;
    }
    else if (metodo == FNV_HASH){
        clave_hasheada = (size_t)(fnv_hashing(clave) % (uint64_t)tamano);
    }
    else if (metodo == JENKINS){
        clave_hasheada = jenkins_hash((uint8_t*) clave, strlen(clave)) % tamano;
    }
    else{
        clave_hasheada = hash(clave) % tamano;
    }
    return clave_hasheada;
}
//***************************************************************
typedef struct espacio{
    char* clave;
    void* valor;
    enum estado estado;
} espacio_t;

struct hash{
    espacio_t* arreglo;
    hash_destruir_dato_t funcion_destruir;
    size_t tamano_actual;
    size_t cant_elem;
};

struct hash_iter{
    const hash_t* hash;
    long posicion_actual;
};


bool espacio_asignar(espacio_t* espacio, char* clave, void* dato, enum estado estado){
    espacio->clave = clave;
    espacio->valor = dato;
    espacio->estado = estado;
    return true;
}

long hash_encontrar(const hash_t* hash, const char* clave, enum estado* estado){
    int vuelta = 0;
    char* clave_dup = strdup(clave);
    long posicion = (long)hashear(clave_dup,hash->tamano_actual, vuelta);
    if (hash->cant_elem == 0){
        *estado = VACIO;
        free(clave_dup);
        return posicion;
    }
    long rondas;
    long posicion_borrado = -1;
    while (vuelta < VUELTAS){
        rondas = 0;
        for (; rondas < RONDAS_MAX;){
            if (posicion + rondas >= (long)hash->tamano_actual){
                posicion = 0;
            }
            if (hash->arreglo[posicion + rondas].estado == VACIO){
                *estado = VACIO;
                free(clave_dup);
                return posicion + rondas;
            }
            if (hash->arreglo[posicion + rondas].estado == BORRADO){
                if (posicion_borrado == -1){
                    posicion_borrado = posicion + rondas;
                }
            }
            else if (strcmp(hash->arreglo[posicion + rondas].clave, clave) == 0){
                *estado = OCUPADO;
                free(clave_dup);
                return posicion + rondas;
            }
            rondas++;
        }
        vuelta++;
        posicion = (long) hashear(clave_dup, hash->tamano_actual, vuelta);
    }
    *estado = BORRADO;
    free(clave_dup);
    return posicion_borrado;
}       

bool redimensionar(hash_t* hash, float operacion){
    hash->tamano_actual = (size_t)((float)hash->tamano_actual * operacion);
    espacio_t* arreglo_nuevo = (espacio_t*)malloc(hash->tamano_actual * sizeof(espacio_t));
    if (arreglo_nuevo == NULL) return false;
    for (size_t i = 0; i < hash->tamano_actual; i++){
        espacio_asignar(&arreglo_nuevo[i], NULL, NULL, VACIO);
    }
    espacio_t* arreglo_antiguo =hash->arreglo;
    hash->arreglo = arreglo_nuevo;
    size_t elementos_actuales = hash->cant_elem;
    int i = 0;
    long posicion = 0;
    enum estado estado = BORRADO;
    while (elementos_actuales > 0){
        if (arreglo_antiguo[i].estado == OCUPADO){
            posicion = hash_encontrar(hash, arreglo_antiguo[i].clave, &estado);
            if (posicion == -1){
                return false;
            }
            espacio_asignar(&hash->arreglo[posicion], arreglo_antiguo[i].clave, arreglo_antiguo[i].valor, OCUPADO);
            elementos_actuales--;
        }
        i++;
    }
    free(arreglo_antiguo);
    return true;
}

//primitivas
hash_t *hash_crear(hash_destruir_dato_t destruir_dato){
    hash_t* hash = malloc(sizeof(hash_t));
    if (hash == NULL){
        return NULL;
    }
    hash->arreglo = (espacio_t*)malloc(TAMANO_ARREGLO * sizeof(espacio_t));
    if (hash->arreglo == NULL){
        free(hash);
        return NULL;
    }
    for (size_t i = 0;i < TAMANO_ARREGLO; i++){
        espacio_asignar(&hash->arreglo[i], NULL, NULL, VACIO);
    }
    hash->funcion_destruir = destruir_dato;
    hash->tamano_actual = TAMANO_ARREGLO;
    hash->cant_elem = 0;
    return hash;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato){
    if ((hash->cant_elem * 2) >= (hash->tamano_actual)){
        redimensionar(hash, AGRANDAR);
    }
    enum estado estado = BORRADO;
    long posicion = hash_encontrar(hash, clave, &estado);
    if (estado == BORRADO && posicion == -1){
        return false;
    }
    if(estado != OCUPADO){
        espacio_asignar(&hash->arreglo[posicion], strdup(clave), dato, OCUPADO);
        hash->cant_elem++;
        return true;
    }
    if(hash->funcion_destruir != NULL) hash->funcion_destruir(hash->arreglo[posicion].valor);
    espacio_asignar(&hash->arreglo[posicion], hash->arreglo[posicion].clave, dato, OCUPADO);
    return true;
}                                                                                                                                                                                                    //amongus

void *hash_borrar(hash_t *hash, const char *clave){
    if(hash->cant_elem *4 <= hash->tamano_actual){
        redimensionar(hash, REDUCIR);
    }
    enum estado estado = BORRADO;
    long posicion = hash_encontrar(hash,clave, &estado);
    if (estado == BORRADO || estado == VACIO){
        return NULL;
    }
    hash->arreglo[posicion].estado = BORRADO;
    free(hash->arreglo[posicion].clave);
    hash->arreglo[posicion].clave = NULL;
    void* resultado = hash->arreglo[posicion].valor;
    hash->arreglo[posicion].valor = NULL;
    hash->cant_elem--;
    return resultado;
}

void* hash_obtener(const hash_t* hash, const char* clave){
    enum estado estado = BORRADO;
    long pos = hash_encontrar(hash, clave, &estado);
    if (estado == BORRADO || estado == VACIO){
        return NULL;
    }
    return hash->arreglo[pos].valor;
}

bool hash_pertenece(const hash_t* hash, const char *clave){
    enum estado estado = BORRADO;
    hash_encontrar(hash, clave, &estado);
    return (estado == OCUPADO);
}

size_t hash_cantidad(const hash_t *hash){
    return hash->cant_elem;
}

void hash_destruir(hash_t *hash){
    size_t i = 0;
    while (hash->tamano_actual > i){
        if (hash->arreglo[i].estado == OCUPADO){
            if (hash->funcion_destruir != NULL){
                hash->funcion_destruir(hash->arreglo[i].valor);
            }
            free(hash->arreglo[i].clave);
        }
        hash->cant_elem--;
        i++;
    }
    free(hash->arreglo);
    free(hash);
}

// INICIO DEL ITERADOR*********************************************************

size_t hash_iter_siguiente_elemento(hash_iter_t* iter, size_t posicion){
    while(posicion + 1 <= iter->hash->tamano_actual){
        if (iter->hash->arreglo[posicion].estado == OCUPADO){
            break;
        }
        posicion++;
    }
    return posicion;
}

hash_iter_t *hash_iter_crear(const hash_t *hash){
    hash_iter_t* iter = (hash_iter_t*)malloc(sizeof(hash_iter_t));
    if (iter == NULL){
        return NULL;
    }
    iter->hash = hash;
    if (hash->cant_elem == 0){
        iter->posicion_actual = -1;
    }
    else {
        iter->posicion_actual = hash_iter_siguiente_elemento(iter, 0);
    }
    return iter;
}

bool hash_iter_avanzar(hash_iter_t *iter){
    if (hash_iter_al_final(iter)){
        return false;
    }
    size_t iterador_indice = (size_t)iter->posicion_actual + 1;
    if (iterador_indice > iter->hash->tamano_actual){
        iter->posicion_actual = -1;
        return false;
    }
    iterador_indice = hash_iter_siguiente_elemento(iter, iterador_indice);
    if (iterador_indice >= iter->hash->tamano_actual){
        iter->posicion_actual = -1;
        return false;
    }
    else if (iter->hash->arreglo[iterador_indice].estado == BORRADO){
        iter->posicion_actual = -1;
        return false;
    }
    iter->posicion_actual = (long)iterador_indice;
    return true; 
}

const char *hash_iter_ver_actual(const hash_iter_t *iter){
    if (hash_iter_al_final(iter)){
        return NULL;
    }
    return iter->hash->arreglo[iter->posicion_actual].clave;
}

bool hash_iter_al_final(const hash_iter_t *iter){
    return iter->posicion_actual == -1;
}

void hash_iter_destruir(hash_iter_t *iter){
    free(iter);
}