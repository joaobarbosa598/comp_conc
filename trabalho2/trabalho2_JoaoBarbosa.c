/*Nome: João Vitor de Freitas Barbosa
DRE: 117055449
Trabalho 2 de computacao concorrente*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "arquivo.h"

//funcao principal
int main(void) {
	//identificadores das threads
	pthread_t tid[L+E];
	int id[L+E];

	//inicializa as variaveis de sincronizacao
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond_leit, NULL);
	pthread_cond_init(&cond_escr, NULL);

	//cria as threads leitoras
	for(int i=0; i<L; i++) {
		id[i] = i+1;
		if(pthread_create(&tid[i], NULL, atuador, (void *) &id[i])) exit(-1);
	} 

	//cria as threads escritoras
	for(int i=0; i<E; i++) {
		id[i+L] = i+1;
		if(pthread_create(&tid[i+L], NULL, sensor, (void *) &id[i+L])) exit(-1);
	} 

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond_leit);
	pthread_cond_destroy(&cond_escr);
	pthread_exit(NULL);
  return 0;
}