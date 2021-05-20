/*Nome: João Vitor de Freitas Barbosa
DRE: 117055449
Laboratorio 7*/

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define NTHREADS 4

int desbloquiaT4 = 0;
sem_t condt1, condt2;     //semaforos para sincronizar a ordem de execucao das threads

void * t1(void * args);
void * t2(void * args);
void * t3(void * args);
void * t4(void * args);

//funcao principal
int main(int argc, char *argv[]) {
  pthread_t tid[NTHREADS];
  int t;

  //inicia os semaforos
  sem_init(&condt1, 0, 0);
  sem_init(&condt2, 0, 0);

  //cria as quatro threads
  if (pthread_create(&tid[0], NULL, t1, NULL)) { printf("--ERRO: pthread_create()\n"); exit(-1); }
  if (pthread_create(&tid[1], NULL, t3, NULL)) { printf("--ERRO: pthread_create()\n"); exit(-1); }
  if (pthread_create(&tid[2], NULL, t2, NULL)) { printf("--ERRO: pthread_create()\n"); exit(-1); }
  if (pthread_create(&tid[3], NULL, t4, NULL)) { printf("--ERRO: pthread_create()\n"); exit(-1); }

  //--espera todas as threads terminarem
  for (t=0; t<NTHREADS; t++) {
    if (pthread_join(tid[t], NULL)) {
         printf("--ERRO: pthread_join() \n"); exit(-1); 
    }
  }

  sem_destroy(&condt1);
  sem_destroy(&condt2);
  pthread_exit(NULL);
}

//funcao executada pela thread 1
void *t1 (void * args) {

  sem_wait(&condt2); //espera T2 e T3 executarem
  printf("Volte sempre!\n");

  pthread_exit(NULL);
}

//funcao executada pela thread 2
void *t2 (void * args) {

  sem_wait(&condt1); //espera T4 executar
  printf("Fique a vontade.\n");
  desbloquiaT4++;  
  sem_post(&condt1); //permite que T3 execute

  if(desbloquiaT4==2)
    sem_post(&condt2); //permite que T1 execute caso T3 ja tenha executado

  pthread_exit(NULL);
}

//funcao executada pela thread 3
void *t3 (void * args) {

  sem_wait(&condt1); //espera T4 executar
  printf("Sente-se por favor.\n");
  desbloquiaT4++;
  sem_post(&condt1); //permite que T2 execute
 
  if(desbloquiaT4==2)
    sem_post(&condt2); //permite que T1 execute caso T2 já tenha executado

  pthread_exit(NULL);
}

//funcao executada pela thread 4
void *t4 (void * args) {

  printf("Seja bem-vindo!\n");
  sem_post(&condt1); //permite que T3 ou T4 executem

  pthread_exit(NULL);
}