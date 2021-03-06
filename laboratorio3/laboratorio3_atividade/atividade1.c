#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"
#include <math.h>

int nThreads;
double numElementosSerie;

void * calculaPiConcorrente(void * arg);

int main (int argc, char *argv[]){

	double *retorno; //valor de retorno das threads
	pthread_t *tid; //identificadores das threads no sistema
	int *id;
	double somaConc = 0.0;
	double pi = 4.0;
	double inicio, fim, delta1, delta2; //variaveis para medir o tempo de execucao

	//-------------------------------------------------------------//
   	GET_TIME(inicio);

	if(argc<3){
		fprintf(stderr, "Digite: %s <numero de elementos da serie> <numero de threads>\n", argv[0]);
		return 1;
	}

	id = (int *)malloc(sizeof(int));
	numElementosSerie = atoll(argv[1]);
	nThreads = atoll(argv[2]);

	tid = (pthread_t *)malloc(sizeof(pthread_t) * nThreads);
	if(tid == NULL){
		fprintf(stderr, "ERRO--malloc\n");
		return 2;
	}

	GET_TIME(fim);

	//calcula o tempo gasto com as inicializacoes
	delta1 = fim - inicio;

	//-----------------------------------------------------------------------------------
   	GET_TIME(inicio);

	//criar as threads
	for(long int i=0; i<nThreads; i++){
		id = (int*)malloc(sizeof(int));
		if(id==NULL){puts("ERRO:mallocID"); return 3;}
		*id = i;
		if(pthread_create(tid+i, NULL, calculaPiConcorrente, (void *)id)){
			fprintf(stderr, "ERRO-pthread_create\n");
			return 3;
		}
	}

	//aguardar o termino das threads
	for(long int i=0; i<nThreads; i++){
		if(pthread_join(*(tid+i), (void **) &retorno)){
			fprintf(stderr, "ERRO--pthread_join\n");
			return 4;
		}
		//computar o valor final
		//soma concorrente
		somaConc += *retorno;

		free(retorno);
	}
	GET_TIME(fim);

	//calcula o tempo gasto com a parte concorrente (calculo de pi)
	delta2 = fim - inicio;

	free(tid);

	pi *= somaConc;
	printf("pi:      %.15lf\n", pi);

	printf("Real Pi: %.15lf\n", M_PI);
	printf("Tempo inicializacoes: %.8lf\n", delta1);
	printf("Tempo do calculo de pi com %d threads: %.8lf\n", nThreads, delta2);
}

void * calculaPiConcorrente(void * arg){

	int id = *(int *) arg;
	double num = 0.0;
	double aux = 0.0;
	double *numAux;
	numAux = (double*) malloc(sizeof(double));
	int i;
	
	for(i = id, aux = (double)id; i < numElementosSerie; i+=nThreads, aux+=nThreads){
		if( i%2 == 0 )num += 1/(2*aux+1);
		else num -= 1/(2*aux+1);
	}
	*numAux = num;
	free(arg);
	pthread_exit( (void *) numAux);
}