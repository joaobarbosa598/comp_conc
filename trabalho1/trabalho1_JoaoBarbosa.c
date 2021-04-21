#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "timer.h"

int nThreads;
long int N;
double a, b;

double funcao(double x);
void * integral(void * arg);

//fluxo principal
int main(int argc, char *argv[]){

	double somaConc = 0;
	pthread_t *tid; //identificadores das threads no sistema
	int *id;
	double *retorno; //valor de retorno das threads
	double inicio, fim, delta1, delta2; //variaveis para medir o tempo de execucao

	//-------------------------------------------------------------//
   	GET_TIME(inicio);

	//recebe e valida os parametros de entrada (dimensao do vetor, numero de threads)
	if(argc < 5){
		fprintf(stderr, "Digite %s <a> <b> <quantos calculos serao feitos N> <numero de threads>\n", argv[0]);
		return 1;
	}

	a = atof(argv[1]);
	b = atof(argv[2]);
	N = atol(argv[3]);
	nThreads = atoi(argv[4]);

	if(a > b){
		int i = a;
		a = b;
		b = i;
	}
	if(nThreads>N){
		nThreads=N;
	}

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
		if(pthread_create(tid+i, NULL, integral, (void *)id)){
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

	//calcula o tempo gasto com a parte concorrente (calculo da integral)
	delta2 = fim - inicio;

	printf("O valor da integração é: %.15lf\n", somaConc);
	
	//exibe os tempos gastos em cada parte do programa 
    printf("Tempo inicializacoes: %.8lf\n", delta1);
    printf("Tempo do calculo da integral: %.8lf\n", delta2);

	free(tid);
	return 0;
}

void * integral(void * arg){

	int id = *(int *) arg;
	double x, y;
	double *somaLocal;
	somaLocal = (double *)malloc(sizeof(double));
	if(somaLocal==NULL){
		fprintf(stderr, "ERRO--malloc\n");
		exit(1);
	}
	*somaLocal=0;
	for(double i = a+ ((b-a)/N)*id; i < b; i+= ((b-a)/N)*nThreads ){
		x = i;
		y = funcao(x);
		*somaLocal += y *  (b - a)/N;
	}

	//retorna o resultado da soma local
	pthread_exit((void *) somaLocal);
}

double funcao(double x){
	//return exp(1-(x*x));
	return cos(5*x);
}