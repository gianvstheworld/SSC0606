/*
============== PROJETO DE ESTRTURUAS DE DADOS 2 ==============

Felipe Andrade Garcia Tommaselli 11800910
Gianluca Capezzuto Sardinha 11876933

Explicação projeto:  https://ae4.tidia-ae.usp.br/access/content/attachment/61c65473-dfd7-45e0-9c5b-f83baa36b63d/Atividades/ecbb2c9d-24c5-4329-a2b2-84b2cdca1331/projeto_1.pdf

* Arquivo header
*/

// definir o tamanho da lista ser ordenada, ou seja, o tamanho da entrada
#define TAM 10000001

// tipo que sera usado nas listas do programa, desse modo não teriamos 
// problemas em mudar essa tipagem depois do codigo pronto 
typedef int elem;

// define o struct list que sera o vetor a ser ordenado
typedef struct{
   // ponteiro elementos leva o elemento em si do vetor 
   elem *elementos; 
   // tamanho carrega um long com o tamanho da lista
   long tamanho;
} list;


// Declarações de funções:

void cria(list *l);
void destroi(list *l);
int insere(list *l, elem e);
int insere_crescente(list *l, elem e);
int insere_decrescente(list *l, elem e);
void imprime(list *l);
void bubblesort(list *l);
void bubblesort_aprimorado(list *l);
void quicksort(list *l);
void radixsort(list *l);
void heapsort(list *l);