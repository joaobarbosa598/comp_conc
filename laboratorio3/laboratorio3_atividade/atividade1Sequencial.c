#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double calculaPi(long int numElementosSerie);

int main (int argc, char *argv[]){

	double result;
	long int numElementosSerie;
	if(argc < 2){
		fprintf(stderr, "Digite: %s <numero de elementos da serie>\n", argv[0]);
		return 1;
	}

	numElementosSerie = atoll(argv[1]);
	result = calculaPi(numElementosSerie);
	printf("Pi: %.12f\n", result);
	printf("Pi: %.12lf\n", M_PI);
	return 0;
}

double calculaPi(long int numElementosSerie){

	double pi = 4.0;
	double aux = 1.0;

	for(long int i=0, aux2=3, aux3=1; i<numElementosSerie; i++, aux2+=2, aux3*=-1){
		
		aux = aux - (1.0/aux2)*aux3;
	}
	pi *= aux;
	return pi;
}