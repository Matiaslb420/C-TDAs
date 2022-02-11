#include "pila.h"
#include "testing.h"
#include <stddef.h>
#include <stdio.h>

/*
Las pruebas deberán verificar que:

    Se pueda crear y destruir correctamente la estructura.
    Se puedan apilar elementos, que al desapilarlos se mantenga el invariante de pila.
    Prueba de volumen: Se pueden apilar muchos elementos (1000, 10000 elementos, o el volumen que corresponda): hacer crecer la pila hasta un valor sabido mucho mayor que el tamaño inicial, y desapilar elementos hasta que esté vacía, comprobando que siempre cumpla el invariante. Recordar no apilar siempre el mismo puntero, validar que se cumpla siempre que el tope de la pila sea el correcto paso a paso, y que el nuevo tope después de cada desapilar también sea el correcto.
    El apilamiento del elemento NULL es válido.
    Condición de borde: comprobar que al desapilar hasta que está vacía hace que la pila se comporte como recién creada.
    Condición de borde: las acciones de desapilar y ver_tope en una pila recién creada son inválidas.
    Condición de borde: la acción de esta_vacía en una pila recién creada es verdadero.
    Condición de borde: las acciones de desapilar y ver_tope en una pila a la que se le apiló y desapiló hasta estar vacía son inválidas.
*/

static void prueba_pila_vacia(void) {
    pila_t *pila = pila_crear();
    print_test("1 se crea bien", pila != NULL);
    print_test("7 esta vacia", pila_esta_vacia(pila));
    print_test("6 ver tope es invalido", pila_ver_tope(pila) == NULL);
    print_test("6 desapilar vacio es invalido", pila_desapilar(pila) == NULL);
    pila_destruir(pila);
    print_test("1 se destruye bien", true);
}

static void prueba_apilar_desapilar(void){
	pila_t *pila = pila_crear();
	int valor = 2;
	pila_apilar(pila, &valor);
	print_test("se apila bien", pila_ver_tope(pila) == &valor);
	pila_t *pila2 = pila_crear();
	print_test("se desapila bien", pila_desapilar(pila) != NULL);
	print_test("8 ver tope sigue siendo invalido", pila_ver_tope(pila) == NULL);
    print_test("8 desapilar vacio sigue siendo invalido", pila_desapilar(pila) == NULL);
	print_test("2 y 5 se mantiene invariable pila",  sizeof(pila) == sizeof(pila2) && pila_ver_tope(pila) == pila_ver_tope(pila2));
	pila_destruir(pila);
	pila_destruir(pila2);
	
}

static void prueba_pila_null(void){
	pila_t *pila = pila_crear();
	pila_apilar(pila,NULL);
	print_test("4 se guarda NULL", pila_ver_tope(pila) == NULL);
	pila_destruir(pila);
}

static void prueba_pila_muchos_elementos(void){
	pila_t *pila = pila_crear();
	int array[1000];
	for(int i = 0;i<1000;i++){
	array[i] = i;
	pila_apilar(pila,&array[i]);
	}
	print_test("ultimo elemento guardado", pila_ver_tope(pila) == &array[999]);
	print_test("penultimo elemento tambien", pila_desapilar(pila) == &array[999]);
	while(pila_desapilar(pila) != NULL){}
	print_test("borrado todo, tope", pila_ver_tope(pila) == NULL);
	pila_destruir(pila);
}
void pruebas_pila_estudiante() {
    prueba_pila_vacia();
    prueba_apilar_desapilar();
    prueba_pila_null();
    prueba_pila_muchos_elementos();
}


/*
 * Función main() que llama a la función de pruebas.
 */

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_pila_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif
