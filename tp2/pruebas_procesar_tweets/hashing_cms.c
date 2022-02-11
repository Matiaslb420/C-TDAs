#define _POSIX_C_SOURCE 200809L
#include <stdbool.h>
#include <stddef.h>
#include "hashing_cms.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define CANT_HASHING 4

enum hashes {
    HASH_IMPROVISADO,
    FNV_HASH,
    JENKINS,
    HASH_SIMPLE_cms,
};

const static size_t TAMANO_ARREGLO = 1000;


//***********************************************************************************
//                              FUNCIONES DE HASHING
size_t hash_cms(char* name){
    size_t length = strlen(name);
    size_t hash_value = 0;
    for (size_t i = 0; i <length; i++){
        hash_value += (size_t)name[i];
        hash_value *= 4268770981;
    }
    return hash_value;
}

uint32_t jenkins_hash_cms(uint8_t* key, size_t length) {
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

size_t hash_simple_cms(char* name){
    size_t length = strlen(name);
    size_t hash_value = 0;
    for (size_t i = 0; i <length; i++){
        hash_value += (size_t)name[i];
        hash_value *= 10530229;
    }
    return hash_value;
}

uint64_t fnv_hashing_cms(char* clave){
    uint64_t h = 14695981039346656037U;
    size_t n = strlen(clave);
    for (size_t i = 0; i< n; i++){
        h *= 1099511628211;
        h ^= (uint64_t)clave[i];
    }
    return h;
}                      

size_t hashear_cms(char *clave,size_t tamano,int metodo ){
    size_t clave_hasheada = 0;
    if (metodo == HASH_IMPROVISADO){
        clave_hasheada = hash_simple_cms(clave) % tamano;
    }
    else if (metodo == FNV_HASH){
        clave_hasheada = (size_t)(fnv_hashing_cms(clave) % (uint64_t)tamano);
    }
    else if (metodo == JENKINS){
        clave_hasheada = jenkins_hash_cms((uint8_t*) clave, strlen(clave)) % tamano;
    }
    else{
        clave_hasheada = hash_cms(clave) % tamano;
    }
    return clave_hasheada;
}
//***************************************************************


struct hash{
    size_t* arreglo[CANT_HASHING];
    size_t tamano_actual;
    size_t cant_elem;
};

long hash_encontrar_cms(const hash_t* hash, const char* clave, int hashing){
    char* clave_dup = strdup(clave);
    long posicion = (long)hashear_cms(clave_dup,hash->tamano_actual, hashing);
    free(clave_dup);
    return posicion;
}

//primitivas
hash_t *hash_crear_cms(){
    hash_t* hash = malloc(sizeof(hash_t));
    if (hash == NULL)return NULL;
    for (int i = 0; i < CANT_HASHING; i++){
        hash->arreglo[i] = (size_t*)malloc(TAMANO_ARREGLO * sizeof(size_t));
        if (hash->arreglo[i] == NULL){
            free(hash);
            return NULL;
        }
        for (size_t j = 0;j < TAMANO_ARREGLO; j++){
            hash->arreglo[i][j] = 0;
        }
    }
    hash->tamano_actual = TAMANO_ARREGLO;
    hash->cant_elem = 0;
    return hash;
}

void hash_guardar_cms(hash_t *hash, const char *clave){
    long posicion = hash_encontrar_cms(hash, clave, 0);
    hash->arreglo[0][posicion]++;
    for (int i = 1; i < CANT_HASHING; i++){
        posicion = hash_encontrar_cms(hash, clave, i);
        hash->arreglo[i][posicion]++;
        
    }
}            

size_t hash_obtener_cms(hash_t *hash, const char *clave){
    long posicion = hash_encontrar_cms(hash, clave, 0);
    size_t menor = hash->arreglo[0][posicion];
    for (int i = 1; i < CANT_HASHING; i++){
        posicion = hash_encontrar_cms(hash, clave, i);
        if (hash->arreglo[i][posicion] < menor){
            menor = hash->arreglo[i][posicion];
        }
    }
    return menor;
}                                                                                                                                                                                                      //amongus

void hash_destruir_cms(hash_t *hash){
    for(int i = 0; i < CANT_HASHING;i++){
        free(hash->arreglo[i]);
    }
    free(hash);
}
