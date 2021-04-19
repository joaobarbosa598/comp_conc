#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

int TURN = 1;
int x = 0;


void * T0(void *arg) {

	while(1){
		printf("Entrando em T0\n");
		while(TURN != 0){;}
		for(int i=0; i < 100000; i++)
			x++;
		TURN=1;
		printf("Saindo de T0, x:%d\n", x);
	}
	pthread_exit(NULL);
}

void * T1(void *arg) {

	while(1){
		printf("Entrando em T1\n");
		while(TURN != 1){;}
		for(int i=0; i < 100000; i++)
			x--;
		TURN=0;
		printf("Saindo de T1, x:%d\n", x);
	}
	pthread_exit(NULL);
}

int main(void) {

	pthread_t *tid;

	tid = (pthread_t *)malloc(sizeof(pthread_t)*2);
	if(tid == NULL){
		fprintf(stderr, "ERRO--malloc\n");
		return 1;
	}

	if (pthread_create(tid+1, NULL, T0, NULL)) 
		return 2;
	if (pthread_create(tid, NULL, T1, NULL)) 
		return 2;


	pthread_join(*(tid), NULL);
	pthread_join(*(tid+1), NULL);
	free(tid);
	printf("x:%d TURN:%d\n", x, TURN);
	return 0;
}