# Relatório - Laboratório 1

**João Vitor de Freitas Barbosa - 117055449**

#### Este trabalho consiste em um programa concorrente, com duas threads(além da pricipal), para incrementar de 1 cada elemento de um vetor de 10000 elementos.

A estratégia utilizada para distribuição das tarefas entre as threads foi a intercalação dos elementos

Por exemplo, a thread com **id 1** começa iterando na posição 0 do vetor e posteriormente todas as posições pares: _{0,2,4,6,8,...}_  

A thread com **id 2** começa iterando na posição 1 do vetor e posteriormente todas as posições ímpares: _{1,3,5,7,9,...}_


##### Primeiramente fazemos a impressão do vetor inicializado com zero
> 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 

##### Logo após exibimos quais threads estão sendo executadas
> Ola, a thread 1 esta sendo executada e ira comecar incrementando na posicao 0 do vetor  

> Ola, a thread 2 esta sendo executada e ira comecar incrementando na posicao 1 do vetor

##### E por fim exibimos o resultado final do vetor
> 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 
