#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define vector_size 7 // tamanho do vetor de inteiros
#define l_threads 2 // numero de threads leitoras
#define e_threads 2 // numero de threads escritoras

int int_vector[vector_size]; // vetor de inteiros
int leit=0; //contador de threads lendo
int escr=0; //contador de threads escrevendo
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // mutex
pthread_cond_t cond_leit = PTHREAD_COND_INITIALIZER; // condicao para leitura
pthread_cond_t cond_escr = PTHREAD_COND_INITIALIZER; // condicao para escrita

void throw(char* msg)
{
	printf("%s\n", msg);
	exit(-1);
}

void entra_leitura(int id)
{
	pthread_mutex_lock(&mutex);
	printf("L[%d] quer acesso ao vetor\n", id);
	while(escr > 0) {
		printf("L[%d] bloqueou\n", id);
		pthread_cond_wait(&cond_leit, &mutex);
		printf("L[%d] desbloqueou\n", id);
	}
	leit++;
	pthread_mutex_unlock(&mutex);
	
	float sum = 0;
	printf("L[%d] -> ", id);
	for(int i = 0; i < vector_size; i++){
		printf("[%d]:%d ", i, int_vector[i]);
		sum += int_vector[i];
	}
	printf("media: %f\n", sum/vector_size );
}

void sai_leitura(int id)
{
	pthread_mutex_lock(&mutex);
	printf("L[%d] terminou de ler\n", id);
	leit--;
	if(leit==0)
		pthread_cond_signal(&cond_escr);
	pthread_mutex_unlock(&mutex);
}

void entra_escrita(int id)
{
	pthread_mutex_lock(&mutex);
	printf("E[%d] quer acesso ao vetor\n", id);
	
	while(leit > 0 || escr > 0) {
		printf("E[%d] bloqueou\n", id);
		pthread_cond_wait(&cond_escr, &mutex);
		printf("E[%d] desbloqueou\n", id);
	}
	escr++;
	
	int_vector[0] = id;
	int_vector[vector_size - 1] = id;
	
	if(vector_size > 2)
		for(int i = 1; i < vector_size - 1; i++)
			int_vector[i] = 2 * id;
	
	pthread_mutex_unlock(&mutex);
}

void sai_escrita(int id)
{
	pthread_mutex_lock(&mutex);
	
	printf("E[%d] terminou de escrever\n", id);
	escr--;
	pthread_cond_signal(&cond_escr);
	pthread_cond_broadcast(&cond_leit);
	
	pthread_mutex_unlock(&mutex);
}

void* leitor(void* arg)
{
	int id = *(int*)arg;
	while(1){
		entra_leitura(id);
		sai_leitura(id);
		sleep(2);
	}
	pthread_exit(NULL);
}

void* escritor(void* arg)
{
	int id = *(int*)arg;
	while(1) {
		entra_escrita(id);
		sai_escrita(id);
		sleep(2);
	}
	pthread_exit(NULL);
}

int main(int argc, char** argv)
{
	// inicializando threads
	pthread_t threads[l_threads + e_threads];
	int thread_ids[l_threads + e_threads];
	
	// inicializando vetor de inteiros
	for(int i = 0; i < vector_size; i++) int_vector[i] = vector_size;
	
	// criando leitores
	for(int i = 0; i < l_threads; i++)
	{
		thread_ids[i] = i + 1;
		if(pthread_create(&threads[i], NULL, leitor, (void *)&thread_ids[i]))
			throw("error on thread creation");
	}
	
	// criando escritores
	for(int i = 0; i < e_threads; i++)
	{
		thread_ids[i + l_threads] = i + 1;
		if(pthread_create(&threads[i + l_threads], NULL, escritor, (void *)&thread_ids[i + l_threads]))
			throw("error on thread creation");
	}
	
	for(int i = 0; i < l_threads + e_threads; i++)
		pthread_join(threads[i], NULL);
	
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond_leit);
	pthread_cond_destroy(&cond_escr);
	
	pthread_exit(NULL);
	
	return 0;
}