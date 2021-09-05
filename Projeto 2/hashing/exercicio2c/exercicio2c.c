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
#define TRUE  1
#define FALSE 0

// Definição do tipo string
typedef char * string;

#define MAX_STRING_LEN 20

// struct no do tipo No, contendo informações do nó
typedef struct no{
    char info[MAX_STRING_LEN];
    struct no *prox;
} No;

// struct no do tipo Lista, formando o hash
typedef struct no *Lista;

// struct do tipo hash com vetor de strings 
typedef struct{
    // vetor do tipo lista com B elementos, sendo B = 150001
    Lista *vet;
} hash;

unsigned converter(string s) {
    unsigned h = 0;
    for (int i = 0; s[i] != '\0'; i++) 
        h = h * 256 + s[i];
    return h;
}

string* ler_strings(const char * arquivo, const int n)
{
    FILE* f = fopen(arquivo, "r");
    
    string* strings = (string *) malloc(sizeof(string) * n);

    for (int i = 0; !feof(f); i++) {
        strings[i] = (string) malloc(sizeof(char) * MAX_STRING_LEN);
        fscanf(f, "%s\n", strings[i]);
    }

    fclose(f);

    return strings;
}

void inicia_tempo()
{
    srand(time(NULL));
    _ini = clock();
}

double finaliza_tempo()
{
    _fim = clock();
    return ((double) (_fim - _ini)) / CLOCKS_PER_SEC;
}

unsigned h_div(unsigned x, unsigned B)
{
    return x % B;
}

unsigned h_mul(unsigned x, unsigned B)
{
    const double A = 0.6180;
    return fmod(x * A, 1) * B;
}

// cria os nos da lista encadeada, sendo uma função do tipo lista
Lista *criar_no(unsigned B){

    // aloca dinamicamente o espaço de B posições do tamanho string para a Lista de elementos
    Lista *elementos = (Lista *) calloc(B, sizeof(Lista));

    // retorna os elementos da lista
    return elementos;
}

// insere cada elemento na lista, formando a lista encadeada 
// para funcionar de acordo com o hashing aberto
int inserir_no(Lista *elementos, string e){
    
    // define colisao
    int colisao = 0;
    
    // cria um ponteiro para um novo no do tipo no
    // contendo as strings que deseja-se inserir
    No *novoNo = (No *) calloc(MAX_STRING_LEN ,sizeof(No));

    // caso os elementos da tabela nao estejam NULL
    // haverá colisao, tomaremos colisao como 1
    if (*elementos != NULL)
        colisao = 1;

    // copia a string que será inserida para o novo no dos elementos
    strcpy(novoNo->info, e);
    // troca de atribuições entre o prox novo no e os elementos a serem
    // inseridos na lista de elementos
    novoNo->prox = *elementos;
    *elementos = novoNo;

    // retorna colisao para a contabilização no hashing
    return colisao;
}

// busca o nó que é igual a string a ser buscada no conjunto de elementos 
int buscar_no(No *conjunto, string e){
    

    while(conjunto != NULL){
        // caso os elementos sejam iguais retorna 0
        if(strcmp(conjunto->info, e) == 0)
            return 0;
        // caso o elementos ainda não foi encontrado, busca recursivamente
        return buscar_no(conjunto->prox, e);
    }

    // caso o elemento não seja encontrado, retorna -1
    return -1;
}

// funçao do tipo hash que aponta para criar_hash, criando a tabela para o hashing
hash *criar_hash(hash *tabela, unsigned B){

    // aloca dinamicamente um espaço 100 posições para o hashing
    tabela->vet = calloc(1 ,sizeof(hash));
    // tabela hashing atribui os nos da lista de elementos
    tabela->vet = criar_no(B);

    // retorna tabela para busca e inserção do hashing
    return tabela;
}

/* A busca no hashing aberto constitui basicamente de uma procura certeira,
a partir do calculo da posição pela funcao hash, pode-se procurar diretamente na posicao 
que espera-se que o elemento esteja armazenado, caso a posição da tabela estela ocupada
com uma lista que o primeiro elemento nao é igual ao buscado, a busca 
acompanha os passos feitos pela lista encadeada para chegar na posicao que o elemento deve estar
localizado, caso ele esteja mesmo na tabela */
int buscar_hash(hash *tabela, unsigned B, string e, unsigned (*fHash)(unsigned, unsigned)){

    // declaracao de variaveis para uso posterior
    unsigned pos, aux;

    // converter o elemento e para o inteiro aux
    aux = converter(e);
    pos = fHash(aux, B);

    // caso a busca na posição calculada pelo hashing seja igual ao elemento
    // retorna 1 e contabiliza o número de elementos encontrados
    if(buscar_no(tabela->vet[pos], e) == 0)
        return 1;
    
    return 0;
}

/* A insercao na tabela hash é basicamente uma insercao comum em um vetor, porem 
calcula-se o indice para ser inserido a partir da funcao hash, dessa forma a busca pode ser
feita de forma mais rapida e precisa. Vale lembrar que caso a posicao que deseja-se armazenar 
o elemento ja esteja ocupada, a tecnica para o hashing aberto funnciona da mesma forma
que a busca em hash */
int inserir_hash(hash *tabela, unsigned B, string e, unsigned (*fHash)(unsigned, unsigned)){
    
    // declaracao de variaveis para uso posterior
    unsigned pos, aux;

    // converter o elemento e para o inteiro aux
    aux = converter(e);
    pos = fHash(aux, B);

    // verifica se ao inserir na lista de elementos haverá colisao
    // ou na busca da mesma haver elementos repetidos
    if(buscar_no(tabela->vet[pos], e) == 0 || inserir_no(&tabela->vet[pos], e) == 0)
        return 0;

    return 1; // não ocorre colisao 
}

void limpar_hash(hash *tabela, unsigned B){

    int i;

    // varre todas as posições liberando o espaço alocado um por um
    for (i= 0; i < B; i++)
        free(tabela->vet[i]);        

    // libera a "cabeça" da tabela (ponteiro para o encadeamento de elementos do vetor)
    free(tabela->vet);
}

int main(int argc, char const *argv[]){
    //DECLARAÇÕES E PRÉ DEFINIÇÕES

    const int N = 50000;
    const int M = 70000;
    const int B = 150001;

    unsigned colisoes_h_div = 0;
    unsigned colisoes_h_mul = 0;

    unsigned encontrados_h_div = 0;
    unsigned encontrados_h_mul = 0;

    string* insercoes = ler_strings("strings_entrada.txt", N);
    string* consultas = ler_strings("strings_busca.txt", M);

    //* HASHING ABERTO

    // cria uma tabela do tipo hash (com um vetor de tamanho B)
    hash tabela;

    // cria tabela hash com hash por divisão
    criar_hash(&tabela ,B);

    // inserção dos dados na tabela hash usando hash por divisão

    // inicia contagem do tempo
    inicia_tempo();
    // roda o vetor de entradas (tamanho N)
    for (int i = 0; i < N; i++)
        // inserir insercoes[i] na tabela hash utilizando hash por divisao
        // caso a funcao inserir retorne 0, houve ao menos uma colisão
        if(inserir_hash(&tabela, B, insercoes[i], &h_div))
            colisoes_h_div++;
    // finaliza contagem do tempo
    double tempo_insercao_h_div = finaliza_tempo();

    // consulta dos dados na tabela hash usando hash por divisão

    // inicia a contagem do tempo
    inicia_tempo();
    // roda o vetor de consultas para a busca (tamanho M)
    for (int i = 0; i < M; i++)
        // buscar consultas[i] na tabela hash
        // caso retorne 1 o elemento foi encontrado
        if (buscar_hash(&tabela, B, consultas[i], &h_div))
            encontrados_h_div++;
    // finaliza contagem de tempo
    double tempo_busca_h_div = finaliza_tempo();

    // limpa e libera o espaço de memória da tabela hash por divisão
    limpar_hash(&tabela, B);

    //* HASH POR MULTIPLICAÇÃO

    // cria tabela hash com hash por multiplicação
    criar_hash(&tabela, B);

    // inserção dos dados na tabela hash com hash por multiplicação

    // inicia contagem do tempo
    inicia_tempo();
    // roda o vetor de entradas (tamanho N)
    for (int i = 0; i < N; i++)
        // inserir insercoes[i] na tabela hash utilizando hash por multiplicação
        // caso a funcao inserir retorne 0, houve ao menos uma colisão
        if (inserir_hash(&tabela, B, insercoes[i], &h_mul))
            colisoes_h_mul++;
    // finaliza contagem de tempo
    double tempo_insercao_h_mul = finaliza_tempo();

    // busca dos dados na tabela hash com hash por multiplicação

    // inicia contagem do tempo
    inicia_tempo();
    // roda o vetor de consultas para a busca (tamanho M)
    for (int i = 0; i < M; i++)
        // buscar consultas[i] na tabela hash
        // caso retorne 1 o elemento foi encontrado
        if (buscar_hash(&tabela, B, consultas[i], &h_mul))
            encontrados_h_mul++;
    // finaliza_tempo
    double tempo_busca_h_mul = finaliza_tempo();

    // limpa e libera o espaço de memória da tabela hash por multiplicação
    limpar_hash(&tabela, B);

    // liberação do espaço de memória alocado do vetor de insercoes e consultas
    free(insercoes);
    free(consultas);

    // OUTPUTS DOS DADOS OBTIDOS

    printf("\nHash por Divisão\n");
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