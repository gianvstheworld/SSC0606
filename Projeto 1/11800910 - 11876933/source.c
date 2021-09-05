/*
============== PROJETO DE ESTRTURUAS DE DADOS 2 ==============

Felipe Andrade Garcia Tommaselli 11800910
Gianluca Capezzuto Sardinha 11876933

Explicaçao projeto:  https://ae4.tidia-ae.usp.br/access/content/attachment/61c65473-dfd7-45e0-9c5b-f83baa36b63d/Atividades/ecbb2c9d-24c5-4329-a2b2-84b2cdca1331/projeto_1.pdf

* Arquivo source
*/

// Include das bibiliotecas necessarias, incluindo o TAD
#include "header.h"
#include <stdio.h>
#include <stdlib.h>

// funçao que cria a lista (dinamicamente)
void cria(list *l){
    // inicia a lista com tamanho nulo
    l->tamanho = 0; 
    // inicia a lista por alocaçao dinamica com o tamanho TAM 
    l->elementos = malloc(sizeof(elem)*TAM); 
}

// funçao que destroi (liber ao espaco alocado dinamicamente)
void destroi(list *l) {
    // libera o espaço dinamico deixado para a lista
    free(l->elementos); 
    // finaliza o tamanho para 0
    l->tamanho = 0;
}

// funçao que insere os elementos dados no paraemtro na lista de forma aleatoria
int insere(list *l, elem elemento){
    // se a lista esta cheia retornamos erro
    if(l->tamanho == TAM)
        return 1; 
    //insere o elemento na ultima posiçao disponivel
    l->elementos[l->tamanho] = elemento;
    // aumenta o tamanho da lista para a achegada de um proximo elemento
    l->tamanho++; 
    return 0; 
}

// funçao que insere os elementos dados no paraemtro na lista de crescente
int insere_crescente(list *l, elem elemento){
    // se a lista esta cheia retornamos erro
    if(l->tamanho == TAM)
        return 1; 
    // loop iterando enquanto o tamanho for menor que o elemento passado
    while(l->tamanho < elemento){
        //insere o atual tamanho na ultima posiçao dsponivel
        l->elementos[l->tamanho] = l->tamanho;
        // aumenta o tamanho da lista para a achegada de um proximo elemento
        l->tamanho++; 
        return 0; 
    }
}

// funçao que insere os elementos dados no paraemtro na lista de forma descrescente
int insere_decrescente(list *l, elem elemento){
    // se a lista esta cheia retornamos erro
    if(l->tamanho == TAM)
        return 1; 
    // loop iterando enquanto o tamanho for menor que o elemento passado
    while(l->tamanho < elemento){
        //insere a diferenca do elemento com o atual tamanho, na ultima posiçao dsponivel
        l->elementos[l->tamanho] = elemento - l->tamanho;
        // aumenta o tamanho da lista para a achegada de um proximo elemento
        l->tamanho++; 
        return 0; 
    }
}

// funçao que imprime na tela os elementos da lista
void imprime(list *l) {
    // declraçao e iniciaçao da varaivel do loop
    long i = 0;
    
    // imprime ( ) se a lista estiver vazia
    if(l->tamanho == 0)
        printf("( )\n");
    // caso contrario, ou seja, lista com elementos
    else{
        // abre chaves
        printf("("); 
        // percorre os elementos da lista
        for(i; i < l->tamanho - 1; i++) // percorre cada elemento da lista
            // printa cada elemento um por um com uma "," entre eles
            printf("%d, ", l->elementos[i]); 
         // fecha chaves
        printf("%d)\n", l->elementos[i]);
    }
}
//* BUBBLESORT
/* 
O Bubble Sort eh um algoritmo dos algoritmos de ordenaçao mais simples possiveis. 
Ele possui complexidade quadratica no pior caso.
Seu funcionamento baseia-se em percorrer o vetor muitas vezes, sendo que a cada passagem eh feito a 
movimentaçao do maior elemento para o topo da sequencia, parecendo-se com uma bolha subindo num tanque d'agua.
Portanto, o algoritmo consiste em fazer comparaçoes da posiçao atual com a proxima e, se a posiçao atual for maior
que a seguinte, eh realizada a troca dos valores nessa posiçao. Caso nao seja necessaria a troca eh passado para o 
proximo par de elementos do vetor a ser comparado.
*/

void bubblesort(list *l){
    // Declaracoes
    int i = 0, j = 0;
    int tam = l->tamanho;
    elem aux;

    // loop passando pela lista inteira
    for(i = 0; i <= tam; i++){
        // loop pelas subsequencias possiveis de troca
       for(j = 0; j <= tam - i - 2; j++){
           // condicao de troca citada na explicaçao da linha 103
           if(l->elementos[j] > l->elementos[j + 1]){
               // o valor do proximo elemento eh atribuido a uma variavel auxiliar (elemento menor)
               // e o elemento maior vai para a proxima posiçao do vetor, efetuando a troca.
               aux = l->elementos[j + 1];
               l->elementos[j+1] = l->elementos[j];
               l->elementos[j] = aux;
           }
       }
    }
}

//* BUBBLESORT_APRIMORADO
/* 
O Bubble Sort aprimorado consiste na mesma ideia que o Bubble Sort normal. Porem,
a grande diferença que o torna aprimorado eh o caso de o vetor ja estar ordenado, ou seja,
minimizando o tempo de ordenaçao no melhor caso e isso eh possivel ser observado nas analises 
empiricas para o melhor caso
*/

void bubblesort_aprimorado(list *l){
    // Declaracoes
    int i = 0, j = 0, ordenado = 1;
    elem tam = l->tamanho;
    elem aux;

    // loop passando pela lista inteira
    for(i = 0; i <= tam; i++){
        // loop pelas subsequencias possiveis de troca
       for(j = 0; j <= tam - i - 2; j++){
           // condicao de troca
           if(l->elementos[j] > l->elementos[j + 1]){
               // troca
               aux = l->elementos[j + 1];
               l->elementos[j+1] = l->elementos[j];
               l->elementos[j] = aux;
               // ha troca de posicoes na lista, entao ordenado atribui um valor para continuar
               // fazendo a troca de posiçoes dos elementos.
               ordenado = 0;
           }
        }
        // se o vetor ja estiver ordenado e pular a condicao de troca eh indicado 
        // que nao ha necessidade de continuar tentando ordenar o mesmo, por isso 
        // ha a parada no codigo
        if(ordenado == 1)
            break;
    }
}

//* QUICKSORT
/* 
O Quicksort eh um algoritmo dos algoritmos de ordenaçao mais simples possiveis. 
Ele possui complexidade quadratica no pior caso. Apesar do tempo de execuçao ser lento no pior caso, 
este algoritmo eh comumente usado como melhor opçao para a ordenaçao.
Dessa forma, eh usado a ordenaçao por intercalaçao, baseando-se no conceito de dividir e conquistar: 
--> Os elementos sao particionados em dois subarranjos, os tais que sao menores a um elemento i em especifico
ou maiores em relaçao a esse mesmo elemento. 
    A[0,...,i-1] - A[i] - A[i+1,...,fim]
Comumente, eh usado o particionamento de lomuto, o qual seleciona i como sendo algum elemento dos extremos, 
mas ao desenvolver do algoritmo foram encontradas soluçoes melhores que essa.
--> Os dois subarranjos sao ordenadas por chamadas recursivas ao algoritmo e assim, havera um momento no qual
sera necessario ajustar apenas a posiçai de i para que os elementos estejam ordenados.
*/

int particion(list *l, int ini, int fim){
    // Declaracoes
    elem pivo = l->elementos[fim];
    int i = ini - 1, j = ini;
    elem aux;

    // loop percorrendo o vetor inteiro 
    while(j <= fim - 1){
        // caso o elemento seguinte ao inicial seja o menor que o pivo eh passado uma posiçao a frente
        // e assim feito a troca entre o inicial e o seu sucessor
        if(l->elementos[j] < pivo){
            i++;
            aux = l->elementos[i];
            l->elementos[i] = l->elementos[j];
            l->elementos[j] = aux;
        } 
        j++;
    }
    // caso contrario, os elementos que sao maiores ao pivo sao trocados com os que estao no final, 
    // garantindo a nossa intençao de dividir os elementos entre os menores - pivo - maiores
    aux = l->elementos[i + 1];
    l->elementos[i + 1] = l->elementos[fim];
    l->elementos[fim] = aux;

    // eh retornado o proximo elemento de i para a verificaçao de todos os elementos do vetor
    return i + 1;
}

int random_particion(list *l, int ini, int fim){
    // declaraçao de variaveis 
    int k =  ini + rand() % (fim - ini + 1);
    // inicialmente, estavamos usando o tamanho da lista para randomizar a particao,
    // mas percebemos que a cada passagem do vetor os elementos i e j vao se aproximando,
    // portanto o vetor nao fica constantemente com o mesmo tamanho de elementos a serem ordenados.
    // printf("l->tamanho: %ld\n", l->tamanho);
    // printf("fim - ini + 1: %d\n", fim - ini + 1);

    elem aux;

    // troca entre o elemento aleatorio da particao para ser posicionado como pivo pelo particionamento de lomuto
    aux = l->elementos[k];
    l->elementos[k] = l->elementos[fim];
    l->elementos[fim] = aux;
    
    return particion(l, ini, fim);
}

void quicksort_rec(list *l, int ini, int fim){
    // declaracao de variaveis
    int pivo = 0;

    // enquanto os elementos do inicio forem menores do que os do fim eh chamado recursivamente esta funcao
    if(ini < fim){
        pivo = random_particion(l, ini, fim);
        quicksort_rec(l, ini, pivo - 1);
        quicksort_rec(l, pivo + 1, fim);
    }
}

void quicksort(list *l){
    // chamada recursiva para o quicksort
        quicksort_rec(l, 0, l->tamanho - 1);
}

//* RADIX SORT
/* 
O radix sort eh um algoritmo de ordenaçao rapido e estavel. 
Ele possui complexidade linear no pior caso.
Seu funcionamento eh baseado na ordenaçaçao por meio dos algarismos (digitos individuais), que
constituem a cadeira de caracteres que foram o numero na i-esima posicao a ser ordenado.
Essa ordenacao eh baseada por chaves unicas de indexaçao que garantem a ordenaçao final dos elementos 
*/

void Counting_sort(list *l, elem tam, elem pos){
    // Declaracoes
    int i = 0; // variavel para percorrer os loops
    elem key = 0; // atribui 0 para a varaivel chave por enquanto
    // inicia dinamicamente o vetor B com 10 elementos ja atribuindo 0 nele inteiro
    long* B = calloc(10, sizeof(long)); 
    // inicia dinamicamente o vetor C com o tamanho de elementos da lista, atribuindo 0 tambem
    long* C = calloc(tam, sizeof(long));
    
    // loop percorrendo o vetor inteiro
    for(int i = 0; i <= tam - 1; i++){
        // variavel chave recebe o valor do elemento em relacao a sua posicao
        key = l->elementos[i]/pos;
        // a propria chave recebe o resto por 10 da antiga relacao entre elemento e pos 
        // isto eh, no decorrer das iteracoes separamos os digitos do valor querido
        // (unidade, dezena, centena ...) 
        key = key % 10;
        // o valor atual de B na chave eh incrementado 
        B[key] += 1;
    }

    // looping de 1 a 9
    for(i = 1; i <= 9; i++)
        // o elemento de B na i-esima posicao recebe o valor acumulado ate a essa posicao
        // ou seja, o elemento eh montado novamente
        B[i] = B[i] + B[i - 1];

    // looping percorrendo o vetor de forma descrescente
    for(i = tam - 1; i >= 0; i--){
        // variavel chave recebe o valor do elemento em relacao a sua posicao
        key = l->elementos[i]/pos;
        // a propria chave recebe o resto por 10 da antiga relacao entre elemento e pos 
        // isto eh, no decorrer das iteracoes separamos os digitos do valor querido
        // (unidade, dezena, centena ...) 
        key = key % 10;
        // o valor atual de C na chave eh decrementado 
        B[key] = B[key] - 1;
        // o elemento de C na posicao dada pelo valor de B na posicao atual da chave
        // recebe o elemento da posicao i, ou seja, apos abrirmos os algoritmos do valor
        // que sera ordenado, ele eh comparado e montado novamente, para assim ser ordenao
        // a partir das comparaçoes entre seus alogoritimos. No final desse processo, C tera
        // recebido os elementos do vetor inicial na posicao correta e estara ordenado
        C[B[key]] = l->elementos[i];
    }

    // loop percorrendo o vetor de forma crescente
    for(i = 0; i <= tam - 1; i++)
        // a lista recebe o valor de C na posicao i, uma vez que C ja esta com o vetor ordenado
        l->elementos[i] = C[i];
}

void radixsort(list *l){
    // Declaracoes
    elem tam = l->tamanho; //atribui a vraivel tamanho o tamanho do vetor
    elem maior = l->elementos[0]; //atribui a varaivel maior o primeiro elemento
    elem pos = 1; //atribui a varaivel pos valor 1 
    
    // loop percorrendo o vetor todo
    for(int i = 0; i <= tam; i++)
        // procura o maior elemento
        if (l->elementos[i] > maior)
    	    // atribui para maior o maior valor ate o momento
            maior = l->elementos[i];
    
    // loop enquanto maior for maior que pos
    while((maior/pos) > 0){
        Counting_sort(l, tam, pos); // funcao de ordenacao do radixsort
        pos *= 10; // aumenta a pos em potencias de 10
    }
}

//* HEAPSORT

/*
O heap sort eh um algoritmo de ordenaçao por seleçao, nao estavel.
Ele possui complexidade O(nlogn) no pior caso.
Seu funcionamento eh baseado na ordenaçaçao por meio de uma estrtura de dados chamada de heap.
A heap pode ser representada por uma arvore binaria (ou por um vetor, mas menos intuitiva).
Os elementos sao inseridos na heap de forma que ao fim teremos o vetor ordenado, caso as proprie
dades da heap sejam corretamente mantidas ao longo das inserçoes. A heap pode ser uma max heap 
(ordena o vetor em ordem crescente) ou min heap (ordem descrescente).
*/

void Heapify(list *l,elem tam,int i){
    // Declaracoes
    elem maior = i; // define o elemento maior como i por enquanto 
    elem aux = 0;  // variavel de troca
    elem esq = (2*i) + 1; // declara e atribue valor para variavel da esquerda  
    elem dir = (2*i) + 2; // declara e atribue valor para variavel da esquerda
    
    // comparaçao entre o elemento da esquerda e o maior elemento 
    if(esq < tam && l->elementos[esq] > l->elementos[maior])
        // se o elemento da esquerda relmente for o maior, maior recebe ele
        maior = esq; 

    // comparaçao entre o elemento da direita e o maior elemento 
    if(dir < tam && l->elementos[dir] > l->elementos[maior])
        // se o elemento da direita relmente for o maior, maior recebe ele
        maior = dir;

    //  compara se maior eh diferente de i
    if(maior > i || maior < i){
        // nesse caso trocamos o elemento da posicao i com o elemento da maior posicao
        aux = l->elementos[i];
        l->elementos[i] = l->elementos[maior];
        l->elementos[maior] = aux;
        // o algoritmo entao eh chamado recursivamente, com o proximo sendo o nosso atual maior
        Heapify(l, tam, maior);
    }
}

void heapsort(list *l){
    // Declaraçoes
    elem tam = l->tamanho; // tam eh iniciado com o tamanho do vetor lista
    elem aux; // elemento auxiliar para trocas
    int i = 0; // operador de iteraçao para os loopings

    // Looping varrendo a metade esquerda do vetor (partindo da metade ate o inicio)
    for(i = (tam/2) - 1; i >= 0; i--)
        Heapify(l, tam, i); // funçao com a ordenaçao 

    // looping varrendo quase todo o vetor (partindo do fim ate o segundo elemento)
    for(i = tam - 1; i >= 1; i--){
        // troca entre o primeiro elemento e o i-esimo elemento da iteracao
        aux = l->elementos[0];
        l->elementos[0] = l->elementos[i];
        l->elementos[i] = aux;
        Heapify(l, i, 0); // funçao com a ordenaçao 
    }
}