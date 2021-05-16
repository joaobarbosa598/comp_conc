#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int x = 0; pthread_mutex_t x_mutex; 
pthread_cond_t x_cond;

void * A(void* args);
void * B(void* args);

int main(void) {

	pthread_t *tid;
	int *id; //identificadores locais das threads
	
	tid = (pthread_t *)malloc(sizeof(pthread_t)*10);
	if(tid == NULL){
		fprintf(stderr, "ERRO--malloc\n");
		return 1;
	}

	for(int i = 0; i < 11; i++){
		id = (int*)malloc(sizeof(int));
		if(id==NULL){puts("ERRO:mallocID"); return 3;}
		*id = i;
		if(i > 0){
			if (pthread_create(tid+i, NULL, A, (void *)id)) 
				return 2;
		}
		else {
			if (pthread_create(tid+i, NULL, B, (void *)id)) 
				return 2;
		}
	}

	for(int i=0; i<11; i++){
		if(pthread_join(*(tid+i), NULL)){
			puts("ERRO--pthread_join"); return 4;
		}
	}
	free(tid);

	return 0;
}

void * A (void *tid) {
	pthread_mutex_lock(&x_mutex);
	for (int i=0; i<1000; i++) {
		x++;
		if((x%10)==0){
			//printf("%d\n", x);
			pthread_cond_signal(&x_cond);
		}
	} 
	pthread_mutex_unlock(&x_mutex);
}

void * B (void *tid) {
	pthread_mutex_lock(&x_mutex);
	if(x%10!=0 || x==0)
		pthread_cond_wait(&x_cond, &x_mutex);
	printf("X=%d\n", x);
	pthread_mutex_unlock(&x_mutex);
}