/* Disciplina: Computacao Concorrente */
/* Nome: Joao Vitor de Freitas */
/* Laboratório: 2 */
/* Codigo: Multiplica uma matriz por uma matriz atraves de arquivos*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"
#include <unistd.h> //para usar a funcao "sysconf"


//variaveis globais
float *matA; //matriz de entrada
float *matX; //segunda matriz de entrada
float *matB; //matriz de saida
int linhas, colunas, linhas1, colunas1; //dimensoes da matriz de entrada
int threads; //numero de threads desejadas

//funcao que multiplica matriz por vetor (A * X = B)
//entrada: matriz de entrada, vetor de entrada, vetor de saida, dimensoes da matriz
//requisito 1: o numero de colunas da matriz eh igual ao numero de elementos do vetor de entrada
//requisito 2: o numero de linhas da matriz eh igual ao numero de elementos do vetor de saida
/*void multiplicaMatrizVetor(const float *a, const float *x, float *b, int linhas, int colunas) {
   int i, j;
   for (i=0; i<linhas; i++) {
      b[i] = 0;
      for (j=0; j<colunas; j++) {
         b[i] += a[i*colunas+j] * x[j];
      }
   }
}*/
//nesse tipo de acesso a memoria de ponteiro para declarar a matriz, estamos fazendo apenas 2 requisitos para ela:
//1 para criar e alocar espaço para a variavel ponteiro e depois 1 para alocar memoria para o endereco que esta no ponteiro.
//no tipo de ponteiro para ponteiro para declarar matriz, fazemos 4 requisitos a memoria:
//2 pra criar a variavel ponteiro de ponteiro e depois 2 pra alocar na memoria 
void *multiplicaMatrizMatriz(void *tid)
{
   int td = *(int *) tid;
   int i, j, k;

   for(i = td; i < linhas; i+=threads)
   {
   		//vetB[i]  = 0;
		for(k = 0; k < colunas1; k++)
		{
			for(j = 0; j < colunas; j++)
			{
			 	matB[i*colunas1+k] += matA[i*colunas+j] * matX[j*colunas1+k];
			}
		}
   }
   //printf("thread %d chegou aqui\n", td);
   free(tid);
   pthread_exit(NULL);
}

//funcao que aloca espaco para uma matriz e preenche seus valores
//entrada: matriz de entrada, dimensoes da matriz
//saida: retorna 1 se a matriz foi preenchida com sucesso e 0 caso contrario
int preencheMatriz(float **mat, int linhas, int colunas, FILE *arq) {
   int i, j;
   //aloca espaco de memoria para a matriz
   *mat = (float*) malloc(sizeof(float) * linhas * colunas);
   if (*mat == NULL) return 0;
   //preenche o vetor
   for (i=0; i<linhas; i++) 
   {
		for (j=0; j<colunas; j++) 
		{
		 	fscanf(arq, "%f", (*mat) + (i*colunas+j));
		}
   }
   return 1;
}

//funcao que imprime uma matriz
//entrada: matriz de entrada, dimensoes da matriz
//saida: matriz impressa na tela
void imprimeMatriz(float *mat, int linhas, int colunas, FILE *arq) {
   int i, j;
   for (i=0; i<linhas; i++) {
      for (j=0; j<colunas; j++) {
         fprintf(arq, "%.1f ", mat[i*colunas+j]);
      }
      fprintf(arq, "\n");
   }
}

//funcao que aloca espaco para um vetor e preenche seus valores
//entrada: vetor de entrada, dimensao do vetor
//saida: retorna 1 se o vetor foi preenchido com sucesso e 0 caso contrario
int preencheVetor(float **vet, int dim, FILE *arq) {
   int i;
   //aloca espaco de memoria para o vetor
   *vet = (float*) malloc(sizeof(float) * dim);
   if (*vet == NULL) return 0;
   //preenche o vetor
   for (i=0; i<dim; i++) {
       //*( (*vet)+i ) = 1.0;
       fscanf(arq, "%f", (*vet) + i);
   }
   return 1;
}

//funcao que imprime um vetor
//entrada: vetor de entrada, dimensao do vetor
//saida: vetor impresso na tela
void imprimeVetor(float *vet, int dim, FILE *arq) {
   int i;
   for (i=0; i<dim; i++) {
      fprintf(arq, "%.1f ", vet[i]);
   }
   fprintf(arq, "\n");
}

//funcao principal
int main(int argc, char *argv[]) {
   
   FILE *arqA, *arqX, *arqB; //arquivos dos dados de entrada e saida

   double inicio, fim, delta1, delta2, delta3; //variaveis para medir o tempo de execucao
   //descobre quantos processadores a maquina possui
   int numCPU = sysconf(_SC_NPROCESSORS_ONLN); 
   printf("Numero de processadores: %d\n", numCPU);

   //-----------------------------------------------------------------------------------
   GET_TIME(inicio); 

   //le e valida os parametros de entrada
   //o arquivo da matriz de entrada deve conter na primeira linha as dimensoes da matriz (linha coluna) seguido dos elementos da matriz separados por espaco
   //o arquivo do vetor de entrada deve conter na primeira linha a dimensao do vetor seguido dos elementos separados por espaco
   if(argc < 4) {
      fprintf(stderr, "Digite: %s <arquivo matriz A> <arquivo matriz X> <arquivo matriz B>.\n", argv[0]);
      return 1;
   }
   //abre o arquivo da matriz de entrada
   arqA = fopen(argv[1], "r");
   if(arqA == NULL) {
      fprintf(stderr, "Erro ao abrir o arquivo da matriz de entrada.\n");
      return 1;
   }
   //le as dimensoes da matriz de entrada
   fscanf(arqA, "%d", &linhas);
   fscanf(arqA, "%d", &colunas);

   //abre o arquivo da segunda matriz de entrada
   arqX = fopen(argv[2], "r");
   if(arqX == NULL) {
      fprintf(stderr, "Erro ao abrir o arquivo da segunda matriz de entrada.\n");
      return 1;;
   }
   //le as dimensoes da segunda matriz de entrada
   fscanf(arqX, "%d", &linhas1);
   fscanf(arqX, "%d", &colunas1);

   //valida as dimensoes da matriz e vetor de entrada
   if(colunas != linhas1) {
      fprintf(stderr, "Erro: as dimensoes da primeira e da segunda matriz nao sao compativeis.\n");
      return 1;
   }

   //abre o arquivo da matriz de saida
   arqB = fopen(argv[3], "w");
   if(arqB == NULL) {
      fprintf(stderr, "Erro ao abrir o arquivo da matriz de saida.\n");
      return 1;
   }

   //aloca e preenche a matriz de entrada
   if(preencheMatriz(&matA, linhas, colunas, arqA) == 0) {
      fprintf(stderr, "Erro de preenchimento da matriz de entrada\n");
      return 1;
   }
   //aloca e preenche a segunda matriz de entrada
   if(preencheMatriz(&matX, linhas1, colunas1, arqX) == 0) {
      fprintf(stderr, "Erro de preenchimento da matriz de entrada\n");
      return 1;
   }
   /*
   //aloca e preenche o vetor de entrada
   if(preencheVetor(&vetX, dim, arqX) == 0) {
      fprintf(stderr, "Erro de preenchimento do vetor de entrada\n");
      return 1;
   }*/
   //aloca a matriz de saida
   matB = (float*) malloc(sizeof(float) * linhas * colunas1);
   if(matB==NULL) {
      fprintf(stderr, "Erro de alocacao da matriz de saida\n");
      return 1;
   }
   GET_TIME(fim);

   //calcula o tempo gasto com as inicializacoes
   delta1 = fim - inicio;

   /*
   //imprime a matriz de entrada
   printf("Matriz de entrada:\n");
   imprimeMatriz(matA, linhas, colunas, stdout);
   //imprime o vetor de entrada
   printf("Vetor de entrada:\n");
   imprimeVetor(vetX, colunas, stdout);
   */
   pthread_t *tid_sistema;
   int *tid;
   int t;

   printf("Digite o numero de threads desejadas:\n");
   scanf("%d", &threads);

   if(threads>linhas1) threads = linhas1;

   tid_sistema = (pthread_t *) malloc(sizeof(pthread_t) * threads);
   if(tid_sistema == NULL)
   {
      printf("--ERRO: malloc()\n");
      exit(-1);
   }


   //-----------------------------------------------------------------------------------
   GET_TIME(inicio);


   for(t=0; t<threads; t++) 
   {
      tid = malloc(sizeof(int)); 
      if(tid==NULL) 
      { 
         printf("--ERRO: malloc()\n"); exit(-1); 
      }
      *tid = t;

      if (pthread_create(&tid_sistema[t], NULL, multiplicaMatrizMatriz, (void*) tid))
      {
         printf("--ERRO: pthread_create()\n"); exit(-1);
      }
   }
   //espera todas as threads terminarem e imprime o vetor de saída
   for(t=0; t<threads; t++) 
   {
     if (pthread_join(tid_sistema[t], NULL)) 
     {
         printf("--ERRO: pthread_join()\n"); 
         exit(-1);
     }
   }


   GET_TIME(fim);

   //calcula o tempo gasto com a parte concorrente (incremento do vetor)
   delta2 = fim - inicio;
   
   //multiplica a matriz de entrada pelo vetor de entrada
   //multiplicaMatrizVetor(matA, vetX, vetB, linhas, colunas);
   
   //salva o vetor de saida no arquivo de saida
   imprimeMatriz(matB, linhas, colunas1, arqB);
   //imprimeVetor(vetB, linhas, arqB);


   //-----------------------------------------------------------------------------------
   GET_TIME(inicio);
   //libera os espacos de memoria alocados
   free(matA);   
   free(matX);   
   free(matB); 

   GET_TIME(fim);

   //calcula o tempo gasto com as finalizacoes 
   delta3 = fim - inicio;
   //exibe os tempos gastos em cada parte do programa 
   printf("Tempo inicializacoes: %.8lf\n", delta1);
   printf("Tempo multiplicacao matriz e matriz com %d threads: %.8lf\n", threads, delta2);
   printf("Tempo finalizacoes: %.8lf\n", delta3);

   return 0;
}

