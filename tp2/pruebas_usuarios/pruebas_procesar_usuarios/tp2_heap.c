#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "tp2_heap.h"
#include <stdio.h>

static const size_t TAM_INICIAL = 16;

static const float AGRANDAR = 2.0;
static const float ACHICAR  = 0.5;

struct heap {
    void ** arreglo;
    size_t cant_elem;
    cmp_func_t comparar; 
    size_t tam_arreglo;
};
void heap_down_heap(void ** arreglo, size_t cant_elem, size_t posicion, cmp_func_t comparar);

//////////////////////////////////////

bool heapify(void* arreglo[], size_t cant_elem, cmp_func_t cmp){
    if (cant_elem == 0)return false;
    for (int i = ((int)cant_elem)-1; i >= 0; i--){
        heap_down_heap(arreglo,cant_elem,(size_t)i, cmp);
    }
    return true;
}

heap_t *heap_crear(cmp_func_t cmp){
    heap_t * heap = malloc(sizeof(heap_t));
    if (!heap) return NULL;
    heap->arreglo = malloc(TAM_INICIAL * sizeof(void*));
    if (!heap->arreglo) return NULL;
    heap->cant_elem = 0;
    heap->comparar = cmp;
    heap->tam_arreglo = TAM_INICIAL;
    return heap;
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp){
    heap_t * heap = malloc(sizeof(heap_t));
    void** arreglo_nuevo = malloc(n * sizeof(void*));
    for(int i = 0 ; i < (int)n ; i++){
        arreglo_nuevo[i] = arreglo[i];
    }
    if (!heap) return NULL;
    if (!arreglo_nuevo) return NULL;
    size_t cant_elem = 0;
    for(int i = 0 ; i < (int)n ; i++){
        if(arreglo[i] != NULL)cant_elem++;
    }
    heap->cant_elem = cant_elem;
    heapify(arreglo_nuevo,n ,cmp);
    heap->arreglo = arreglo_nuevo;
    heap->comparar = cmp;
    heap->tam_arreglo = n;
    return heap;
}
void heap_destruir(heap_t *heap, void (*destruir_elemento)(void *e)){
    if(destruir_elemento != NULL){
        for(size_t i = 0;heap->cant_elem != 0;i++){
            destruir_elemento(heap->arreglo[i]);
            heap->cant_elem--;
        }
    }
    free(heap->arreglo);
    free(heap);
}

size_t heap_cantidad(const heap_t *heap){
    return heap->cant_elem;
}

bool heap_esta_vacio(const heap_t *heap){
    return (heap_cantidad(heap) == 0);
}

void heap_up_heap(heap_t* heap,size_t posicion){
    if(posicion == 0) return;
    size_t padre = (posicion - 1) / 2;
    if(heap->comparar(heap->arreglo[posicion],heap->arreglo[padre]) > 0){
        void * guardar = heap->arreglo[padre];
        heap->arreglo[padre] = heap->arreglo[posicion];
        heap->arreglo[posicion] = guardar;
        heap_up_heap(heap,padre);
    }
}

bool heap_redimensionar(heap_t* heap, float operacion){
    heap->tam_arreglo = (size_t)((float)heap->tam_arreglo * operacion);
    void** arreglo_nuevo = realloc(heap->arreglo,sizeof(void*) * heap->tam_arreglo);
    if (arreglo_nuevo == NULL) return false;
    heap->arreglo = arreglo_nuevo;
    return true;
}

bool heap_encolar(heap_t *heap, void *elem){
    if(heap->cant_elem * 2 >= heap->tam_arreglo){
        if (!heap_redimensionar(heap, AGRANDAR)) return false;
    }
    heap->arreglo[heap->cant_elem] = elem;
    heap_up_heap(heap,heap->cant_elem);
    heap->cant_elem++;
    return true;
}

void *heap_ver_max(const heap_t *heap){
    if (heap_esta_vacio(heap)){
        return NULL;
    }
    return heap->arreglo[0];
}

void heap_down_heap(void ** arreglo, size_t cant_elem, size_t posicion, cmp_func_t comparar){
    size_t hijo_izq = 2*posicion + 1;
    size_t hijo_der = 2*posicion + 2;
    if(hijo_izq >= cant_elem)return;
    size_t pos_max = hijo_izq; 
    if(hijo_der < cant_elem){
        int cmp = comparar(arreglo[hijo_izq],arreglo[hijo_der]);
        if(cmp < 0){
            pos_max = hijo_der;
        }
    }
    if(comparar(arreglo[posicion],arreglo[pos_max]) < 0){
        void * padre = arreglo[posicion];
        arreglo[posicion] = arreglo[pos_max];
        arreglo[pos_max] = padre;
        heap_down_heap(arreglo, cant_elem, pos_max, comparar);
    }
}

void *heap_desencolar(heap_t *heap){
    if(heap_esta_vacio(heap))return NULL;
    if(heap->cant_elem * 4 <= heap->tam_arreglo){
        if (!heap_redimensionar(heap, ACHICAR)) return false;
    }
    void* resultado = heap->arreglo[0];
    heap->arreglo[0] = heap->arreglo[heap->cant_elem - 1];
    heap_down_heap(heap->arreglo, heap->cant_elem, 0, heap->comparar);
    heap->cant_elem--;
    return resultado;
}

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp){
    if (!heapify(elementos, cant, cmp)) return;
    void* puntero = NULL;
    for(int i = ((int)cant)-1;i >= 0;i--){
        puntero = elementos[0];
        elementos[0] = elementos[i];
        elementos[i] = puntero;
        heap_down_heap(elementos, (size_t)i , 0, cmp);
    }
}
//(i-1)/2

//2i + 1 isq
//2i + 2 dsr