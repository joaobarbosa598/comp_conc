#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

int n = 0;
sem_t s, d; //s inicializa com 1 e d com 0
int Buffer[10]; //espaco de dados compartilhados
int in = 0, out = 0;

void consome_item(int );
void retira_item(int *);
void produz_item(int *);
void insere_item(int );
void * cons(void * args);
void * prod(void * args);
void exibeBuffer(void);

int main (int argc, char **argv){

	pthread_t tid[2];
	int id[2] = {0,1};
	sem_init(&s,0,1); 
	sem_init(&d,0,0); 

	if(pthread_create(&tid[0], NULL, cons, (void *) &id[0]))
		exit(-1);

	if(pthread_create(&tid[1], NULL, prod, (void *) &id[1]))
		exit(-1);

	pthread_exit(NULL);
	return 1;
}

void * cons(void * args){
	int id = *(int *) args;
	int item;
	sem_wait(&d);
	while(1){
		printf("Consumidor [%d] iniciou\n", id);
		sem_wait(&s);
		retira_item(&item);
		n--;
		sem_post(&s);
		consome_item(item);
		if(n==0) sem_wait(&d);

		sleep(1);
	}
}

void * prod(void * args){
	int id = *(int *) args;
	int item;
	while(1){
		printf("Produtor [%d] iniciou\n", id);
		produz_item(&item);
		sem_wait(&s);
		insere_item(item);
		n++;
		if(n==1) sem_post(&d);
		sem_post(&s);

		sleep(1);
	}
}

void consome_item(int item){
	for(int i = 0; i < 10; i++){
		if(Buffer[i] == item){
			Buffer[i] = 0;
			out = (out+1)%10;
			exibeBuffer();
			return;
		}
	}
}

void retira_item(int * item){
	*item = Buffer[out];
	printf("Item a ser retirado: %d\n", *item);
}

void produz_item(int * item){

	*item = rand()% 16 + 1;
	printf("Item produzido: %d\n", *item);
}

void insere_item(int item){
	Buffer[in] = item;
	in = (in+1)%10;
	exibeBuffer();
}

void exibeBuffer(void){
	for(int i = 0; i < 10; i++){
		printf("%d ", Buffer[i]);
		if(!(i%10) && i!=0)
			printf("\n");
	}

	printf("\n");
}