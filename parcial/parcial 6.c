
//EJERCICIO 6
/*Implementar la primitiva pila_t* pila_merge (pila_t* pila, pila_t* otra) para el
TDA Pila, que permite insertar ordenadamente los elementos de la segunda pila en la primera,
tomando como precondición que ambas pilas se encuentran inicialmente ordenadas. Detallar
y justificar la complejidad del algoritmo considerando que el tamaño de las colas es N y M
respectivamente.*/

// [1,3,5,7] pila
// [2,4,6,8] otra
//considerando que están ordenadas de forma que el tope es el menor
#include <stdio.h>
#include <stdlib.h>

//crea una pila auxiliar, y va insertando los valores en orden
pila_t* pila_merge (pila_t* pila, pila_t* otra){
	pila_t* pila_aux = pila_crear();
	while (pila_ver_tope(pila) != NULL || pila_ver_tope(otra) != NULL){
		void* tope;
		if (pila_ver_tope(pila) < pila_ver_tope(otra)){
			tope = pila_desapilar(pila);
		}
		else {
			tope = pila_desapilar(otra);
		}
		pila_apilar(pila_aux, tope);
	}
	while (pila_ver_tope(pila_aux)){
		pila_apilar(pila, pila_desapilar(pila_aux));
	}
	pila_destruir(pila_aux);
	return pila;
}

//La complejidad en este caso sería O(2N + 2M), esto debido a que itera ambas pilas, para apilarlas en la pila auxiliar
//Luego reapila cada elemento en la pila inicial, haciendo que al final, termina iterando ambas pilas 2 veces.

//EJERCICIO 9
/* Implementar una función lista_t* top_3(lista_t* lista) que reciba una lista enlazada
de enteros y devuelva una lista enlazada nueva con los 3 enteros más grandes. La lista
original no debe ser modificada. Detallar y justificar la complejidad del algoritmo.*/

int min_dato(void* dato1,void* dato2,void* dato3){
	void* minimo;
	if (dato1 <dato2 && dato1< dato3){
		minimo = dato1;
		}
	else if (dato2 <dato1 && dato2< dato3){
		minimo = dato2;
		}
	else{
		minimo = dato3;
		}
	return minimo
}

lista_t* top_3(lista_t* lista){
	void* = top_1;
	void* = top_2;
	void* = top_3;
	
	lista_t* lista_top = lista_crear();
	lista_iter_t* iter = lista_iter_crear(lista);
	//reconozco que esto es raro pero estoy nervioso
	for (i = 0; i < 3; i++){
		if (i== 0){
			top_1 = lista_iter_ver_actual(iter);
			}
		else if (i == 1){
			top_2 = lista_iter_ver_actual(iter);
			}
		else{
			top_3 = lista_iter_ver_actual(iter);
			}
		lista_iter_avanzar(iter);
	}
	
	while ( !lista_iter_al_final(iter)){
		void* dato = lista_iter_ver_actual(iter);
		void* minimo = min_dato(top_1, top_2, top_3);
		if (dato > minimo){
			//codigo que si es mayor, reemplaza la menor variable de los top 3 por ese nuevo dato
		}
		
		lista_iter_avanzar(iter);
	}
	lista_iter_destruir(iter);
	lista_insertar_ultimo(lista_top, top_1);
	lista_insertar_ultimo(lista_top, top_2);
	lista_insertar_ultimo(lista_top, top_3);
	return lista_top;
	
}

//toda la lista debe iterarse al menos una vez, pero hay que comparar cada dato con los actualmente guardados como mayores (o no, si los primeros 3 valores son los 3 mayores de la lista)
//así que es O(N) minimo, u O(3N) en el peor de los casos que la lista esté ordenada de menor a mayor
















