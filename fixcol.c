void imprimir_linea(FILE* archivo){
	size_t pos = 0;
  	char linea[75];
  	char caracter = fgetc(archivo);
  		
  	while(eof(archivo) || caracter == '\n'){
  			
  		linea[pos] = caracter;
  		pos++;
  		caracter = fgetc(archivo);
  	}

  	printf("%s\n", linea);
}

void fixcol(char* nombre_archivo){

	FILE* archivo = fopen(nombre_archivo, "r");
  	if(!archivo){
    	printf("Error al abrir");
    	return 0;
  	}

  	while(eof(archivo)){
  		
  		imprimir_linea(archivo);	

  	}
}



int main(int argc, char* argv[]){
  if(argc != 2){
    printf("Necesita el nombre del archivo");
    return 0;
  }
  fixcol(argv[1]);
  return 0;
}