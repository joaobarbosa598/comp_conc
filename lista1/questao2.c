#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

int x = 0; //variavel global

void * T1(void *arg) {
	x++;
	x--;
	if(x == 0)
		printf("%d\n",x);

	pthread_exit(NULL);
}

void * T2(void *arg) {
	x--;
	x--;
	if(x == -2)
		printf("%d\n", x);
	pthread_exit(NULL);
}

void * T3(void *arg) {
	x++;
	x++;
	if(x == 2)
		printf("%d\n",x);
	
	pthread_exit(NULL);
}

int main(void) {

	pthread_t *tid;
	
	tid = (pthread_t *)malloc(sizeof(pthread_t)*3);
	if(tid == NULL){
		fprintf(stderr, "ERRO--malloc\n");
		return 1;
	}

	for(int i = 2; i > -1; i--){
		if(i == 0){
			if (pthread_create(tid+i, NULL, T1, NULL)) 
				return 2;
		}
		else if(i == 1){
			if (pthread_create(tid+i, NULL, T2, NULL)) 
				return 2;
		}
		else{
			if (pthread_create(tid+2, NULL, T3, NULL)) 
				return 2;
		}
	}

	/*if (pthread_create(tid+1, NULL, T2, NULL)) 
		return 2;

	if (pthread_create(tid+2, NULL, T3, NULL)) 
		return 2;
	if (pthread_create(tid, NULL, T1, NULL)) 
		return 2;*/

	rename ("foo", "bar");
	pthread_join(*(tid), NULL);
	pthread_join(*(tid+2), NULL);
	pthread_join(*(tid+1), NULL);
	free(tid);

	return 0;
}