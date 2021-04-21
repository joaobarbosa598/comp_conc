/*Versao sequencial do trabalho 1 de computacao concorrente
Calculo de integrais definidas
Nome: Joao Vitor de Freitas*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "timer.h"

double funcao(double x);
//quanto maior o N, maior a precisao
long int N;

int main(int argc, char **argv) 
{
    double i, a, b, x, y, sum = 0;
    double inicio, fim, delta1, delta2; //variaveis para medir o tempo de execucao
    
    //-------------------------------------------------------------//
    GET_TIME(inicio);

    //recebe e valida os parametros de entrada (dimensao do vetor, numero de threads)
    if(argc < 4){
        fprintf(stderr, "Digite %s <a> <b> <quantos calculos serao feitos N>\n", argv[0]);
        return 1;
    }

    a = atof(argv[1]);
    b = atof(argv[2]);
    N = atol(argv[3]);

    if (a > b) 
    {
        i = a;
        a = b;
        b = i;
    }


    GET_TIME(fim);

    //calcula o tempo gasto com as inicializacoes
    delta1 = fim - inicio;

    //-----------------------------------------------------------------------------------
    GET_TIME(inicio);

    for (i = a; i < b; i += (b - a) / N) 
    {
        x=i;
        y = funcao(x);
        sum += y * (b - a) / N;
    }


    GET_TIME(fim);

    //calcula o tempo gasto com a parte concorrente (calculo da integral)
    delta2 = fim - inicio;

    printf("O valor da integração é: %.15lf", sum);
    printf("\n");

    //exibe os tempos gastos em cada parte do programa 
    printf("Tempo inicializacoes: %.8lf\n", delta1);
    printf("Tempo do calculo da integral: %.8lf\n", delta2);
    return 0;
}


double funcao(double x){
    return cos(5*x);
}
