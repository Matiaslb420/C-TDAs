#define _POSIX_C_SOURCE 200809L
#include <stdbool.h>
#include <stddef.h>
#include "hashing_cms.h"
#include "tp2_heap.h"
#include "tp2_strutil.h"
#include "tp2_abb.h"
#include "cola.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


typedef struct tweet{
    char* nombre;
    size_t cantidad;
}tweet_t;

int cmp_num(const void* tw1, const void* tw2){
    if (((tweet_t*)tw1)->cantidad == ((tweet_t*)tw2)->cantidad) return 0;
    return (((tweet_t*)tw1)->cantidad > ((tweet_t*)tw2)->cantidad) ? 1 : -1;
}

tweet_t * crear_tweet(char* nombre, size_t cantidad){
    tweet_t * tweet = (tweet_t*)malloc(sizeof(tweet_t));
    if(tweet == NULL)return NULL;
    tweet->nombre = strdup(nombre);
    tweet->cantidad = cantidad;
    return tweet;
}

void destruir_tweet(void* tweet){
    tweet_t* twit = (tweet_t*)tweet;
    free(twit->nombre);
    free(twit);
}

abb_t* heap_a_arbol(heap_t* heap, size_t* tweet_cant){
    abb_t* abb = abb_crear(strcmp, NULL);
    tweet_t* twit = heap_desencolar(heap);
    abb_guardar(abb,twit->nombre, NULL);
    *tweet_cant = twit->cantidad;
    destruir_tweet(twit);
    while (!heap_esta_vacio(heap) && ((tweet_t*)heap_ver_max(heap))->cantidad == *tweet_cant){
        twit = heap_desencolar(heap);
        abb_guardar(abb,twit->nombre, NULL);
        destruir_tweet(twit);
    }
    return abb;
}

void imprimir_heap(heap_t *heap, size_t cant_tt, int* contador){
    size_t tt_imp = 0;
    size_t tweet_cant = 0;
    abb_t* abb = heap_a_arbol(heap, &tweet_cant);
    abb_iter_t* iter = abb_iter_in_crear(abb);
    printf("--- %d\n", ++*contador);
    while(tt_imp < cant_tt){
        printf("%lu %s\n",tweet_cant, abb_iter_in_ver_actual(iter));
        tt_imp++;

        abb_iter_in_avanzar(iter);
        if(abb_iter_in_al_final(iter)){
            if(heap_esta_vacio(heap)) break;
            abb_iter_in_destruir(iter);
            abb_destruir(abb);
            abb = heap_a_arbol(heap, &tweet_cant);
            iter = abb_iter_in_crear(abb);
        }
    }
    abb_iter_in_destruir(iter);
    abb_destruir(abb);
}

/*
void imprimir_heap(heap_t *heap, size_t cant_tt, int* contador){
    size_t tt_imp = 0;
    tweet_t * tweet_imp = heap_desencolar(heap);
    abb_t * abb = abb_crear(strcmp,free);
    bool pertenece = true;
    size_t cant_anterior = tweet_imp->cantidad;
    printf("--- %d\n", ++*contador);
    fprintf(stdout,"%lu %s\n",tweet_imp->cantidad, tweet_imp->nombre);
    abb_guardar(abb, tweet_imp->nombre, NULL);
    tt_imp++;
    while(!heap_esta_vacio(heap) && tt_imp < cant_tt){
        if(!pertenece){
            fprintf(stdout,"%lu %s\n",tweet_imp->cantidad, tweet_imp->nombre);
            tt_imp++;
        }
        destruir_tweet(tweet_imp);
        tweet_imp = (tweet_t*)heap_desencolar(heap);
        if(strcmp(cant_anterior, tweet_imp->cantidad) == 0){
            if(!abb_pertenece(abb,tweet_imp->nombre)){
                abb_guardar(abb,tweet_imp->nombre,NULL);
                pertenece = false;
            }
            else{
                pertenece = true;
            }
        }
        else{
            free(cant_anterior);
            cant_anterior = strdup(tweet_imp->cantidad);
            abb_destruir(abb);
            abb = abb_crear(strcmp,free);
            abb_guardar(abb,tweet_imp->nombre,NULL);
            pertenece = false;
        }
    }
    free(cant_anterior);
    destruir_tweet(tweet_imp);
    abb_destruir(abb);
    contador++;
}
*/


ssize_t leer_tweets(size_t cant_lineas, size_t cant_tt,FILE *tweets_f, int* con, hash_t* hash){
    cola_t* cola = cola_crear();
    size_t tam = 0;
    char* linea = "\0";
    ssize_t sumar = getline(&linea, &tam, tweets_f);
    size_t contador = 0;
    ssize_t caracteres = 0;
    while (sumar != EOF){
        caracteres += sumar;
        linea[sumar - 1] = '\0';                 
        char** tweets = split(linea,',');
        for(int i = 1; tweets[i]; i++){
            cola_encolar(cola, strdup(tweets[i]));
            hash_guardar_cms(hash, tweets[i]);
        }
        contador++;
        free_strv(tweets);
        if (contador == cant_lineas) break;
        sumar = getline(&linea, &tam, tweets_f);
    }
    free(linea);
    heap_t* heap = heap_crear(cmp_num);
    contador = 0;
    tweet_t *tweet = NULL;
    char* desencolado = "\0";
    while (!cola_esta_vacia(cola)){
        desencolado = (char*)cola_desencolar(cola);
        tweet = crear_tweet(desencolado , hash_obtener_cms(hash, desencolado));
        heap_encolar(heap, tweet); 
        free(desencolado);       
    }
    if(!heap_esta_vacio(heap)){
        imprimir_heap(heap, cant_tt, con);
    }
    heap_destruir(heap,destruir_tweet);
    cola_destruir(cola, NULL);
    return ( sumar == EOF) ?  EOF : caracteres ;
}

int main(int argc, char** argv){
    size_t cant_lineas = strtol(argv[1],NULL,10);
    size_t cant_tt = strtol(argv[2],NULL,10);
    int contador = 0;
    hash_t* hash = hash_crear_cms();
    while(leer_tweets(cant_lineas,cant_tt, stdin, &contador, hash) != EOF);
    hash_destruir_cms(hash);
    return 0;
}
