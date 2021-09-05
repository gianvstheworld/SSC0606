#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Definição das variaveis que controlam a medição de tempo
clock_t _ini, _fim;

// Definição do tipo booleano
unsigned char typedef bool;
#define TRUE  1
#define FALSE 0

int* ler_inteiros(const char * arquivo, const int n){
    
    FILE* f = fopen(arquivo, "r");

    int * inteiros = (int *) malloc(sizeof(int) * n);

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

int busca_sequencial(int entradas[], int n, int e){
    
    // iterador que conterá a posicao do elemento e procurado, caso seja encontrado 
    int pos; 
    
    // procura o elemento em todas as posições do vetor
    for(pos = 0; pos < n; pos++) 
        // testa se o elemento da posição atual é o elemento procurado de um por um
        if(entradas[pos] == e) 
            // para a busca segunrando a posição pos do elemento
            break;
    
    // retorna a posicao caso o elemento seja encontrado, cc retorna -1
    return (pos == n) ? -1 : pos;
}

int main(int argc, char const *argv[]){
    
    const int N = 50000;
    unsigned encontrados = 0;
    

    int* entradas = ler_inteiros("inteiros_entrada.txt", N);
    int* consultas = ler_inteiros("inteiros_busca.txt", N);

    
    // realiza busca sequencial
    inicia_tempo();
    for (int i = 0; i < N; i++)
        // se a busca for bem sucedida, encontrados é incrementado
        if(busca_sequencial(entradas, N, consultas[i]) != -1) 
            encontrados++;
    
    double tempo_busca = finaliza_tempo();
    
    printf("Tempo de busca    :\t%fs\n", tempo_busca);
    printf("Itens encontrados :\t%d\n", encontrados);
    
    /*
    int entradas[5] = {9, 6, 4, 3, 1};
    int consultas[5] = {10, 6, 4, 0, 1};
    int res;
    for (int i = 0; i < 5; i++){
        res = busca_sequencial(entradas, 5, consultas[i]);
        printf("\nres = %d", res);
        // buscar o elemento consultas[i] na entrada
    }
    */
    return 0;
}