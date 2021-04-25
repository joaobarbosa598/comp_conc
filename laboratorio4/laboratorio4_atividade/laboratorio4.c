/*Laboratorio 4
Nome: Joao Vitor de Freitas Barbosa
DRE: 117055449*/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define NTHREADS  4

/* Variaveis globais */
int x = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond, x_cond2;

void * T1(void * arg);
void * T2(void * arg);
void * T3(void * arg);
void * T4(void * arg);

int main(int argc, int **argv){

	int i; 
 	pthread_t threads[NTHREADS];

	/* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
	pthread_mutex_init(&x_mutex, NULL);
	pthread_cond_init (&x_cond, NULL);
	pthread_cond_init (&x_cond2, NULL);

	/* Cria as threads */
	pthread_create(&threads[0], NULL, T1, NULL);
	pthread_create(&threads[1], NULL, T2, NULL);
	pthread_create(&threads[2], NULL, T3, NULL);
	pthread_create(&threads[3], NULL, T4, NULL);

	/* Espera todas as threads completarem */
	for (i = 0; i < NTHREADS; i++){
		pthread_join(threads[i], NULL);
	}
	printf ("\nFIM\n");

	/* Desaloca variaveis e termina */
	pthread_mutex_destroy(&x_mutex);
	pthread_cond_destroy(&x_cond);
	pthread_cond_destroy(&x_cond2);
	return 0;
}

void * T1 (void * arg){

	//printf("T1: comecei\n");
  	pthread_mutex_lock(&x_mutex);
	if(x < 3){
    	//printf("T1: x= %d, vai se bloquear...\n", x);
		pthread_cond_wait(&x_cond2, &x_mutex);
		//printf("T1: sinal recebido e mutex realocado, x = %d\n", x);
	}
	printf("Volte sempre!\n");
	pthread_mutex_unlock(&x_mutex); 
	pthread_exit(NULL);
}

void * T2 (void * arg){

	//printf("T2: comecei\n");
  	pthread_mutex_lock(&x_mutex);
	if(x < 1){
    	//printf("T2: x= %d, vai se bloquear...\n", x);
		pthread_cond_wait(&x_cond, &x_mutex);
		//printf("T2: sinal recebido e mutex realocado, x = %d\n", x);
	}
	x++;
	if(x>2){
		pthread_cond_signal(&x_cond2);
	}
	printf("Fique a vontade.\n");
	pthread_mutex_unlock(&x_mutex); 
	pthread_exit(NULL);
}

void * T3 (void * arg){
	
	//printf("T3: comecei\n");
  	pthread_mutex_lock(&x_mutex);
	if(x < 1){
    	//printf("T3: x= %d, vai se bloquear...\n", x);
		pthread_cond_wait(&x_cond, &x_mutex);
		//printf("T3: sinal recebido e mutex realocado, x = %d\n", x);
	}
	x++;
	if(x>2){
		pthread_cond_signal(&x_cond2);
	}
	printf("Sente-se por favor.\n");
	pthread_mutex_unlock(&x_mutex); 
	pthread_exit(NULL);
}

void * T4 (void * arg){

	//printf("T4: comecei\n");

	printf("Seja bem-vindo!\n");

	pthread_mutex_lock(&x_mutex);
	x++;
	//printf("T1: x = %d, vai sinalizar a condicao\n", x);
	pthread_cond_broadcast(&x_cond);
	pthread_mutex_unlock(&x_mutex);
}
