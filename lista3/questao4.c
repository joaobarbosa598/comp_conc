#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define N 100
#define P 2
#define C 3

int n = 0;
sem_t s, d; //s inicializa com 1 e d com 0

//variaveis do problema
int Buffer[N]; //espaco de dados compartilhados
int count = 0, in = 0, out = 0;

int consome_item(void);	//retorna o inteiro a ser removido
void remocao_item(int);
int produz_item(void);	//retorna no inteiro produzido
void insere_item(int);
void * consumidor(void * args);
void * produtor(void * args);
void ImprimeBuffer(void);
void IniciaBuffer(void);

int main (int argc, char **argv){

	int i;
	//identificadores das threads
	pthread_t tid[P+C];
	int *id[P+C];
	sem_init(&s,0,1); 
	sem_init(&d,0,0); 

	//inicializa o buffer
	IniciaBuffer();

	//aloca espaco para os IDs das theads
	for(i=0; i <P+C; i++){
		id[i] = malloc(sizeof(int));
		if(id[i] == NULL) exit(-1);
		*id[i] = i+1;
	}

	//cria as theads produtoras
	for(i = 0; i < P; i++){
		if(pthread_create(&tid[i], NULL, produtor, (void *) id[i]))
			exit(-1);
	}

	//cria as threads consumidoras
	for(i = 0; i < C; i++){
		if(pthread_create(&tid[i+P], NULL, consumidor, (void *)id[i+P]))
			exit(-1);
	}

	pthread_exit(NULL);
	return 1;
}

void * consumidor(void * args){
	int id = *(int *) args;
	int item;
	sem_wait(&d);
	while(1){
		sem_wait(&s);
		item = consome_item();
		n--;
		if(count==C){ //todo item inserido no buffer deve ser consumido por todos os consumidores antes de ser removido do buffer
			printf("Item retirado: %d\n", item);
			remocao_item(item);
			count = 0;
		}
		sem_post(&s);
		if(n==0) {
			sem_wait(&d);
		}

		sleep(1);
	}
}

void * produtor(void * args){
	int id = *(int *) args;
	int item;
	while(1){
		item = produz_item();
		printf("Item produzido: %d\n", item);
		sem_wait(&s);
		insere_item(item);
		n++;
		if(n==1) {
			sem_post(&d);
		}
		sem_post(&s);

		sleep(1);
	}
}

int consome_item(void){
	int item;
	item = Buffer[out];
	count++;
	return item;
}

void remocao_item(int item){
	for(int i = 0; i < N; i++){
		if(Buffer[i] == item){
			Buffer[i] = 0;
			out = (out+1)%N;
			ImprimeBuffer();
			return;
		}
	}
}

int produz_item(void){
	int item;
	item = rand()% 16 + 1;
	return item;
}

void insere_item(int item){
	Buffer[in] = item;
	in = (in+1)%N;
	ImprimeBuffer();
}

//inicializa o buffer
void IniciaBuffer(void){

	int i;
	for(i = 0; i < N; i++)
		Buffer[i] = 0;
}

//imprime o buffer
void ImprimeBuffer(void){

	int i;
	for(i = 0; i < N; i++){
		printf("%d ", Buffer[i]);
		if(!(i%11) && i!=0)
			printf("\n");
	}
	printf("\n");
}