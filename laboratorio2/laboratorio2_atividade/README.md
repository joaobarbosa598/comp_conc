# Relatório resumo - Laboratório 2

João Vitor de Freitas Barbosa - 117055449

O relatório completo se encontra no arquivo pdf nesse mesmo diretório

Foi utilizada a estratégia de linhas intercaladas de acordo com o identificador de cada thread para efetuar a multiplicação das matrizes

A primeira matriz de entrada é feita de apenas 2 e a segunda de apenas 3. 
A matriz de saída é um arquivo txt que possui o nome dado pelo usuário no momento da execução do programa.

Arquivo deve ser executado com os seguintes parâmetros: ./atividade1 (tamanho das matrizes quadradas) (numero de threads) (arquivo Matriz Saida)
  
| **Tempos**    | Sequencial    | 1 thread | 2 threads | 4 threads |
| :-----------: |:-------------:| :-----:  | :------:  | :-------: |
| 500x500       |   0.686       |  0.683   |  0.3501   |  0.275    |
| 1000x1000     |   8.37        |  8.0736  |  4.163    |  2.439    |
| 2000x2000     |   103.54      |  106.29  |  51.67    |  26.461   |

| **Ganhos**    | 1 thread | 2 threads | 4 threads |
| :-----------: | :-----:  | :------:  | :-------: |
| 500x500       |  1.0043  |  1.9594   | 2.4945    |
| 1000x1000     |  1.0367  |  2.0105   |  3.4317   |
| 2000x2000     |  0.9741  |  2.0038   |  3.9129   |


