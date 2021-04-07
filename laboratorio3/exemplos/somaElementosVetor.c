//Soma todos os elementos de um vetor de inteiro
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

long int dim; //dimensao do vetor de entrada
int nThreads; //numero de threads
double *vetor; //vetor de entrada com dimensao dim

void preencheVetor();
void * somaConcorrente(void * arg);

//fluxo principal
int main(int argc, char *argv[]){

	double somaSeq = 0; //soma sequencial
	double somaConc = 0;
	double inicio, fim, delta;
	pthread_t *tid; //identificadores das threads no sistema
	double *retorno; //valor de retorno das threads

	//recebe e valida os parametros de entrada (dimensao do vetor, numero de threads)
	if(argc < 3){
		fprintf(stderr, "Digite %s <dimensao do vetor> <numero de threads>\n", argv[0]);
		return 1;
	}

	dim = atoi(argv[1]);
	nThreads = atoi(argv[2]);

	//aloca o vetor de entrada
	vetor = (double*) malloc(sizeof(double)*dim);
	if(vetor == NULL){
		fprintf(stderr, "ERRO--malloc\n");
		return 2;
	}
	for(long int i=0; i < dim; i++){
		if(vetor+i==NULL){
			fprintf(stderr, "ERRO--malloc na pos %ld\n", i);
			return 3;
		}
	}

	//preenche o vetor de entrada
	preencheVetor();

	//soma sequencial dos elementos
	GET_TIME(inicio);
	for(long int i= 0; i<dim; i++){
		somaSeq += vetor[i];
	}
	GET_TIME(fim);
	delta = fim - inicio;
	printf("Tempo sequencial: %lf\n", delta);

	//soma concorrente dos elementos
	GET_TIME(inicio);
	tid = (pthread_t *)malloc(sizeof(pthread_t) * nThreads);
	if(tid == NULL){
		fprintf(stderr, "ERRO--malloc\n");
		return 2;
	}
	//criar as threads
	for(long int i=0; i<nThreads; i++){
		if(pthread_create(tid+i, NULL, somaConcorrente, (void *) i)){
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
	delta = fim - inicio;
	printf("Tempo concorrente: %lf\n", delta);

	//exibir os resultados
	printf("Soma sequencial: %.15lf\n", somaSeq);
	printf("Soma concorrente: %.15lf\n", somaConc);

	//libera as areas de memoria alocadas
	free(vetor);
	free(tid);
	return 0;
}

//fluxo das threads
void * somaConcorrente(void * arg){
	long int id = (long int) arg; //identificador da thread
	long int tamBloco = dim/nThreads; //tamanho do bloco de cada thread
	long int ini = id * tamBloco; //elemento inicial do bloco da thread
	long int fim; //elemento final(nao processado) do bloco da thread
	double *somaLocal; //variavel local da soma de elementos
	somaLocal = (double *)malloc(sizeof(double));
	if(somaLocal==NULL){
		fprintf(stderr, "ERRO--malloc\n");
		exit(1);
	}
	*somaLocal = 0;

	if(id == nThreads-1){
		fim = dim;
	}
	else {
		fim = ini+tamBloco;
	}

	for(long int i = ini; i<fim; i++){
		*somaLocal += vetor[i];
	}
	//retorna o resultado da soma local
	pthread_exit((void *) somaLocal);
}

void preencheVetor(){
	for(long int i=0; i<dim; i++){
		vetor[i] = 1000.1/(i+1);
	}
}