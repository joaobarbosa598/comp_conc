#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

int x = 0; //variavel global
int y = 0; //variavel global
pthread_mutex_t lock; //variavel especial para sincronizacao de exclusao mutua

void * T1(void *arg) {
	int a = 0;
	while(a < 2){
		//entrada de secao critica
		pthread_mutex_lock(&lock);
		x++; //incrementa a variavel compartilhada
		x--; //decrementa a variavel compartilhada
		pthread_mutex_unlock(&lock);
		//saida de secao critica
		if(x==0){
			printf("T1 x=%d\n", x);
			a++;
		}
		printf("T1 a=%d\n", a);
	}

	pthread_exit(NULL);
}

void * T2(void *arg) {
	int a = 2;
	while(a >0){
		//entrada de secao critica
		pthread_mutex_lock(&lock);
		x++; //incrementa a variavel compartilhada
		x--; //decrementa a variavel compartilhada
		pthread_mutex_unlock(&lock);
		//saida de secao critica
		if(x==0){
			printf("T2 x=%d\n", x);
		}
		a--;
		printf("T2 a=%d\n", a);
	}
	pthread_exit(NULL);
}

void * T3(void *arg) {
	//entrada de secao critica
	pthread_mutex_lock(&lock);
	x--;
	x++;
	y++;
	pthread_mutex_unlock(&lock);
	//saida de secao critica
	pthread_exit(NULL);
}

int main(void) {

	pthread_t *tid;

	//inicializa a variavel de exclusao mutua
	pthread_mutex_init(&lock, NULL);
	
	tid = (pthread_t *)malloc(sizeof(pthread_t)*3);
	if(tid == NULL){
		fprintf(stderr, "ERRO--malloc\n");
		return 1;
	}

	
	if (pthread_create(tid+1, NULL, T2, NULL)) 
		return 2;

	if (pthread_create(tid+2, NULL, T3, NULL)) 
		return 2;
	if (pthread_create(tid, NULL, T1, NULL)) 
		return 2;


	pthread_join(*(tid), NULL);
	pthread_join(*(tid+2), NULL);
	pthread_join(*(tid+1), NULL);

	pthread_mutex_destroy(&lock);
	free(tid);

	return 0;
}