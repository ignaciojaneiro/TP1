#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define CAPACIDAD 100

typedef struct vectorbf vectorbf_t;

void instrucciones_basicas(char caracter, unsigned char** puntero){
	if(caracter == '+')(*(*puntero))++;
    if(caracter == '-')(*(*puntero))--;
    if(caracter == '>')(*puntero)++;
    if(caracter == '<')(*puntero)--;
    if(caracter == '.'){
      putchar((*(*puntero)));
    }
    if(caracter == ',')(*(*puntero)) = (char)getchar();
}


struct vectorbf{
  unsigned char* vector;
  size_t cantidad;
  size_t capacidad;
  size_t balanceados;
};

vectorbf_t* vectorbf_crear(){
  vectorbf_t* vectorbf = malloc(sizeof(vectorbf_t));
  vectorbf->vector = malloc(sizeof(unsigned char)*CAPACIDAD);
  vectorbf->cantidad = 0;
  vectorbf->capacidad = CAPACIDAD;
  vectorbf->balanceados = 0;
  return vectorbf;
}

bool vectorbf_redimencionar(vectorbf_t* vectorbf){
  unsigned char* nuevo_vector = realloc(vectorbf->vector,vectorbf->capacidad * 2);
  if(!nuevo_vector)return false;
  vectorbf->capacidad*=2;
  vectorbf->vector=nuevo_vector;
  return true;
}

bool vectorbf_balanceado(vectorbf_t* vectorbf){
  return !vectorbf->balanceados;
}

bool vectorbf_agregar(vectorbf_t* vectorbf, char caracter){
  if(vectorbf->cantidad == vectorbf->capacidad){
    vectorbf_redimencionar(vectorbf);
  }
  vectorbf->vector[vectorbf->cantidad] = caracter;
  vectorbf->cantidad++;
  if(caracter == '['){
    vectorbf->balanceados++;
  }else if(caracter == ']'){
    vectorbf->balanceados--;
  }
  return true;
}

size_t vectorbf_findelloop(vectorbf_t* vectorbf, size_t pos){

	if(pos == 0)return (vectorbf->cantidad-1);

	pos++;
	while(true){
		if(vectorbf->vector[pos] == '[')vectorbf->balanceados++;
		if(vectorbf->vector[pos] == ']'){
			if(vectorbf->balanceados == 0){
				return ++pos;
			}
			vectorbf->balanceados--;
		}
		pos++;
	}

	return ++pos;
}

size_t vectorbf_recorrerloops(vectorbf_t* vectorbf, size_t pos, unsigned char** puntero){

  size_t pos_act = pos;

  while(true){
  		
      if(vectorbf->vector[pos_act] == '['){
      	if((*(*puntero)) == 0){
      	  pos_act = vectorbf_findelloop(vectorbf, pos_act);
      	}else if(pos_act != 0){
          pos_act = vectorbf_recorrerloops(vectorbf, pos_act + 1,puntero);
      	}
      }
      
      if(vectorbf->vector[pos_act] == ']'){
          if((*(*puntero)) == 0){
            return ++pos_act;
          }else{
            pos_act = pos;
          }
      }

      instrucciones_basicas(vectorbf->vector[pos_act],puntero);
      
      pos_act++;
  }

  return (pos_act++);


}

void vectorbf_destruir(vectorbf_t* vectorbf){
  free(vectorbf->vector);
  free(vectorbf);
}

bool caracter_valido(char caracter){
	char* str = "<>+-.,[]";
	for(size_t i = 0; str[i]; i++){
		if(caracter == str[i])return true;
	}
	return false;
}

int proximo_caracter(FILE* archivo){
	int nuevo_caracter = (char)fgetc(archivo);
	while(nuevo_caracter != -1 && !caracter_valido((char)nuevo_caracter)){
		nuevo_caracter = (char)fgetc(archivo);
	}
	return nuevo_caracter;
}

int interprete_bf(char* nombre_archivo){

  FILE* archivo = fopen(nombre_archivo, "r");
  if(!archivo){
    printf("Error al abrir el archivo");
    return 0;
  }

  unsigned char vector[50];
  for(size_t pos = 0; pos < 50; pos++){
    vector[pos] = 0;
  }

  unsigned char* puntero = &vector[0];
  unsigned char** p = &puntero;
  int caracter = proximo_caracter(archivo);
  while(caracter != -1){
    if(caracter == '['){
      vectorbf_t* vectorbf = vectorbf_crear();
      do{
        vectorbf_agregar(vectorbf,(char)caracter);
        caracter = proximo_caracter(archivo);
      }while(caracter != ']');
      do{
        vectorbf_agregar(vectorbf,(char)caracter);
        caracter = proximo_caracter(archivo);
      }while(!vectorbf_balanceado(vectorbf));
      vectorbf_recorrerloops(vectorbf,0,p);
      vectorbf_destruir(vectorbf);
    }

	instrucciones_basicas((char)caracter,p);
    caracter = proximo_caracter(archivo);

  }


  return 1;
}



int main(int argc, char* argv[]){
  if(argc != 2){
    printf("El interprete necesita el nombre del archivo");
    return 0;
  }
  interprete_bf(argv[1]);
  return 0;
}