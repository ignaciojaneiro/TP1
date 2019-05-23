#include "stddef.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


char *substr(const char *str, size_t n){
	char* copia = malloc(sizeof(char)*(n+1));

	if(!copia) return NULL;

	if(strlen(str) >= n){
		strncpy(copia,str,n);
		copia[n] = '\0';
	}else{
		strncpy(copia,str,strlen(str));
		copia[n] = '\0';
	}
	return copia;
}

char **split(const char *str, char sep){
	size_t apariciones = 0;
	char **strv;
	size_t inicio = 0;

	for(size_t i = 0; str[i]; i++){
		if(str[i] == sep) apariciones++;	
	}

	strv = malloc(sizeof(char*) * (apariciones + 2));
	if(!strv)return NULL;

	apariciones = 0;

	for(size_t i = 0; i <= strlen(str); i++){
		
		if(str[i] == sep || strlen(str) == i){
			strv[apariciones] = substr(str + inicio, i - inicio);
			inicio = i+1;
			apariciones++; 
		}
	}

	strv[apariciones] = NULL;

	return strv;
}

size_t cant_posiciones(char **strv){
	size_t pos = 0;

	while(strv[pos]){
		pos++;
	}
	return pos;
}


char *join(char **strv, char sep){

	size_t cant_pos = cant_posiciones(strv);
	size_t cant_caracteres = 0;

	for(int i = 0; i < (cant_pos-1); i++){
		cant_caracteres += sizeof(strv[i]) + 1;
	}

	char* nuevo_str = malloc(sizeof(char) * (cant_caracteres+1));

	size_t pos = 0;
	size_t cant = 0;

	for(size_t i = 0; i < (cant_pos);i++){
		char* str_act = strv[i];
		cant = strlen(str_act);
		strncpy(nuevo_str + pos,strv[i],cant);
		pos += cant;
		nuevo_str[pos] = sep;
		pos++;
	}
	pos--;
	nuevo_str[pos] = '\0';

	return nuevo_str;
}

void free_strv(char **strv){

	size_t pos = 0;

	while(strv[pos]){
		free(strv[pos]);
		pos++;
	}

	free(strv);
}