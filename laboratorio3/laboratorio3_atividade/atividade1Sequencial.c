#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "timer.h"
#include <pthread.h>

double calculaPi(long int numElementosSerie);

int main (int argc, char *argv[]){

	double result;
	long int numElementosSerie;
	double inicio, fim, delta1; //variaveis para medir o tempo de execucao

	//-------------------------------------------------------------//
   	GET_TIME(inicio);

	if(argc < 2){
		fprintf(stderr, "Digite: %s <numero de elementos da serie>\n", argv[0]);
		return 1;
	}

	numElementosSerie = atoll(argv[1]);
	result = calculaPi(numElementosSerie);

	GET_TIME(fim);

	//calcula o tempo gasto para o cálculo de pi
	delta1 = fim - inicio;

	printf("Pi: %.15f\n", result);
	printf("Pi: %.15lf\n", M_PI);
	printf("Tempo do calculo de pi sequencial: %.8lf\n", delta1);
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