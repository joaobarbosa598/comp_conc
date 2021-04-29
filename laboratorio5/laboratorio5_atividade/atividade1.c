/*Laboratorio 5
Nome: Joao Vitor de Freitas Barbosa
DRE: 117055449*/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>


/* Variaveis globais */
int *vetor;
int *vetorSequencial;
int nThreads;
int bloqueadas = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;

void barreira(int i);
void * somaVetor(void * arg);
void inicializaVetores(void);
void imprimeVetorConcorrente(void);
void imprimeVetorSequencial(void);
void somaVetorSequencial(void);
void comparaVetores(void);


int main(int argc, char **argv){

	pthread_t *tid; //identificadores das threads no sistema
	int *id;
	int potenciaDeDois = 0;

	/* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
	pthread_mutex_init(&x_mutex, NULL);
	pthread_cond_init (&x_cond, NULL);

	if(argc<2){
		fprintf(stderr, "Digite: %s <numero de elementos do vetor>\n", argv[0]);
		return 1;
	}

	nThreads = atoi(argv[1]);

	for(int i = 1; i < 11; i++){
		if(pow(2,i)==nThreads){
			potenciaDeDois = 1;
		}
	}
	if(potenciaDeDois==0){
		printf("Número de elementos do vetor deve uma potencia de 2.\n");
		return 2;
	}

	//aloca memoria para ambos os vetores: vetorSequencial e vetorConcorrente
	inicializaVetores();
	//imprime o estado inicial do vetorConcorrente
	imprimeVetorConcorrente();
	//faz a soma sequencial do vetorSequencial
	somaVetorSequencial();


	tid = (pthread_t *)malloc(sizeof(pthread_t) * nThreads);
	if(tid == NULL){
		fprintf(stderr, "ERRO--malloc\n");
		return 2;
	}

	/* Cria as threads */
	for(int i=0; i<nThreads; i++){
		id = (int*)malloc(sizeof(int));
		if(id==NULL){puts("ERRO:mallocID"); return 3;}
		*id = i;
		if(pthread_create(tid+i, NULL, somaVetor, (void *)id)){
			fprintf(stderr, "ERRO-pthread_create\n");
			return 3;
		}
	}

	/* Espera todas as threads completarem */
	for(int i=0; i<nThreads; i++){
		if(pthread_join(*(tid+i), NULL)){
			fprintf(stderr, "ERRO--pthread_join\n");
			return 4;
		}
	}

	imprimeVetorConcorrente();
	imprimeVetorSequencial();
	comparaVetores();

	/* Desaloca variaveis e termina */
	pthread_mutex_destroy(&x_mutex);
	pthread_cond_destroy(&x_cond);
	return 0;
}

//Funcao das threads
void * somaVetor(void * arg) {
	int id = *(int *)arg;
	int aux;

	for(int i = 1; i <= id; i *= 2) {
		aux = vetor[id-i];
		//espera a leitura dos elementos do vetorConcorrente
		barreira(i);
		vetor[id] += aux;
		//espera a soma dos elementos do vetorConcorrente
		barreira(i);
	}
	pthread_exit(NULL);
}


//Funcao barreira
void barreira(int i) {

	pthread_mutex_lock(&x_mutex);//inicio secao critica
	
	//ultima thread a chegar na barreira
	if (bloqueadas == nThreads - i - 1) {
		pthread_cond_broadcast(&x_cond);
		bloqueadas = 0;
	}
	else {
		bloqueadas++;
		pthread_cond_wait(&x_cond, &x_mutex);
	}

	pthread_mutex_unlock(&x_mutex); //fim secao critica
}

//aloca memoria para ambos os vetores
void inicializaVetores(void){

	vetor = (int *)malloc(sizeof(int) * nThreads);
	vetorSequencial = (int *)malloc(sizeof(int) * nThreads);
	for(int i = 0; i < nThreads; i++){
		*(vetor+i) = i+1;
		*(vetorSequencial+i) = i+1;
	}
}

void somaVetorSequencial(void){

	for(int i = 0; i < nThreads; i++){
		vetorSequencial[i] = vetorSequencial[i] + vetorSequencial[i-1];
	}
}

void comparaVetores(void){
	for(int i = 0; i < nThreads; i++){
		if(vetor[i] != vetorSequencial[i]){
			printf("A soma concorrente está errada\n");
		}
	}
	printf("Os valores da soma concorrente estão corretos\n");
}

void imprimeVetorConcorrente(void){
	printf("Vetor concorrente: ");
	for(int i = 0; i < nThreads; i++)
		printf("%d ", vetor[i]);
	printf("\n");
}

void imprimeVetorSequencial(void){
	printf("Vetor sequencial: ");
	for(int i = 0; i < nThreads; i++)
		printf("%d ", vetorSequencial[i]);
	printf("\n");
}