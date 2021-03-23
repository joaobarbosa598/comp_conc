#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define TAM  100
#define NTHREADS  2

int vetor[TAM];

void * incrementa (void * arg);
void imprimeVetor();

int main (void){
	pthread_t tid_sistema[NTHREADS];
	int i;
	int identificador[NTHREADS]; //identificador local da thread

	for(i = 0; i < TAM; i++){
		vetor[i]=0;
	}
	imprimeVetor();

	for(i = 0; i < NTHREADS; i++){
		identificador[i] = i+1;
		if(pthread_create(&tid_sistema[i], NULL, incrementa, (void *)&identificador[i])){
			printf("ERRO -- pthread_create\n");
		}
	}

	//espera as threads terminarem
	for(int i = 0; i < NTHREADS; i++){
		if(pthread_join(tid_sistema[i], NULL)){
			printf("ERRO -- pthread_join\n");
		}
	}

	imprimeVetor();

	//imprime a mensagem de boas vindas
	printf("Ola, sou a thread principal!\n");
	
	return 0;
}

void * incrementa (void * arg){
	int identificador = *(int *)arg;
	printf("Ola, a thread %d esta sendo executada e ira comecar incrementando na posicao %d do vetor\n", identificador, identificador-1);
	//Aqui a thread com o identificador par ira incrementar 1 nas posicoes impares do vetor (1,3,5,7,9,...)
	if(identificador%2==0){
		for(int i=identificador-1; i < TAM; i+=2){
			vetor[i] += 1;
		}
	}
	//Aqui a thread com o identificador impar ira incrementar 1 nas posicoes pares de vetor (0,2,4,6,8,...)
	else{
		for(int i=identificador-1; i < TAM; i+=2){
			vetor[i] += 1;
		}
	}	
}

void imprimeVetor(){
	int i;
	for(i=0; i<TAM;i++){
		if( (i%50)==0  && i!=0)
			printf("\n");
		printf("%d ", vetor[i]);
		
	}
	printf("\n"); 
}