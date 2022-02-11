#include "lista.h"
#include "testing.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void destruir_dato(void* dato){
	free(dato);
}

static void prueba_lista_vacio(){
	lista_t* lista = lista_crear();
	print_test("1 se crea bien", lista != NULL);
    print_test("7 esta vacia", lista_esta_vacia(lista));
    print_test("6 ver primero es invalido", lista_ver_primero(lista) == NULL);
    print_test("6 borrar vacio es invalido", lista_borrar_primero(lista) == NULL);
	lista_destruir(lista, NULL);
	print_test("1 se destruye bien", true);
}

static void prueba_agregar_borrar(void){
	lista_t *lista = lista_crear();
	int valor = 2;
	lista_insertar_primero(lista, &valor);
	print_test("se enlista bien", lista_ver_primero(lista) == &valor);
	print_test("se desenlista bien", lista_borrar_primero(lista) != NULL);
	print_test("lista_esta_vacia sirve", lista_esta_vacia(lista));
	lista_t *lista2 = lista_crear();
	print_test("8 ver primero sigue siendo invalido", lista_ver_primero(lista) == NULL);
    print_test("8 desenlistar vacio sigue siendo invalido", lista_borrar_primero(lista) == NULL);
	print_test("2 y 5 se mantiene invariable lista",  sizeof(lista) == sizeof(lista2) && lista_ver_primero(lista) == lista_ver_primero(lista2));
	print_test("ver largo bien", lista_largo(lista) == 0);
	lista_destruir(lista, NULL);
	lista_destruir(lista2, NULL);
}

static void prueba_lista_null(void){
	lista_t *lista = lista_crear();
	void* nulo = NULL;
	lista_insertar_primero(lista, nulo);
	print_test("4 se guarda NULL", lista_ver_primero(lista) == NULL);
	lista_borrar_primero(lista);
	lista_destruir(lista, NULL);
}

static void prueba_lista_muchos_elementos(void) {
    size_t tam = 10000;
    lista_t* lista = lista_crear();
    

    /* Pruebo guardar en todas las posiciones */
    size_t i;
    bool ok = true;
    for (i = 0; i < tam; i++) {
        // Si algun elemento no se pudo guardar correctamente, ok sera false
        ok &= lista_insertar_primero(lista, &i);
    }
    print_test("largo de lista es el indicado", lista_largo(lista) == tam);
    print_test("se pudieron guardar todos los elementos", ok);
    
	for (i=0; i < tam; i++){
	if (lista_borrar_primero(lista) != &i){
		break;
		}
	}
	print_test("se borra bien", lista_largo(lista) == 0);
    
   

    /* Destruyo el vector*/
   	lista_destruir(lista,NULL);
}

static void prueba_iteradores(void){
	lista_t* super = lista_crear();
	lista_iter_t* iter = lista_iter_crear(super);
	int dato = 4;
	int dato2 = 5;
	int dato3 = 2;
		  //pointer
	void* ptr = &dato;		//1
	void* ptr2 = &dato2;	//2
	void* ptr3 = &dato3;	//3
	lista_iter_insertar(iter, ptr);   //[1]
	lista_iter_insertar(iter, ptr2);	//[2,1]
	print_test("prueba iter 1: se crea realmente al inicio", lista_iter_al_final(iter) == false);
	while (lista_iter_al_final(iter) != true){
		lista_iter_avanzar(iter);
	}
	
	lista_iter_insertar(iter, ptr3); //[2,1,3]
	lista_iter_avanzar(iter);
	print_test("prueba iter 2: se crea también al final", lista_iter_al_final(iter) == true);
	lista_iter_destruir(iter);
	//3 insertar elemento en el medio
	lista_iter_t* iter2 = lista_iter_crear(super);
	lista_iter_avanzar(iter2);
	lista_iter_insertar(iter2,ptr3); //[2,3,1,3]
	void* dato_insertado = lista_iter_ver_actual(iter2);
	lista_iter_avanzar(iter2);
	lista_iter_avanzar(iter2);
	print_test("prueba iter 3: se inserta en la posición correcta", lista_iter_ver_actual(iter2) == dato_insertado);
	lista_iter_destruir(iter2);
	lista_iter_t* iter3 = lista_iter_crear(super);
	lista_iter_borrar(iter3);	//[3,1,3]
	print_test("prueba iter 4: se cambia el inicial ",lista_ver_primero(super) == ptr3);
	lista_iter_avanzar(iter3);
	lista_iter_avanzar(iter3);
	lista_iter_borrar(iter3);	//[3,1]
	lista_iter_destruir(iter3);
	print_test("prueba iter 5: se cambia el final ", lista_ver_ultimo(super) == ptr);
	lista_iter_t* iter4 =lista_iter_crear(super);
	lista_iter_insertar(iter4, ptr2);
	lista_iter_insertar(iter4,ptr); //[1,2,3,1]
	lista_iter_avanzar(iter4);
	lista_iter_borrar(iter4); //[1,3,1]
	print_test("prueba iter 6: se borra elemento del final", lista_iter_ver_actual(iter4) == ptr3);
	lista_iter_destruir(iter4);
	lista_destruir(super, NULL);
}

bool visitar_prueba(void* dato, void* suma){
	*(int*)suma += *(int*)dato;
	return true;
}
static void prueba_iter_interno(void){
	lista_t* lista = lista_crear();
	int dato = 1;
	int dato2 = 2;
	int dato3 = 3;
	void* ptr = &dato;
	void* ptr2 = &dato2;
	void* ptr3 = &dato3;
	lista_insertar_primero(lista,ptr);
	lista_insertar_primero(lista,ptr2);
	lista_insertar_primero(lista,ptr3);
	int suma = 0;
	void* suma_p = &suma;
	lista_iterar(lista,visitar_prueba, suma_p);
	lista_destruir(lista, NULL);
	print_test("el iterador interno funciona", suma == 6);
}

void pruebas_lista_estudiante(){
	prueba_lista_vacio();
	prueba_agregar_borrar();
	prueba_lista_null();
	prueba_lista_muchos_elementos();
	prueba_iteradores();
	prueba_iter_interno();
}

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
	pruebas_lista_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif
