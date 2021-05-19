/*Nome: João Vitor de Freitas Barbosa	DRE: 117055449
Nome: Leonardo Emerson André Alves	DRE: 117062624
Trabalho 2 de computacao concorrente*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "arquivo.h"

//funcao principal
int main(int argc, char *argv[]) {
	//identificadores das threads
	pthread_t *tid;
	int *id;

	//inicializa as variaveis de sincronizacao
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond_leit, NULL);
	pthread_cond_init(&cond_escr, NULL);

	if(argc < 2) {
   		fprintf(stderr, "Digite: %s <quantidade de sensores>.\n", argv[0]);
   		return 1;
   	}

	sensores = atuadores = atoi(argv[1]);

	tid = (pthread_t *) malloc(sizeof(pthread_t)*(sensores+atuadores));
	if(tid==NULL){puts("ERRO-malloc"); return 2;}

	id = (int*)malloc(sizeof(int)*(sensores+atuadores));
	if(id==NULL){puts("ERRO:mallocID"); return 3;}

	//cria as threads leitoras
	for(int i=0; i<atuadores; i++) {
		*(id+i) = i+1;
		if(pthread_create(tid+i, NULL, atuador, (void *) (id+i) )) exit(-1);
	} 

	//cria as threads escritoras
	for(int i=0; i<sensores; i++) {
		*(id+i+atuadores) = i+1;
		if(pthread_create(tid+i+atuadores, NULL, sensor, (void *) (id+i+atuadores) )) exit(-1);
	} 

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond_leit);
	pthread_cond_destroy(&cond_escr);
	pthread_exit(NULL);
  return 0;
}