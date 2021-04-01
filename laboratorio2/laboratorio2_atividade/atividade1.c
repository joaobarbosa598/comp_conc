/*Laboratório 2 - Computacao Concorrente - Multiplicacao de duas matrizes quadradas pre-definidas
Nome: João Vitor de Freitas Barbosa
DRE: 117055449*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"
#include <unistd.h>

float **matrizA; //matriz de entrada
float **matrizB; //segunda matriz de entrada
float **matrizSaida;  //matriz de saida
int linhas, colunas;
int nThreads;

void * multiplicaMatrizMAtriz(void * arg);
int preencheMatriz(void);
void imprimeMatriz(FILE *arq);

int main(int argc, char *argv[]){

	FILE *arqSaida, *arqSaida2; //arquivo de dado para saida

	pthread_t *tid; //identificadores das threads no sistema
	int *id; //identificadores locais das threads
	double inicio, fim, delta1, delta2, delta3; //variaveis para medir o tempo de execucao

	//descobre quantos processadores a maquina possui
   	int numCPU = sysconf(_SC_NPROCESSORS_ONLN); 
   	printf("Numero de processadores: %d\n", numCPU);

   	//-------------------------------------------------------------//
   	GET_TIME(inicio);

   	if(argc < 3) {
   		fprintf(stderr, "Digite: %s <tamanho das matrizes quadradas> <numero de threads> <arquivo Matriz Saida>.\n", argv[0]);
   		return 1;
   	}

   	linhas = colunas = atoi(argv[1]);
   	nThreads = atoi(argv[2]);
   	if(nThreads>linhas){nThreads=linhas;}

   	if(preencheMatriz() == 0){
   	 	printf("Preenchimento das matrizes feito com sucesso\n");
   	}
   	else{
   		return 1;
   	}

   	//abre o arquivo da matriz de saida
	arqSaida = fopen(argv[3], "w");
	if(arqSaida == NULL) {
	  fprintf(stderr, "Erro ao abrir o arquivo da matriz de saida.\n");
	  return 1;
	}

	tid = (pthread_t *) malloc(sizeof(pthread_t)*nThreads);
	if(tid==NULL){puts("ERRO-malloc"); return 2;}

	//criacao das threads
	for(int i=0; i<nThreads; i++){
		id = (int*)malloc(sizeof(int));
		if(id==NULL){puts("ERRO:mallocID"); return 3;}
		*id = i;

		if(pthread_create(tid+i, NULL, multiplicaMatrizMAtriz, (void *)id)){
			puts("ERRO--pthread_create"); return 3;
		}
	}

	for(int i=0; i<nThreads; i++){
		if(pthread_join(*(tid+i), NULL)){
			puts("ERRO--pthread_join"); return 4;
		}
	}
	imprimeMatriz(arqSaida);

	free(matrizA);
	free(matrizB);
	free(matrizSaida);
}

void * multiplicaMatrizMAtriz(void * arg){
	int id = *(int *)arg;
	for(int i = id; i < linhas; i+=nThreads){
		for(int j = 0; j < colunas; j++){
			for(int k = 0; k < colunas; k++){
				matrizSaida[i][j] += matrizA[i][k] * matrizB[k][j];
				//printf("thread %d, i = %d, j = %d, k = %d\n", id, i, j, k);
			}
		}
	}

	free(arg);
   	pthread_exit(NULL);
}

int preencheMatriz(){
	matrizA = (float **) malloc(sizeof(float *) * linhas);
	matrizB = (float **) malloc(sizeof(float *) * linhas);
	matrizSaida = (float **) malloc(sizeof(float *) * linhas);
	if(!matrizA || !matrizB){
		puts("ERRO-malloc");
		return 1;
	}
	for(int i=0; i<linhas; i++){
		*(matrizA+i) = (float *) malloc(sizeof(float) * colunas);
		*(matrizB+i) = (float *) malloc(sizeof(float) * colunas);
		*(matrizSaida+i) = (float *) malloc(sizeof(float) * colunas);
		if(! *(matrizA+i) || ! *(matrizB+i) || ! *(matrizSaida+i)){
			puts("Erro-malloc2");
			return 1;
		}
	}
	for(int i=0; i<linhas; i++){
		for(int j=0; j<colunas; j++){
			*(*(matrizA+i) + j) = 3;
			*(*(matrizB+i) + j) = 2;
			*(*(matrizSaida+i)+j) = 0;
		}
	}

	return 0;
}

void imprimeMatriz(FILE *arq) {
   int i, j;
   for (i=0; i<linhas; i++) {
      for (j=0; j<colunas; j++) {
         fprintf(arq, "%.1f ", matrizSaida[i][j]);
      }
      fprintf(arq, "\n");
   }
}