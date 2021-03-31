/*Multiplicação de matriz-vetor(considerando matrizes quadradas)*/
#include <stdio.h>
#include <stdlib.h>

float *matriz; //matriz de entrada
float *vetor; //vetor de entrada
float *saida; //matriz de saida

int main(int argc, char* argv[]){

	int dim; //dimensao da matriz de entrada

	//leitura e avaliacao dos parametros de entrada
	if(argc<2){ //argc sempre vai ter tamanho 1 ou mais, o primeiro parametro é o nome do arquivo
		printf("Digite: %s <dimensao da matriz>\n", argv[0]);
		return 1;
	}
	//atoi converte char para int
	dim = atoi(argv[1]); //argv[0] é o nome do arquivo, argv[1] é o parametro de entrada

	//alocacao de memoria para as estruturas de dados
	matriz = (float *) malloc(sizeof(float) * dim * dim);
	if(matriz == NULL){
		printf("ERRO--malloc\n");
		return 2;
	}

	vetor = (float *) malloc(sizeof(float) * dim);
	if(vetor == NULL){
		printf("ERRO--malloc\n");
		return 2;
	}

	saida = (float *) malloc(sizeof(float) * dim);
	if(saida == NULL){
		printf("ERRO--malloc\n");
		return 2;
	}
	//inicializacao das estruturas de dados de entrada e saida
	for(int i=0; i < dim; i++){
		for(int j=0; j < dim; j++){
			matriz[i*dim+j] = 1; //equivalente a matriz[i][j]
		}
		vetor[i] = 1;
		saida[i] = 0;
	}
	//multiplicacao da matriz pelo vetor
	for(int i=0; i<dim; i++){
		for(int j=0; j<dim; j++){
			saida[i] += matriz[i*dim+j]*vetor[j];
		}
	}

	//exibicao dos resultados
	/*
	puts("Matriz de entrada:");
	for(int i=0; i<dim; i++){
		for(int j=0; j<dim; j++)
			printf("%.1f ", matriz[i*dim+j]);
		puts("");
	}
	puts("Vetor de entrada:");
	for(int i=0; i<dim; i++)
		printf("%.1f ", vetor[i]);
	puts("");*/
	puts("Vetor de saida:");
	for(int i=0; i<dim; i++)
		printf("%.1f ", saida[i]);
	puts("");

	//liberacao da memoria
	free(matriz);
	free(vetor);
	free(saida);
	return 0;
}