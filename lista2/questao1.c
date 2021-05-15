#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int contaFoo=0, contaBar=0; //variaveis de estado
pthread_cond_t condFoo, condBar; //inicializadas na main
pthread_mutex_t mutex; //inicializado na main

void * Foo(void* args);
void * Bar(void* args);

int main(void) {

	pthread_t *tid;
	int *id; //identificadores locais das threads
	
	tid = (pthread_t *)malloc(sizeof(pthread_t)*10);
	if(tid == NULL){
		fprintf(stderr, "ERRO--malloc\n");
		return 1;
	}

	for(int i = 0; i < 10; i++){
		id = (int*)malloc(sizeof(int));
		if(id==NULL){puts("ERRO:mallocID"); return 3;}
		*id = i;
		if(i < 5){
			if (pthread_create(tid+i, NULL, Foo, (void *)id)) 
				return 2;
		}
		else {
			if (pthread_create(tid+i, NULL, Bar, (void *)id)) 
				return 2;
		}
	}

	for(int i=0; i<10; i++){
		if(pthread_join(*(tid+i), NULL)){
			puts("ERRO--pthread_join"); return 4;
		}
	}
	free(tid);

	return 0;
}

void * Foo(void* args) {
	int id = *(int *)args;
	while(1) {
		//...codigo principal da thread
		//forc ̧a alternancia com as threads Bar
		pthread_mutex_lock(&mutex);
  		//printf("Foo: Comecei: thread %d\n", id);
		contaFoo++;
		//printf("contaFoo: %d\n", contaFoo);
		if(contaFoo==5) {
			contaFoo=0;
  			printf("Foo: Thread %d vai liberar as funcoes Bar para funcionamento\n", id);
			pthread_cond_broadcast(&condBar);
		}
     	printf("Foo %d executou e vai se bloquear...\n", id);
		pthread_cond_wait(&condFoo, &mutex);
     	printf("Foo %d desbloqueou e vai executar...\n", id);
		pthread_mutex_unlock(&mutex);
	} 
}

void * Bar(void* args) {
	int id = *(int *)args;
	//aguarda as threads Foo executarem primeiro
	pthread_mutex_lock(&mutex);
  	//printf("Bar: Comecei: thread %d\n", id);
 	printf("Bar %d não executou e espera todas as Foo terminarem...\n", id);
	pthread_cond_wait(&condBar, &mutex);
 	printf("Bar %d desbloqueou e vai executar...\n", id);
	pthread_mutex_unlock(&mutex);
	while(1) {
		//...codigo principal da thread
		//forc ̧a alternancia com as threads Foo
		pthread_mutex_lock(&mutex);
		contaBar++;
		printf("contaBar: %d\n", contaBar);
		if(contaBar==5) {
			contaBar=0;
  			printf("Bar: Thread %d vai liberar as funcoes Foo para funcionamento\n", id);
			pthread_cond_broadcast(&condFoo);
		}
     	printf("Bar %d executou e vai se bloquear\n", id);
		pthread_cond_wait(&condBar, &mutex);
     	printf("Bar %d desbloqueou e vai executar...\n", id);
		pthread_mutex_unlock(&mutex);
	} 
}