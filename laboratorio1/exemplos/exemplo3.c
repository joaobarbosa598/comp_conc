#include<stdio.h>
#include<pthread.h>

#define NTHREADS 10

//funcao que a thread ira executar
void * tarefa (void * arg){
	int ident = *(int *)arg;
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
	for(int i = NTHREADS-1; i > -1 ; i--){ 
		printf("Esperando a thread %d terminar\n", ident[i]);
		if(pthread_join(tid[i], NULL)){
			printf("ERRO -- pthread_join\n");
		}
	}
	//imprime a mensagem de boas vindas
	printf("Ola, sou a thread principal!\n");
	//desvincunla o termino da main do termino do programa
	pthread_exit(NULL);
	return 0;
}