/*Multiplicação de matriz-vetor(considerando matrizes quadradas)*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

float *matriz; //matriz de entrada
float *vetor; //vetor de entrada
float *saida; //matriz de saida

typedef struct {
	int id; //identificador do elemento que a thread ira processar
	int dim; //dimensao das estruturas de entrada
} tArgs;

//funcao que as threads executarao
void * multiplicaMatrizVetor(void * arg);

int main(int argc, char* argv[]){

	int dim; //dimensao da matriz de entrada
	pthread_t *tid; //identificadores das threads no sistema
	tArgs *args; //identificadores locais das threads e dimensao

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
	//alocacao das estruturas
	tid = (pthread_t *) malloc(sizeof(pthread_t)*dim);
	if(tid==NULL){puts("ERRO-malloc"); return 2;}
	args = (tArgs *) malloc(sizeof(tArgs)*dim); //o dim aqui seria o numero de linhas e não coluna
	if(args==NULL){puts("ERRO-malloc"); return 2;}

	//criacao das threads
	for(int i=0; i<dim; i++){
		(args+i)->id=i;
		(args+i)->dim=dim;
		if(pthread_create(tid+i, NULL, multiplicaMatrizVetor, (void *) (args+i))){
			puts("ERRO--pthread_create"); return 3;
		}
	}

	//espera pelo termino das threads
	for(int i=0; i<dim; i++){
		pthread_join(*(tid+i), NULL);
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
	free(args);
	free(tid);
	return 0;
}

void * multiplicaMatrizVetor(void * arg){
	tArgs *args = (tArgs *) arg;
	for(int j=0; j<args->dim; j++){
			saida[args->id] += matriz[(args->id)*(args->dim) +j] * vetor[j];
	}
	pthread_exit(NULL);
}