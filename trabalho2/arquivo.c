#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "arquivo.h"

//variaveis do problema
int leit=0; //contador de threads lendo
int escr=0; //contador de threads escrevendo
int escritorasEsperando = 0; //contador de threads escritoras esperando
int pos = 0;

void iniciaLeitura(int id){
	pthread_mutex_lock(&mutex);
	//printf("L[%d] quer ler\n", id);
	while(escr > 0 || escritorasEsperando > 0) {
		//printf("L[%d] bloqueou pois escritores:%d e escritorasEsperando:%d\n", id, escr, escritorasEsperando);
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
		escritorasEsperando += 1;
		pthread_cond_wait(&cond_escr, &mutex);
		escritorasEsperando -= 1;
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
		//printf("Escritora %d esta escrevendo pois escritores:%d e escritorasEsperando:%d\n", *id, escr, escritorasEsperando);
		//representa = rand() % range + min ----> 16 + 25 = [25,40];
		temperaturaAux = rand()% 16 + 25;
		if(temperaturaAux > 30){
			iteracaoSensor++;
			meusDados[pos].temperatura = temperaturaAux;
			meusDados[pos].idSensor = *id;
			meusDados[pos].idLeitura = iteracaoSensor;
			pos = (pos+1)%60;
		}
		fechaEscrita(*id);
		sleep(1);
	} 
	free(arg);
	pthread_exit(NULL);
}

void * atuador(void * arg){
	int *id = (int *) arg;
	int todasAsLeituras = 0;
	int ultimasCincoTemperaturas = 0;
	int ultimasQuinzeTemperaturas = 0;
	int ultimasCincoLeituras = 0;
	int ultimasQuinzeLeituras = 0;
	int temperaturas = 0;
	double media;
	while(1) {
		iniciaLeitura(*id);

		for(int i = 0; i < 60; i++){
			if(meusDados[i].idSensor == *id && meusDados[i].idLeitura>0){
				temperaturas += meusDados[i].temperatura;

				if(meusDados[i].temperatura > 35){
					ultimasQuinzeTemperaturas++;
					ultimasCincoTemperaturas++;
				}
				ultimasCincoLeituras++;
				ultimasQuinzeLeituras++;
				todasAsLeituras++;

				media = ((double)temperaturas)/((double)todasAsLeituras);

				if(ultimasCincoLeituras==5 && ultimasCincoTemperaturas==5){
					//printf("\nAtuador %d com ultimasCincoLeituras:%d e ultimasCincoTemperaturas: %d\n", *id, ultimasCincoLeituras, ultimasCincoTemperaturas);
					printf("Atuador %d emitiu um alerta vermelho!\nTemperatura média de todas as leituras do sensor: %.2lf\n", *id, media);
					ultimasCincoLeituras = 0;
					ultimasCincoTemperaturas =0;
				}
				else if(ultimasQuinzeLeituras>5 && ultimasQuinzeLeituras<=15 && ultimasQuinzeTemperaturas>=5){
					//printf("\nAtuador %d com ultimasQuinzeLeituras:%d e ultimasQuinzeTemperaturas: %d\n", *id, ultimasQuinzeLeituras, ultimasQuinzeTemperaturas);
					printf("Atuador %d emitiu um alerta amarelo!\nTemperatura média de todas as leituras do sensor: %.2lf\n", *id, media);
					ultimasQuinzeLeituras=0;
					ultimasQuinzeTemperaturas=0;
				}
				else{
					//printf("\nAtuador %d com ultimasCincoLeituras:%d e ultimasCincoTemperaturas: %d\n", *id, ultimasCincoLeituras, ultimasCincoTemperaturas);
					//printf("Atuador %d com ultimasQuinzeLeituras:%d e ultimasQuinzeTemperaturas: %d\n", *id, ultimasQuinzeLeituras, ultimasQuinzeTemperaturas);
					printf("Atuador %d emitiu um alerta normal.\nTemperatura média de todas as leituras do sensor: %.2lf\n", *id, media);
				}

				if(ultimasCincoLeituras>5) {ultimasCincoLeituras = 0; ultimasCincoTemperaturas = 0;}
				if(ultimasQuinzeLeituras>15) {ultimasQuinzeLeituras = 0; ultimasQuinzeTemperaturas = 0;}
			}
		}
		todasAsLeituras=0; 
		ultimasCincoTemperaturas=0; ultimasQuinzeTemperaturas=0; 
		ultimasCincoLeituras=0; ultimasQuinzeLeituras=0;
		temperaturas=0; media=0.0;

		fechaLeitura(*id);
		sleep(2);
	} 
	free(arg);
	pthread_exit(NULL);
}