#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Definição das variaveis que controlam a medição de tempo
clock_t _ini, _fim;

// indice da bsuca sequencial indexada
typedef struct{
    int kindex;
    int pos;
} index;

// Definição do tipo booleano
unsigned char typedef bool;
#define TRUE  1
#define FALSE 0

int* ler_inteiros(const char * arquivo, const int n){
    FILE* f = fopen(arquivo, "r");

    int* inteiros = (int *) malloc(sizeof(int) * n);

    for (int i = 0; !feof(f); i++)
        fscanf(f, "%d\n", &inteiros[i]);

    fclose(f);

    return inteiros;
}

void inicia_tempo(){
    srand(time(NULL));
    _ini = clock();
}

double finaliza_tempo(){
    _fim = clock();
    return ((double) (_fim - _ini)) / CLOCKS_PER_SEC;
}

void ordena_entrada(int entradas[], long ini, long fim){    
    // Quicksort com particionamento de Lomuto
    
    long i, j;
    int aux;

    // particionamento
    i = ini;
    for(j = ini; j < fim; j++){
        // compara com o pivo
        if(entradas[j] < entradas[fim]){
            //troca elementos das pos i por j
            aux = entradas[j];
            entradas[j] = entradas[i];
            entradas[i] = aux;
            i++; // achou novo elemento menor que o pivo
        }
    }
    // reposiciona o pivo, ou seja, troca elementos das posicoes i e fim
    aux = entradas[fim];
    entradas[fim] = entradas[i];
    entradas[i] = aux;

    // ordenada as duas metades de maneira independente
    if (ini < i - 1)
        ordena_entrada(entradas, ini, i - 1);
    if (i + 1 < fim)
        ordena_entrada(entradas, i + 1, fim);
}

int busca_sequencial_index(int entradas[], index tabela_index[], int n, int n_index, int e){

    int i; // iterador que marca a posição do elemento procurado

    // posiciona i no intervalo que e esta contido na tabela de indice 
    for(i = 0; i < n && e > tabela_index[i].kindex; i++); 
    
    // caso elemento buscado menor ou igual ao primeiro
    if(i == 0)
        if(e == tabela_index[i].kindex)
            return i;

    // varre a entrada a partir do posicionamento de i feito pela comparacao com a
    // tabela de indices, ate achar e, ou acabar o vetor 
    for(i = tabela_index[i - 1].pos; i < n && e > entradas[i]; i++);
    
    // caso que o elemento foi encontrado, retorna a sua posição i
    if(i < n && e == entradas[i])
        return i;

    // elemento não encontrado
    return -1;
}


int main(int argc, char const *argv[]){

    const int N = 50000;
    const int index_size = 10000;
    unsigned encontrados = 0;

    
    int* entradas = ler_inteiros("inteiros_entrada.txt", N);
    int* consultas = ler_inteiros("inteiros_busca.txt", N);

    // declara a tabela de index com o tamanho estipulado: (N/index_size)
    index tabela_index[N/index_size];

    // ordenar entrada
    ordena_entrada(entradas, 0, N - 1);

    // criacao da tabela de indices
    // iteracoes ate o tamanho estipulado para a tabela de indices: (N/index_size)
    for(int i = 0; i < (N/index_size); i++){
        // identifica e guarda a posicao i
        tabela_index[i].pos = i*(index_size); //i = 0,1,2 ...     
        // guarda o valor na tabela de indice
        tabela_index[i].kindex = entradas[tabela_index[i].pos];
    }
    
    // realizar consultas na tabela de indices 
    inicia_tempo();
    for (int i = 0; i < N; i++) {
        // se a busca for bem sucedida, encontrados é incrementado
        if(busca_sequencial_index(entradas, tabela_index, N, N/index_size, consultas[i]) != -1)
            encontrados++;
    }
    double tempo_busca = finaliza_tempo();

    printf("Tempo de busca    :\t%fs\n", tempo_busca);
    printf("Itens encontrados :\t%d\n", encontrados);

    return 0;
}