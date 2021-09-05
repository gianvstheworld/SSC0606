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

/* A funcão hash descrita funciona a partir de um conceito parecido com o do overflow
progressivo, no qual a cada vez que o i é incrementado a funcao hash sofre alterações
nesse caso:
funcao hash primario: (h_mul(x, 0, B))
funcao hash secundario, p/ i = 2: (h_mul(x, 0, B) + 2 * h_div(x, 0, B)) % B
funcao hash secundario, p/ i = 3: (h_mul(x, 0, B) + 3 * h_div(x, 0, B)) % B
... */
#define h(x, i, B) (h_mul(x, 0, B) + i * h_div(x, 0, B)) % B

// Definição das variaveis que controlam a medição de tempo
clock_t _ini, _fim;

// Definição do tipo booleano
typedef unsigned char bool;
#define TRUE 1
#define FALSE 0

// Definição do tipo string
typedef char *string;

#define MAX_STRING_LEN 20

// struct com vetor de strings 
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

/* A insercao na tabela hash é basicamente uma insercao comum em um vetor estatico, porem 
calcula-se o indice para ser inserido a partir da funcao hash, dessa forma a busca pode ser
feita de forma mais rapida e precisa. Vale lembrar que caso a posicao que deseja-se armazenar 
o elemento ja esteja ocupada, a tecnica de rehash por hashing duplo é aplicada, ou seja, 
conforme o iterador i é incrementado a partir das psoicoes ja ocupadas, a funcao hash sofre 
alterações, gerando novas possiveis psoicoes, até alguma ser encontrada para armazenar o elemento,
para o caso default, onde i = 0, a funcao é bem diferenca dos casos i > 0, por isso pode-se dizer
que há uma funcao hashing principal (i = 0) e uam funcao hashing duplo (i > 0).
(considerando B suficientemente grande)*/
int inserir(hash *tabela, unsigned B, string e){
    
    // declaracao de variaveis para uso posterior
    unsigned pos, i, aux;

    // converter o elemento e para o inteiro aux
    aux = converter(e);
    
    // incrementa i para cada tentativa de hash não sucedidade, ou seja,
    // se i > 0, houve ao menos um rehash para achar uma posicao para o elemento e
    // sendo e a string enviada pelo insercoes[i] para ser inserida no vetor
    for(i = 0; i < B; i++){
        // calcula a posicao na funcao hash
        pos = h(aux, i, B);
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
int buscar(hash *tabela, unsigned B, string e){
    
    // declaracao de variaveis para uso posterior
    unsigned pos, i, aux;

    // converter o elemento e para o inteiro aux
    aux = converter(e);

    // incrementa i para cada tentativa de achar a posicao no hash não sucedida
    // ou seja, cada vez que houve rehash na insercao, a busca deve acompanhar os passos
    // sendo e a string enviada pelo consultas[i] para ser inserida no vetor
    for (i = 0; i < B; i++){
        // calcula a posicao na funcao hash 
        pos = h(aux, i, B);
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

int main(int argc, char const *argv[]){
    //DECLARAÇÕES E PRÉ DEFINIÇÕES

    const int N = 50000;
    const int M = 70000;
    const int B = 150001;

    unsigned colisoes = 0;
    unsigned encontrados = 0;

    string *insercoes = ler_strings("strings_entrada.txt", N);
    string *consultas = ler_strings("strings_busca.txt", M);

    // HASHING FECHADO

    // cria uma tabela do tipo hash(com um vetor de tamanho B)
    hash tabela;
    
    // cria tabela hash com hash por hash duplo (com um vetor de tamanho B)
    criar(&tabela, B);

    // inserção dos dados na tabela hash
    
    // inicia contagem do tempo
    inicia_tempo();
    // roda o vetor de entradas (tamanho N)
    for (int i = 0; i < N; i++)
        // inserir insercoes[i] na tabela hash 
        // caso a funcao insserir retorne i > 0, houve ao menos uma colisão   
        if(inserir(&tabela, B, insercoes[i]) > 0) 
            colisoes++;    

    // finaliza contagem de tempo
    double tempo_insercao = finaliza_tempo();
    
    // consulta dos dados na tabela hash usando

    // inicia contagem do tempo
    inicia_tempo();
    // roda o vetor de consultas para a busca (tamanho M)
    for (int i = 0; i < M; i++)
        // busca consultar[i] na tabela hash
        // caso retorne -1 elemento não encontrado, cc elemento encontrado
        if(buscar(&tabela, B, consultas[i]) != -1) 
            encontrados++;
    
    // finaliza contagem de tempo
    double tempo_busca = finaliza_tempo();

    // liberacao do espaco de memoria alocado do vetor de insercoes e consultas
    free(insercoes);
    free(consultas);

    // OUTPUTS DOS DADOS OBTIDOS

    printf("Colisoes na insercao: %d\n", colisoes);
    printf("Tempo de insercao   : %fs\n", tempo_insercao);
    printf("Tempo de busca      : %fs\n", tempo_busca);
    printf("Itens encontrados   : %d\n", encontrados);


    return 0;
}