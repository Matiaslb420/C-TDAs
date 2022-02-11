#include "cola.h"
#include "testing.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void prueba_cola_vacio(){
	cola_t* cola = cola_crear();
	print_test("1 se crea bien", cola != NULL);
    print_test("7 esta vacia", cola_esta_vacia(cola));
    print_test("6 ver primero es invalido", cola_ver_primero(cola) == NULL);
    print_test("6 desencolar vacio es invalido", cola_desencolar(cola) == NULL);
	cola_destruir(cola, NULL);
	print_test("1 se destruye bien", true);
}

static void prueba_encolar_desencolar(void){
	cola_t *cola = cola_crear();
	int valor = 2;
	cola_encolar(cola, &valor);
	print_test("se encola bien", cola_ver_primero(cola) == &valor);
	print_test("se desencola bien", cola_desencolar(cola) != NULL);
	print_test("cola_esta_vacia sirve", cola_esta_vacia(cola));
	cola_t *cola2 = cola_crear();
	print_test("8 ver primero sigue siendo invalido", cola_ver_primero(cola) == NULL);
    print_test("8 desencolar vacio sigue siendo invalido", cola_desencolar(cola) == NULL);
	print_test("2 y 5 se mantiene invariable cola",  sizeof(cola) == sizeof(cola2) && cola_ver_primero(cola) == cola_ver_primero(cola2));
	cola_destruir(cola, NULL);
	cola_destruir(cola2, NULL);
	//agregado ****************************
	cola_t* cola3 = cola_crear();
	int dato = 4;
	int dato2 = 8;
	cola_encolar(cola3, &dato);
	cola_encolar(cola3, &dato2);
	print_test("cola != pila", cola_ver_primero(cola3) == &dato);
	cola_desencolar(cola3);
	print_test("efectivamente cola != pila", cola_ver_primero(cola3) == &dato2);
	cola_destruir(cola3, NULL);
}

static void prueba_cola_null(void){
	cola_t *cola = cola_crear();
	cola_encolar(cola,NULL);
	print_test("4 se guarda NULL", cola_ver_primero(cola) == NULL);
	cola_destruir(cola, NULL);
}

static void pruebas_cola_volumen(void) {
    printf("INICIO DE PRUEBAS DE VOLUMEN\n");

    size_t tam = 10000;

    cola_t* cola = cola_crear();


    /* Pruebo guardar en todas las posiciones */
    size_t i;
    bool ok = true;
    for (i = 0; i < tam; i++) {
        // Si algun elemento no se pudo guardar correctamente, ok sera false
        ok &= cola_encolar(cola,&i);
    }
    print_test("se pudieron guardar todos los elementos", ok);

    /* Pruebo que lo guardado sea correcto */
    ok = true;
    void* valor = NULL;
    for (i = 0; i < tam; i++) {
		valor = cola_desencolar(cola);
        ok &= valor != NULL;
        ok &= ((size_t*)valor == &i );
    }
    print_test("se pudieron obtener todos los elementos", ok);
    print_test("no se puede obtener el ultimo elemento en cola vacia", cola_ver_primero(cola) == NULL);

	cola_destruir(cola, NULL);
}


void pruebas_cola_estudiante(){
	prueba_cola_vacio();
	prueba_encolar_desencolar();
	prueba_cola_null();
	pruebas_cola_volumen();
}

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
	pruebas_cola_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif
