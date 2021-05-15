#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "arquivo.h"

//variaveis do problema
int leit=0; //contador de threads lendo
int escr=0; //contador de threads escrevendo
int pos = 0;

void iniciaLeitura(int id){
	pthread_mutex_lock(&mutex);
	//printf("L[%d] quer ler\n", id);
	while(escr > 0) {
		//printf("L[%d] bloqueou\n", id);
		pthread_cond_wait(&cond_leit, &mutex);
		//printf("L[%d] desbloqueou\n", id);
	}
	leit++;
	pthread_mutex_unlock(&mutex);
}

void fechaLeitura(int id){
	pthread_mutex_lock(&mutex);
	//printf("L[%d] terminou de ler\n", id);
	leit--;
	if(leit==0) pthread_cond_signal(&cond_escr);
	pthread_mutex_unlock(&mutex);
}

void iniciaEscrita(int id){
	pthread_mutex_lock(&mutex);
	//printf("E[%d] quer escrever\n", id);
	while((leit>0) || (escr>0)) {
		//printf("E[%d] bloqueou\n", id);
		pthread_cond_wait(&cond_escr, &mutex);
		//printf("E[%d] desbloqueou\n", id);
	}
	escr++;
	pthread_mutex_unlock(&mutex);
}

void fechaEscrita(int id){
	pthread_mutex_lock(&mutex);
	//printf("E[%d] terminou de escrever\n", id);
	escr--;
	pthread_cond_signal(&cond_escr);
	pthread_cond_broadcast(&cond_leit);
	pthread_mutex_unlock(&mutex);
}

void * sensor(void * arg){
	int *id = (int *) arg;
	int iteracaoSensor = 0; //identificador local de iteracao da leitura do Sensor
	int temperaturaAux = 0;
	while(1) {
		iniciaEscrita(*id);
		//printf("Escritora %d esta escrevendo\n", *id);
		//representa = rand() % range + min ----> 16 + 25 = [25,40];
		temperaturaAux = rand()% 16 + 25;
		if(temperaturaAux > 30){
			meusDados[pos].temperatura = temperaturaAux;
			meusDados[pos].idSensor = *id;
			meusDados[pos].idLeitura = iteracaoSensor;
			pos = (pos+1)%60;
			iteracaoSensor++;
		}
		fechaEscrita(*id);
		sleep(2);
	} 
	free(arg);
	pthread_exit(NULL);
}

void * atuador(void * arg){
	int *id = (int *) arg;
	while(1) {
		iniciaLeitura(*id);
		//printf("Leitora %d esta lendo\n", *id);
		for(int i = 0; i < 60; i++){
			if(meusDados[i].idSensor == *id && meusDados[i].idLeitura>0){
				//coloquei toda a saida em um print apenas pois com mais, o print estava saindo desordenado
				printf("meusDados[%d].temperatura:%d\nmeusDados[%d].idSensor:%d\nmeusDados[%d].idLeitura:%d\n\n", i, meusDados[i].temperatura, i, meusDados[i].idSensor, i, meusDados[i].idLeitura);
				/*printf("meusDados[%d].temperatura:%d\n", i, meusDados[i].temperatura);
				printf("meusDados[%d].idSensor:%d\n", i, meusDados[i].idSensor);
				printf("meusDados[%d].idLeitura:%d\n\n", i, meusDados[i].idLeitura);*/
			}
		}
		fechaLeitura(*id);
		sleep(2);
	} 
	free(arg);
	pthread_exit(NULL);
}