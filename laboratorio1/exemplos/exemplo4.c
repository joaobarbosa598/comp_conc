#include<stdio.h>
#include<pthread.h>

#define NTHREADS 10

int vetor[NTHREADS];

//funcao que a thread ira executar
void * tarefa (void * arg){
	int ident = *(int *)arg;
	vetor[ident-1] = ident;
	printf("Ola, sou a thread %d!\n", ident);
	pthread_exit(NULL);
}

//funcao principal
int main (void){
	pthread_t tid[NTHREADS]; //identificador da thread no sistema
	int ident[NTHREADS]; //identificador local da thread
	//cria as threads novas
	for(int i = 0; i < NTHREADS; i++){
		ident[i] = i+1;
		if(pthread_create(&tid[i], NULL, tarefa, (void *)&ident[i])){
			printf("ERRO -- pthread_create\n");
		}
	}
	//espera as threads terminarem
	for(int i = 0; i < NTHREADS; i++){
		if(pthread_join(tid[i], NULL)){
			printf("ERRO -- pthread_join\n");
		}
	}

	//imprimir o vetor de identificadores
	for(int i = 0; i < NTHREADS; i++){
		printf("%d ", vetor[i]);
	}
	printf("\n");

	//imprime a mensagem de boas vindas
	printf("Ola, sou a thread principal!\n");
	
	return 0;
}