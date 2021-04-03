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

void multiplicaMatrizMAtriz();
int preencheMatriz(void);
void imprimeMatriz(FILE *arq);

int main(int argc, char *argv[]){

	FILE *arqSaida, *arqSaida2; //arquivo de dado para saida

	double inicio, fim, delta1, delta2, delta3; //variaveis para medir o tempo de execucao

	//descobre quantos processadores a maquina possui
   	int numCPU = sysconf(_SC_NPROCESSORS_ONLN); 
   	printf("Numero de processadores: %d\n", numCPU);

   	//-------------------------------------------------------------//
   	GET_TIME(inicio);

   	if(argc < 2) {
   		fprintf(stderr, "Digite: %s <tamanho das matrizes quadradas> <arquivo Matriz Saida>.\n", argv[0]);
   		return 1;
   	}

   	linhas = colunas = atoi(argv[1]);

   	if(preencheMatriz() == 0){
   	 	printf("Preenchimento das matrizes feito com sucesso\n");
   	}
   	else{
   		return 1;
   	}

   	//abre o arquivo da matriz de saida
	arqSaida = fopen(argv[2], "w");
	if(arqSaida == NULL) {
	  fprintf(stderr, "Erro ao abrir o arquivo da matriz de saida.\n");
	  return 1;
	}

	GET_TIME(fim);

	//calcula o tempo gasto com as inicializacoes
	delta1 = fim - inicio;

	//-----------------------------------------------------------------------------------
   	GET_TIME(inicio);

	multiplicaMatrizMAtriz();

	GET_TIME(fim);

	//calcula o tempo gasto com a parte sequencial (multiplicacao das matrizes)
	delta2 = fim - inicio;

	imprimeMatriz(arqSaida);

	//-----------------------------------------------------------------------------------
	GET_TIME(inicio);

	free(matrizA);
	free(matrizB);
	free(matrizSaida);

	GET_TIME(fim);

	//calcula o tempo gasto com as finalizacoes 
	delta3 = fim - inicio;
	//exibe os tempos gastos em cada parte do programa 
	printf("Tempo inicializacoes: %.8lf\n", delta1);
	printf("Tempo multiplicacao de duas matrizes %dx%d sequencial: %.8lf\n", linhas, colunas, delta2);
	printf("Tempo finalizacoes: %.8lf\n", delta3);
}

void multiplicaMatrizMAtriz(){
	for(int i = 0; i < linhas; i++){
		for(int j = 0; j < colunas; j++){
			for(int k = 0; k < colunas; k++){
				matrizSaida[i][j] += matrizA[i][k] * matrizB[k][j];
				//printf("thread %d, i = %d, j = %d, k = %d\n", id, i, j, k);
			}
		}
	}
}

int preencheMatriz(){
	matrizA = (float **) malloc(sizeof(float *) * linhas);
	matrizB = (float **) malloc(sizeof(float *) * linhas);
	matrizSaida = (float **) malloc(sizeof(float *) * linhas);
	if(!matrizA || !matrizB || !matrizSaida){
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