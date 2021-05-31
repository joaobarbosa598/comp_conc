#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define NTHREADSCARROSNORTE 10
#define NTHREADSCARROSSUL 10

//variaveis globais
sem_t em, dorme, e; 
int cont=0, n_sul=0, n_norte=0;

void espera(void);
void libera(void);
void carrosNorteEntraPonte(int);
void carrosNorteSaiPonte(int);
void carrosSulEntraPonte(int);
void carrosSulSaiPonte(int);
void * carrosNorte(void * args);
void * carrosSul(void * args);

int main(int argc, char *argv[]) {

	pthread_t tid[NTHREADSCARROSNORTE+NTHREADSCARROSSUL];
  	int *id;
	//inicializacoes feitas na main...
	sem_init(&em,0,1); 
	sem_init(&dorme,0,0); 
	sem_init(&e,0,1);

	for(int i = 0; i < NTHREADSCARROSNORTE; i++){
		id = malloc(sizeof(int)); 
		if(id==NULL) {printf("--ERRO: malloc()\n"); exit(-1); }
		*id = i;
		if (pthread_create(&tid[i], NULL, carrosNorte, (void *)id)) { 
			printf("--ERRO: pthread_create()\n"); exit(-1); 
		}
	}

	for(int i = NTHREADSCARROSNORTE; i < NTHREADSCARROSNORTE+NTHREADSCARROSSUL; i++){
		id = malloc(sizeof(int)); 
		if(id==NULL) {printf("--ERRO: malloc()\n"); exit(-1); }
		*id = i;
		if (pthread_create(&tid[i], NULL, carrosSul, (void*)id)) { 
			printf("--ERRO: pthread_create()\n"); exit(-1); 
		}
	}

	//espera todas as threads terminarem e imprime o vetor de saída
	for(int i =0; i < NTHREADSCARROSNORTE+NTHREADSCARROSSUL; i++) 
	{
		if (pthread_join(tid[i], NULL)) 
		{
			printf("--ERRO: pthread_join()\n"); 
			exit(-1);
		}
	}


	sem_destroy(&em);
	sem_destroy(&e);
	sem_destroy(&dorme);

}

void * carrosNorte(void * args){
	int id = *(int *) args;
	while(1){
		carrosNorteEntraPonte(id);
		printf("Carro [%d] entrou na ponte pelo norte\n", id);
		carrosNorteSaiPonte(id);
		printf("Carro [%d] saiu da ponte\n", id);
		sleep(1);
	}
}

void * carrosSul(void * args){

	int id = *(int *) args;
	while(1){
		carrosSulEntraPonte(id);
		printf("Carro [%d] entrou na ponte pelo sul\n", id);
		carrosSulSaiPonte(id);
		printf("Carro [%d] saiu da ponte\n", id);
		sleep(1);
	}
}

void espera(void){
	sem_wait(&em);
	cont++;
	sem_post(&em);
	sem_wait(&dorme);
	cont--;
	if(cont>0)
		sem_post(&dorme);
	else
		sem_post(&em);
}

void libera(void){
	sem_wait(&em);
	if(cont>0)
		sem_post(&dorme);
	else
		sem_post(&em);
}
//Pseudo-codigo dos carros do norte
void carrosNorteEntraPonte(int id){
	sem_wait(&e);

	while(n_sul > 0) {
		sem_post(&e);
		printf("Carro [%d] vindo pelo norte ira se bloquear\n", id);
		espera();
		printf("Carro [%d] vindo pelo norte ira se desbloquear\n", id);
		sem_wait(&e);
	}
	n_norte++;
	sem_post(&e);
}

void carrosNorteSaiPonte(int id){
	//...aqui codigo para atravessar a ponte
	sem_wait(&e); n_norte--;

	if(n_norte == 0)
		libera();

	sem_post(&e);
}

//Pseudo-codigo dos carros do sul
void carrosSulEntraPonte(int id){
	sem_wait(&e);

	while(n_norte > 0) {
		sem_post(&e);
		printf("Carro [%d] vindo pelo sul ira se bloquear\n", id);
		espera();
		printf("Carro [%d] vindo pelo sul ira se desbloquear\n", id);
		sem_wait(&e);
	}
	n_sul++;
	sem_post(&e);
}

//...aqui codigo para atravessar a ponte
void carrosSulSaiPonte(int id){
	sem_wait(&e); n_sul--;

	if(n_sul == 0)
		libera();

	sem_post(&e);
}