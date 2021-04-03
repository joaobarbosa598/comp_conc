#include <stdio.h>
#include <stdlib.h>

int main (void){
	int i = 10;
	int *p;

	*p = i;
	printf("&i:%p\n", &i);
	printf("p:%p\n", p); //o endereco armazenado em p
	printf("&p:%p\n", &p); //o endereco que armazena p
	printf("*p:%d\n", *p); //o valor que o endereco armazenado em p aponta
	printf("\n");
	
	p = &i;
	printf("&i:%p\n", &i);
	printf("p:%p\n", p); //o endereco armazenado em p
	printf("&p:%p\n", &p); //o endereco que armazena p
	printf("*p:%d\n", *p); //o valor que o endereco armazenado em p aponta

	return 0;
}