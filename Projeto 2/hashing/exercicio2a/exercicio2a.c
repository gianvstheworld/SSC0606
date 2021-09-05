/*
============== PROJETO 2 DE ESTRTURUAS DE DADOS 2 ==============
Felipe Andrade Garcia Tommaselli 11800910
Gianluca Capezzuto Sardinha 11876933
Explicacao projeto:  https://ae4.tidia-ae.usp.br/access/content/attachment/61c65473-dfd7-45e0-9c5b-f83baa36b63d/Atividades/31604d0b-fae8-42b8-b950-2f625d6f8035/projeto_2.pdf
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include <math.h>

// Definição das variaveis que controlam a medição de tempo
clock_t _ini, _fim;

// Definição do tipo booleano
typedef unsigned char bool;
#define TRUE 1
#define FALSE 0

// Definição do tipo string
typedef char *string;

#define MAX_STRING_LEN 20

// struct hash com vetor de strings 
typedef struct{
    // vetor com B elementos, sendo B = 150001
    string *vet; 
} hash;

unsigned converter(string s){
    unsigned h = 0;
    for (int i = 0; s[i] != '\0'; i++)
        h = h * 256 + s[i];
    return h;
}

string *ler_strings(const char *arquivo, const int n){
    FILE *f = fopen(arquivo, "r");

    string *strings = (string *)malloc(sizeof(string) * n);

    for (int i = 0; !feof(f); i++){
        strings[i] = (string)malloc(sizeof(char) * MAX_STRING_LEN);
        fscanf(f, "%s\n", strings[i]);
    }

    fclose(f);

    return strings;
}

void inicia_tempo(){
    srand(time(NULL));
    _ini = clock();
}

double finaliza_tempo(){
    _fim = clock();
    return ((double)(_fim - _ini)) / CLOCKS_PER_SEC;
}

unsigned h_div(unsigned x, unsigned i, unsigned B){
    return ((x % B) + i) % B;
}

unsigned h_mul(unsigned x, unsigned i, unsigned B){
    const double A = 0.6180;
    return ((int)((fmod(x * A, 1) * B) + i)) % B;
}

//* FUNÇÕES DESENVOLVIDAS PARA O HASHING

void criar(hash *tabela, unsigned B){
    // aloca dinamicamente o espaço de B de posições do tamanho string para o vetor
    // por utilizar a funcao calloc todas as posicoes sao setadas como NULL
    tabela->vet = (string *) calloc(B, sizeof(string));
}

/* A insercao na tabela hash é basicamente uma insercao comum em um vetor estativo, porem 
calcula-se o indice para ser inserido a partir da funcao hash, dessa forma a busca pode ser
feita de forma mais rapida e precisa. Vale lembrar que caso a posicao que deseja-se armazenar 
o elemento ja esteja ocupada, a tecnica de rehash por overflow progressivo é aplicada, ou seja, 
conforme o iterador i é incrementado a partir das psoicoes ja ocupadas, a funcao hash sofre 
alterações, gerando novas possiveis psoicoes, até alguma ser encontrada para armazenar o elemento
(considerando B suficientemente grande)*/
int inserir(hash *tabela, unsigned B, string e, unsigned (*fHash)(unsigned, unsigned, unsigned)){
    
    // declaracao de variaveis para uso posterior
    unsigned pos, i, aux;
    
    // converter o elemento e para o inteiro aux
    aux = converter(e);

    // incrementa i para cada tentativa de hash não sucedidade, ou seja,
    // se i > 0, houve ao menos um rehash para achar uma posicao para o elemento e
    // sendo e a string enviada pelo insercoes[i] para ser inserida no vetor
    for(i = 0; i < B; i++){
        // calcula a posicao na funcao hash de acordo com a funcao hash passada,
        // pode ser o hash por multiplcação ou por divisão
        pos = fHash(aux, i, B);
        // caso que a posicao esta vazia (pode receber o elemento)
        if(tabela->vet[pos] == NULL){
            // aloca-se um espaço para a string (tamanho max de MAX_STRING_LEN)
            tabela->vet[pos] = (string) calloc(MAX_STRING_LEN, sizeof(char));
            // copia o elemento na tabela de acordo com a posicao calculada
            strcpy(tabela->vet[pos], e);
            // retorna i para verificar se houve rehash (incrementar colisões)
            return i;
        }
        // caso que tenta-se inserir elementos repetidos
        // pelo conjunto de dados fornecidos esse caso não deve ocorrer 
        if(!strcmp(tabela->vet[pos], e))
            return -1;
    }
    // erro na insercao
    return -1;
}

/* A busca no hashing fechado constitui basicamente de uma procura quase que certeira,
a partir do calculo da posição pela funcao hash, pode-se procurar diretamente na posicao 
que espera-se que o elemento esteja armazenado, caso haja rehash na insercao, a busca 
acompanha os passos feitos pela insercao para chegar na posicao que o elemento deve estar
localizado, caso ele esteja mesma na tabela (caso contrario retorna-se -1) */
int buscar(hash *tabela, unsigned B, string e, unsigned (*fHash)(unsigned, unsigned, unsigned)){
    
    // declaracao de variaveis para uso posterior
    unsigned pos, i, aux;
    
    // converter o elemento e para o inteiro aux
    aux = converter(e);

    // incrementa i para cada tentativa de achar a posicao no hash não sucedida
    // ou seja, cada vez que houve rehash na insercao, a busca deve acompanhar os passos
    // sendo e a string enviada pelo consultas[i] para ser inserida no vetor
    for (i = 0; i < B; i++){
        // calcula a posicao na funcao hash de acordo com a funcao hash passada,
        // pode ser o hash por multiplcação ou por divisão
        pos = fHash(aux, i, B);
        // caso que mesmo apos os rehash chegou-se numa posicao vazia, elemento nao encontrado
        if(tabela->vet[pos] == NULL)
            return -1;
        // elemento encontrado
        if((strcmp(tabela->vet[pos], e)) == 0)
            return pos;
    }
    // erro na busca
    return -1;
}

void limpar(hash *tabela, unsigned B){
    int i;

    // varre todas as posições liberando o espaço alocado um por um
    for(i = 0; i < B; i++)
        free(tabela->vet[i]);

    //libera a "cabeça" da tabela (ponteiro para o encadeamento de elementos do vetor)
    free(tabela->vet);
}

int main(int argc, char const *argv[]){
    //DECLARAÇÕES E PRÉ DEFINIÇÕES

    unsigned N = 50000;
    unsigned M = 70000;
    unsigned B = 150001;

    unsigned colisoes_h_div = 0;
    unsigned colisoes_h_mul = 0;

    unsigned encontrados_h_div = 0;
    unsigned encontrados_h_mul = 0;

    string* insercoes = ler_strings("strings_entrada.txt", N);
    string* consultas = ler_strings("strings_busca.txt", M);

    // HASHING FECHADO

    // cria uma tabela do tipo hash (com um vetor de tamanho B)
    hash tabela;

    //* HASH POR DIVISÃO

    // cria tabela hash com hash por divisão
    criar(&tabela, B);

    // inserção dos dados na tabela hash usando hash por divisão
    
    // inicia contagem do tempo
    inicia_tempo();
    // roda o vetor de entradas (tamanho N)
    for (int i = 0; i < N; i++)
        // inserir insercoes[i] na tabela hash utilizando hash por divisao
        // caso a funcao insserir retorne i > 0, houve ao menos uma colisão  
        if(inserir(&tabela, B, insercoes[i], &h_div) > 0)
            colisoes_h_div++;    
    
    // finaliza contagem de tempo
    double tempo_insercao_h_div = finaliza_tempo();

    // consulta dos dados na tabela hash usando hash por divisão

    // inicia contagem do tempo
    inicia_tempo();
    // roda o vetor de consultas para a busca (tamanho M)
    for (int i = 0; i < M; i++)
        // busca consultar[i] na tabela hash
        // caso retorne -1 elemento não encontrado, cc elemento encontrado
        if(buscar(&tabela, B, consultas[i], &h_div) != -1)
            encontrados_h_div++;

    // finaliza contagem de tempo
    double tempo_busca_h_div = finaliza_tempo();
        
    // limpa e libera o espaço de memória da tabela hash por divisão
    limpar(&tabela, B);
    
    //* HASH POR MULTIPLICAÇÃO

    // cria tabela hash com hash por multiplicaçao
    criar(&tabela, B);

    // inserção dos dados na tabela hash usando hash por multiplicação

    // inicia contagem do tempo
    inicia_tempo();
    // roda o vetor de entradas (tamanho N)
    for (int i = 0; i < N; i++)
        // inserir insercoes[i] na tabela hash utilizando hash por multiplicacao
        // caso a funcao insserir retorne i > 0, houve ao menos uma colisão  
        if(inserir(&tabela, B, insercoes[i], &h_mul) > 0) 
            colisoes_h_mul++;
    
    // finaliza contagem de tempo
    double tempo_insercao_h_mul = finaliza_tempo();


    // busca dos dados na tabela hash com hash por multiplicação
    
    // inicia contagem do tempo
    inicia_tempo();

    // roda o vetor de consultas para a busca (tamanho M)
    for (int i = 0; i < M; i++)
        // busca consultar[i] na tabela hash
        // caso retorne -1 elemento não encontrado, cc elemento encontrado
        if(buscar(&tabela, B, consultas[i], &h_mul) != -1) // buscar consultas[i] na tabela hash
            encontrados_h_mul++;
    
    // finaliza contagem de tempo
    double tempo_busca_h_mul = finaliza_tempo();

    // limpa e libera o espaço de memória da tabela hash por multiplicacao
    limpar(&tabela, B);

    // liberacao do espaco de memoria alocado do vetor de insercoes e consultas
    free(insercoes);
    free(consultas);

    // OUTPUTS DOS DADOS OBTIDOS

    printf("Hash por Divisão\n");
    printf("Colisões na inserção: %d\n", colisoes_h_div);
    printf("Tempo de inserção   : %fs\n", tempo_insercao_h_div);
    printf("Tempo de busca      : %fs\n", tempo_busca_h_div);
    printf("Itens encontrados   : %d\n", encontrados_h_div);
    printf("\n");
    printf("Hash por Multiplicação\n");
    printf("Colisões na inserção: %d\n", colisoes_h_mul);
    printf("Tempo de inserção   : %fs\n", tempo_insercao_h_mul);
    printf("Tempo de busca      : %fs\n", tempo_busca_h_mul);
    printf("Itens encontrados   : %d\n", encontrados_h_mul);

    return 0; // sucesso
}