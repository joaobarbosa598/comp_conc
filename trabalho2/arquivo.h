#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int sensores; //numero de threads escritoras
int atuadores; //numero de threads leitoras

typedef struct{
	int temperatura;
	int idSensor;
	int idLeitura;
}temperatura;

temperatura meusDados[60]; //espaco de memoria compartilhado com os sensores e atuadores
//variaveis para sincronizacao
pthread_mutex_t mutex;
pthread_cond_t cond_leit, cond_escr;

void iniciaLeitura(int id);
void fechaLeitura(int id);
void iniciaEscrita(int id);
void fechaEscrita(int id);
void * sensor(void * arg); //funcao que escreve na estrutua de dados compartilhados
void * atuador(void * arg); //funcao de le os dados compartilhados