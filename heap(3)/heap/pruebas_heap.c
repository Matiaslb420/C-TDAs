#include <stdbool.h>
#include <stddef.h>
#include "heap.h"
#include "testing.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


int comparar(const void* a,const void * b){

    return strcmp((const char*)a,(const char*)b);
}

static void prueba_crear_heap_vacio(){
    heap_t* heap = heap_crear(comparar);
    print_test("Prueba crear heap vacio", heap);
    print_test("Prueba la cantidad de elementos es 0", heap_cantidad(heap) == 0);
    print_test("Prueba ver max es NULL, no existe", heap_ver_max(heap) == NULL);
    print_test("Prueba desencolar es NULL, no existe", heap_desencolar(heap) == NULL);
    heap_destruir(heap,NULL);
}

static void prueba_heap_insertar(){
    heap_t* heap = heap_crear(comparar);

    char* valor1 = "guau";
    char *valor2 = "miau";
    char *valor3 = "mu";

    /* Inserta 1 valor y luego lo borra */
    print_test("Prueba heap encolar valor1", heap_encolar(heap, valor1));
    print_test("Prueba heap la cantidad de elementos es 1", heap_cantidad(heap) == 1);
    print_test("Prueba heap desencolar valor1, es valor1", heap_desencolar(heap) == valor1);
    print_test("Prueba heap la cantidad de elementos es 0", heap_cantidad(heap) == 0);

    /* Inserta otros 2 valores y no los borra (se destruyen con el heap) */
    print_test("Prueba heap encolar clave2", heap_encolar(heap, valor2));
    print_test("Prueba heap la cantidad de elementos es 1", heap_cantidad(heap) == 1);
    print_test("Prueba heap ver max es valor2", heap_ver_max(heap) == valor2);

    print_test("Prueba heap encolar clave3", heap_encolar(heap, valor3));
    print_test("Prueba heap la cantidad de elementos es 2", heap_cantidad(heap) == 2);
    print_test("Prueba heap ver max es valor2", heap_ver_max(heap) == valor3);

    heap_destruir(heap,NULL);
}

static void prueba_heap_borrar(){
    heap_t* heap = heap_crear(comparar);

    char *valor1 = "guau";
    char *valor2 = "miau";
    char *valor3 = "mu";

    /* Inserta 3 valores y luego los borra */
    print_test("Prueba heap encolar valor1", heap_encolar(heap, valor1));
    print_test("Prueba heap encolar valor1", heap_encolar(heap, valor2));
    print_test("Prueba heap encolar valor1", heap_encolar(heap, valor3));

    /* Al borrar cada elemento comprueba que ya no está pero los otros sí. */
    print_test("Prueba heap ver max es valor3", heap_ver_max(heap) == valor3);
    print_test("Prueba heap desencolar valor1, es valor3", heap_desencolar(heap) == valor3);
    print_test("Prueba heap ver max no es valor3", !(heap_ver_max(heap) == valor3));
    print_test("Prueba heap desencolar es valor2", heap_desencolar(heap) == valor2);
    print_test("Prueba heap la cantidad de elementos es 1", heap_cantidad(heap) == 1);

    print_test("Prueba heap ver max es valor1", heap_ver_max(heap) == valor1);
    print_test("Prueba heap desencolar es valor1", heap_desencolar(heap) == valor1);
    print_test("Prueba heap desencolar es NULL", heap_desencolar(heap) == NULL);
    print_test("Prueba heap ver max es NULL", heap_ver_max(heap) == NULL);
    print_test("Prueba heap la cantidad de elementos es 0", heap_cantidad(heap) == 0);

    heap_destruir(heap,NULL);
}
static void prueba_heap_vacia(){
    heap_t* heap = heap_crear(comparar);

    char *valor = "";

    print_test("Prueba heap encolar valor1", heap_encolar(heap, valor));
    print_test("Prueba heap la cantidad de elementos es 1", heap_cantidad(heap) == 1);
    print_test("Prueba heap ver max es valor", heap_ver_max(heap) == valor);
    print_test("Prueba heap desencolar es valor", heap_desencolar(heap) == valor);
    print_test("Prueba heap la cantidad de elementos es 0", heap_cantidad(heap) == 0);

    heap_destruir(heap,NULL);
}

static void prueba_heap_volumen(size_t largo, bool debug)
{
    heap_t* heap = heap_crear(comparar);
    char (*claves)[10] = malloc(largo * 10);
    bool ok = true;
    for (unsigned i = 0; i < largo; i++) {
        sprintf(claves[i], "%08d", i);
        ok = heap_encolar(heap, claves[i]);
        if (!ok) break;
        ok = heap_ver_max(heap) == claves[i];
        if (!ok) break;
    }
    if (debug) print_test("Prueba heap almacenar muchos elementos", ok);
    if (debug) print_test("Prueba heap pertenece y obtener muchos elementos", ok);
    for (int i = ((int)largo) -1; i >= 0; i--) {
        ok = heap_desencolar(heap) == claves[i];
        if(!ok) break;
    }
    if (debug) print_test("Prueba heap la cantidad de elementos es 0", heap_cantidad(heap) == 0);

    heap_destruir(heap, NULL);

    free(claves);
}

static void prueba_heap_crear_arr(size_t largo){
    char (*claves)[10] = calloc(largo + 1, 10);
    *claves[largo] = '\0';
    bool ok = true;
    void** clave_p = calloc(largo + 1, sizeof(void*));
    clave_p[largo] = NULL;
    for (unsigned i = 0; i < largo; i++) {
        sprintf(claves[i], "%08d", i);
        clave_p[i] = claves[i];
    }
    heap_t* heap = heap_crear_arr(clave_p, largo, comparar);
    for (int i = ((int)largo) -1; i >= 0; i--) {
        char* aux = (char*)heap_desencolar(heap);
        ok = (strcmp(aux,claves[i]) == 0);
        if(!ok) break;
    }
    print_test("prueba heap desencolar muchos elementos", ok);
    print_test("Prueba heap la cantidad de elementos es 0", heap_cantidad(heap) == 0);

    heap_destruir(heap, NULL);
    free(clave_p);
    free(claves);
}

void pruebas_heap_estudiante(){
    prueba_crear_heap_vacio();
    prueba_heap_insertar();
    prueba_heap_borrar();
    prueba_heap_vacia();
    prueba_heap_volumen(5000, true);
    prueba_heap_crear_arr(5000);
}

#ifndef CORRECTOR
int main(void) {
    pruebas_heap_estudiante();
    return failure_count() > 0;
}
#endif
