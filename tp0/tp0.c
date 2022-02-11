#include "tp0.h"

/* *****************************************************************
 *                     FUNCIONES A COMPLETAR                       *
 *         (ver en tp0.h la documentación de cada función)         *
 * *****************************************************************/

void swap(int *x, int *y) {
    int variable_temporal = *x;
    *x = *y;
    *y = variable_temporal;
}


int maximo(int vector[], int n) {
    if (n != 0){
        int maximo = 0;
        for (int indice = 1; indice < n; indice++){
            if (vector[maximo] < vector[indice]){
                maximo = indice;
            }
        }
        return maximo;
    }
    return -1;
}

int comparar(int vector1[], int n1, int vector2[], int n2) {
    int index = 0;
    while (index < n1 && index < n2){
        if (vector1[index]< vector2[index]){
            return -1;
        }
        else if(vector1[index] > vector2[index]){
            return 1;
        }
        index++;
    }
    if (n1 < n2){
        return -1;
    }
    else if(n1> n2){
        return 1;
    }
    return 0;
}

void seleccion(int vector[], int n){
for (int i = 0; i < n-1; i++){
	swap(&vector[maximo(vector, n-i)], &vector[n-i-1]);
 	}
}
