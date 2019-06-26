#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Definição das variaveis que controlam a medição de tempo
clock_t _ini, _fim;

// Definição do tipo booleano
unsigned char typedef bool;
#define TRUE  1
#define FALSE 0

typedef struct elemento
{
    int pos;
    int valor;
}elemento;

int* ler_inteiros(const char * arquivo, const int n)
{
    FILE* f = fopen(arquivo, "r");

    int* inteiros = (int *) malloc(sizeof(int) * n);

    for (int i = 0; !feof(f); i++)
        fscanf(f, "%d\n", &inteiros[i]);

    fclose(f);

    return inteiros;
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

void rearranja_heap(int *l, int i, int tam_heap) {
    int fe, fd, mf;
    int aux;
    fe = (2*i+1 < tam_heap) ? 2*i+1 : -1; // indice do filho esq
    fd = (2*i+2 < tam_heap) ? 2*i+2 : -1; // indice do filho dir
    if (fe == -1)
        return; // nenhum filho; heap ok
    mf = (fd == -1 || l[fe] > l[fd]) ? fe : fd; // encontra maior filho
    if (l[i] >= l[mf])
        return; // raiz maior ou igual aos filhos; heap ok
    // troca raiz por maior filho
    aux = l[i];
    l[i] = l[mf];
    l[mf] = aux;
    rearranja_heap(l, mf, tam_heap); // arruma proximo nivel
    return;
}

void constroi_heap(int *l, int tam_heap) {
    for (int i = tam_heap/2-1; i>=0; i--)
        rearranja_heap(l,i,tam_heap);
    return;
}

void heap_sort(int *l, int tam_heap) {
    int aux;
    // constroi heap
    constroi_heap(l, tam_heap);
    // remove um por vez, em ordem
    while (tam_heap > 1) {
        // troca raiz por ultima folha
        aux = l[0];
        l[0] = l[tam_heap-1];
        l[tam_heap-1] = aux;
        tam_heap--; // diminui heap
        rearranja_heap(l,0,tam_heap);
    }
    return;
}

int busca_tabela(elemento * tabela, int n, int T){
    int i;
    for(i=0;i<T;i++){
        if(n < tabela[i].valor){
            return (i == 0) ? -1 : i-1;
        }
    }

    return T-1;
}

int busca_sequencial(int *entradas, elemento * tabela, int n, int T){
    
    int posicao = busca_tabela(tabela,n,T);
    int inicio, fim;
    int i;

    if(posicao == -1)
        return FALSE;

    inicio = tabela[posicao].pos;
    fim = (posicao == T-1) ? 50000 : tabela[posicao+1].pos;


    for(i=inicio; i < fim; i++){
        if(entradas[i]==n)
            return TRUE;
    }

    return FALSE;
}


int main(int argc, char const *argv[])
{
    const int N = 50000;
    const int index_size = 10000;
    unsigned encontrados = 0;

    int* entradas = ler_inteiros("inteiros_entrada.txt", N);
    int* consultas = ler_inteiros("inteiros_busca.txt", N);
    
    // ordenar entrada

    heap_sort(entradas,N);

    // criar tabela de indice

    int T = N/index_size;

    elemento indices[T];

    for(int i=0; i<T; i++){
        indices[i].valor = entradas[i*index_size];
        indices[i].pos = i*index_size;
    }

    // realizar consultas na tabela de indices 
    inicia_tempo();
    for (int i = 0; i < N; i++) {
        // buscar o elemento consultas[i] na entrada
        encontrados += busca_sequencial(entradas,indices,consultas[i],T);   
    }
    double tempo_busca = finaliza_tempo();

    printf("Tempo de busca    :\t%fs\n", tempo_busca);
    printf("Itens encontrados :\t%d\n", encontrados);

    return 0;
}
