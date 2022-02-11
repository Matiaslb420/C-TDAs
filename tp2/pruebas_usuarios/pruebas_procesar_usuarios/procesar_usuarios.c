#define _POSIX_C_SOURCE 200809L
#include <stdbool.h>
#include <stddef.h>
#include "tp2_hash.h"
#include "tp2_strutil.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct usuario {
    size_t hashtag_cant;
    char* nombre;
    char* nombre_a;
} usuario_t;

void destruir_subhash(void* hashito){
    hash_destruir((hash_t*)hashito);
}

hash_t* guardar_hashtags(char** elementos){

    hash_t* hash_tags = hash_crear(free);
    if (!hash_tags) return NULL;
    for(int i = 1; elementos[i]; i++){
        hash_guardar(hash_tags, elementos[i], NULL);
    }
    return hash_tags;
}

void actualizar_hashtags(hash_t* usuarios,char** elementos){
    hash_t* hash_tags = (hash_t*)hash_obtener(usuarios, elementos[0]);
    for(int i = 1; elementos[i]; i++){
        hash_guardar(hash_tags, elementos[i], NULL);
    }
}

hash_t* procesar_usuarios(char* archivo){
    FILE* archivo_f = fopen(archivo, "r");
    hash_t* usuarios = hash_crear(destruir_subhash);
    char* linea = "\0";
    size_t tam = 0;
    char** elementos = &linea;
    size_t contador = 0;
    ssize_t sumar = getline(&linea, &tam, archivo_f);
    while(sumar != EOF){
        linea[sumar -1] = '\0';
        elementos = split(linea, ',');
        if(!hash_pertenece(usuarios, elementos[0])){
            hash_guardar(usuarios, elementos[0], (void*)guardar_hashtags(elementos));
        }
        else{
            actualizar_hashtags(usuarios, elementos);
        }
        //printf("cant: %lu\n",hash_cantidad(hash_obtener(usuarios, elementos[0])));
        free_strv(elementos);
        contador++;
        sumar = getline(&linea, &tam, archivo_f);
    }
    free(linea);
    fclose(archivo_f);
    return usuarios;
}

char* inicializar_nombre(const char* nombre, char* salida){
    for (int i = 0; nombre[i]; i++){
        salida[i] = nombre[i];
    }
    return strdup(salida);
}

usuario_t* count_sort_nombres(usuario_t* ranking, size_t cantidad, int indice, int digito){
    usuario_t* resultado = calloc(cantidad+1,sizeof(usuario_t));
    int frecuencia[10] = {0,0,0,0,0,0,0,0,0,0};
    int ascii = 0;
    for (int i = 0; i < cantidad; i++){
        
        ascii = ((int)ranking[i].nombre_a[indice]) - 32;
        frecuencia[(ascii / digito) % 10]++;
    }
    int posiciones[10] = {0,0,0,0,0,0,0,0,0,0};
    for (int i = 1; i < 10; i++){
        posiciones[i] = posiciones[i - 1] + frecuencia[i - 1];
    }
    for(int i = 0; i < cantidad; i++){
        ascii = ((int)ranking[i].nombre_a[indice]) - 32;
        resultado[posiciones[(ascii / digito) % 10]] = ranking[i];
        posiciones[(ascii / digito) % 10]++;
    }
    free(ranking);
    return resultado;
}

usuario_t* radix_sort_nombres(usuario_t* ranking,size_t cantidad){
    int indice = 14;
    for (; indice >= 0; indice--){
        for(int digito = 1; digito <=10; digito *= 10){
            ranking = count_sort_nombres(ranking, cantidad, indice, digito);
        }
    }
    return ranking;
}

usuario_t* count_sort_hashtags(usuario_t* ranking, size_t cantidad, size_t decimal){
    usuario_t* resultado = (usuario_t*)calloc( cantidad+1,sizeof(usuario_t));
    int frecuencia[10] = {0,0,0,0,0,0,0,0,0,0};
    for (int i = 0; i < cantidad; i++){
        frecuencia[(ranking[i].hashtag_cant / decimal) % 10]++;
    }
    int posiciones[10] = {0,0,0,0,0,0,0,0,0,0};
    for (int i = 1; i < 10; i++){
        posiciones[i] = posiciones[i - 1] + frecuencia[i - 1];
    }
    for(int i = 0; i < cantidad; i++){
        resultado[ posiciones[(ranking[i].hashtag_cant / decimal) % 10] ] = ranking[i];
        posiciones[(ranking[i].hashtag_cant / decimal) % 10]++;
    }
    free(ranking);
    return resultado;
}

usuario_t* radix_sort_hashtags(usuario_t* ranking, size_t cantidad, size_t max){
    size_t decimal = 1;
    for (; max / decimal > 0; decimal *= 10){
        ranking = count_sort_hashtags(ranking, cantidad, decimal);
    }
    return ranking;
}

usuario_t* ordenar_usuarios(hash_t* usuarios){
    size_t cantidad = hash_cantidad(usuarios);
    usuario_t* ranking = (usuario_t*)calloc(cantidad+1,sizeof(usuario_t));
    hash_iter_t* iterador = hash_iter_crear(usuarios);
    const char* nombre = hash_iter_ver_actual(iterador);
    size_t max = 0;
    size_t cant_h = 0;
    for (size_t i = 0; i < cantidad; i++){
        ranking[i].nombre = strdup(nombre);
        cant_h =hash_cantidad(hash_obtener(usuarios, nombre));
        max = (max < cant_h) ? cant_h : max;
        ranking[i].hashtag_cant = cant_h;
        char nombre_arreglado[15] = {'/','/','/','/','/','/','/','/','/','/','/','/','/','/','/'};
        ranking[i].nombre_a = inicializar_nombre(nombre, nombre_arreglado);
        if(!hash_iter_avanzar(iterador)){
            break;
        }
        nombre = hash_iter_ver_actual(iterador);
    }
    hash_iter_destruir(iterador);
    ranking = radix_sort_nombres(ranking, cantidad);
    return radix_sort_hashtags(ranking, cantidad, max);
}

void imprimir_ranking(usuario_t* ranking, size_t largo){
    size_t cant_hashtags = ranking[0].hashtag_cant;
    for (size_t i = 0; i < largo; ){
        printf("%lu: ",cant_hashtags);
        while(cant_hashtags == ranking[i].hashtag_cant){
            printf("%s", ranking[i].nombre);
            if(cant_hashtags == ranking[i +1].hashtag_cant){
                printf(", ");
            }
            free(ranking[i].nombre);
            free(ranking[i].nombre_a);
            i++;
        }
        cant_hashtags = ranking[i].hashtag_cant;
        printf("\n");
    }
    free(ranking);
}

int main(int argc, char** argv){
    hash_t* usuarios = procesar_usuarios(argv[1]);
    usuario_t* ranking = ordenar_usuarios(usuarios);
    imprimir_ranking(ranking, hash_cantidad(usuarios));
    hash_destruir(usuarios);
    return 0;
}