#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

int e = 0, l = 0; //globais
sem_t em_e, em_l, escrita, leitura; //s inicializa com 1 e d com 0

void * leitor(void * args);
void * escritor(void * args);

int main (int argc, char **argv){

	pthread_t tid[2];
	int id[2] = {0,1};
	sem_init(&em_l,0,1); 
	sem_init(&em_e,0,1); 
	sem_init(&escrita,0,1); 
	sem_init(&leitura,0,1); 

	if(pthread_create(&tid[0], NULL, leitor, (void *) &id[0]))
		exit(-1);

	if(pthread_create(&tid[1], NULL, escritor, (void *) &id[1]))
		exit(-1);

	pthread_exit(NULL);
	return 1;
}

void * leitor(void * args){
	while(1){
		sem_wait(&leitura);
		sem_wait(&em_l); 
		l++;
		if(l == 1)
			sem_wait(&escrita);
	
		sem_post(&em_l);
		sem_post(&leitura);
		printf("lendo\n");
		sem_wait(&em_l);
		l--;
		if(l == 0)
			sem_post(&escrita);
	
		sem_post(&em_l);

		sleep(0.5);
	}
}

void * escritor(void * args){
	while(1){
		sem_wait(&em_e);
		e++;
		if(e==1)
			sem_wait(&leitura);
	
		sem_post(&em_e);
		sem_wait(&escrita);
		printf("escrevendo\n");
		sem_post(&escrita);
		sem_wait(&em_e);
		e--;
		if(e==0)
			sem_post(&leitura);
	
		sem_post(&em_e);
		sleep(0.5);
	}
}