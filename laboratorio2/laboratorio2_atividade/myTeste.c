#include <stdio.h>
#include <stdlib.h>

int teste(void){
	return 2;
}

int main (){

	teste() == 1 ? printf("Sim\n") : printf("Nao\n"), printf("Nao2\n");;
	return 0;
}