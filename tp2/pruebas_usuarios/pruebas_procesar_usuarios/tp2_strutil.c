#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include "tp2_strutil.h"
#include <string.h>
#include <stddef.h>
#include <stdlib.h>


char *substr(const char *str, size_t n){
	char* frase = NULL;
	if (n > 0){
		frase = strndup(str, n);
	}
	else {
		frase = malloc(sizeof(char) * 4);
		strcpy(frase, "\0");
	}
	return frase;
 }

unsigned long contar_char(const char* str, char caracter, size_t largo){
	unsigned long count = 0;
	for (int i = 0;i < largo;i++){
		if (str[i] == caracter){
			count++;
		}
	}
	return count;
}

char** split(const char *str, char sep){
	size_t largo = strlen(str);
	unsigned long ocurrencias = contar_char(str,sep,largo);
	char** arreglo = malloc(sizeof(char*) * (ocurrencias+2)); //SCREAMING
	char* parte = NULL;
	if (ocurrencias == 0){
		parte = strdup(str);
		arreglo[0] = parte;
		arreglo[1] = NULL;
		return arreglo;
	}
	char* str_aux = strdup(str);
	char* direccion_inicial = str_aux;
	size_t i_str = 0;
	size_t i_arr = 0;
	for(;ocurrencias >0; ){
		if (str_aux[i_str] == sep){
			ocurrencias--;
			parte = substr(str_aux, i_str);
			str_aux += i_str + 1;
			largo = strlen(str_aux);
			arreglo[i_arr] = parte;
			i_arr++;
			parte = NULL;
			i_str = 0;
			continue;
		}
		i_str++;
	}
	parte = strdup(str_aux);
	arreglo[i_arr] = parte;
	arreglo[i_arr+1] = NULL;
	free(direccion_inicial);
	return arreglo;
}

char *join(char **strv, char sep){
	size_t caracteres = 0;
	size_t largo_arreglo = 0;
	for(; strv[largo_arreglo]; largo_arreglo++){
		caracteres += strlen(strv[largo_arreglo]);
	}
	char* cadena;
	if (largo_arreglo == 0){
		cadena = strndup("\0", 1);
		return cadena;
	}
	if (largo_arreglo == 1){
		cadena = strdup(strv[0]);
		return cadena;
	}
	size_t maximo = caracteres + largo_arreglo + 1;
	cadena = (char*)malloc(maximo);
	size_t cadena_index = 0;
	for (int i = 0;strv[i]; i++){
		cadena_index += (size_t)snprintf(cadena + cadena_index, maximo - cadena_index ,"%s%c", strv[i],sep);
		if (sep == '\0'){
			cadena_index--;
		}
	}
	if (sep == '\0'){
			cadena_index++;
		}
	cadena[cadena_index - 1] = '\0';
	return cadena;
}

void free_strv(char *strv[]){
	for(int i = 0; strv[i]; i++){

		free(strv[i]);
	}
	free(strv);
}

/*

int main(){
	
	//char* str= "Algoritmos";

	//char* sub = substr(str, 30);
	//printf("%s\n", sub);
	//free(sub);
	//char o = 'o';
	char** esplit = split("abcdefg,hijklmn,opqrstu,vwxyz", ',');
	
	//for (int i =0; esplit[i];i++){
	//	printf("%s\n",esplit[i]);
	//}
	char* cadena = join(esplit, '\0');
	printf("%s\n", cadena);
	free(cadena);
	
	free_strv(esplit);
	
	return 0;
}
*/