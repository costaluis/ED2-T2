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

typedef struct dados
{
    struct dados * prox;
    string entrada;
}dados;

#define MAX_STRING_LEN 20


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

int busca_h_mul(string elem, dados ** tabela, unsigned B){
    unsigned elem_int = converter(elem);
    unsigned pos = h_mul(elem_int,B);
    dados * aux = tabela[pos];
    
    while(aux != NULL){   
        if(!strcmp(aux->entrada,elem))
            return 1;
        aux = aux->prox;
    }
    
    return 0;
}

int insere_h_mul(string elem, dados ** tabela, unsigned B){
    unsigned elem_int;
    unsigned pos;
    unsigned i=0;
    dados * aux;
    
    elem_int = converter(elem);
    pos = h_mul(elem_int,B); 

    aux = tabela[pos];

    while(aux != NULL){   
        if(!strcmp(aux->entrada,elem))
            return i;
        aux = aux->prox;
        i++;
    }

    dados * elemento = (dados *) malloc(sizeof(dados));
    
    elemento->entrada = elem;
    elemento->prox = tabela[pos];
    tabela[pos] = elemento;
    
    return i;
    
}

int busca_h_div(string elem, dados ** tabela, unsigned B){
    unsigned elem_int = converter(elem);
    unsigned pos = h_div(elem_int,B);
    dados * aux = tabela[pos];
    
    while(aux != NULL){   
        if(!strcmp(aux->entrada,elem))
            return 1;
        aux = aux->prox;
    }
    
    return 0;
}

int insere_h_div(string elem, dados ** tabela, unsigned B){
    unsigned elem_int;
    unsigned pos;
    unsigned i=0;
    dados * aux;
    
    elem_int = converter(elem);
    pos = h_div(elem_int,B); 

    aux = tabela[pos];

    while(aux != NULL){   
        if(!strcmp(aux->entrada,elem))
            return i;
        aux = aux->prox;
        i++;
    }

    dados * elemento = (dados *) malloc(sizeof(dados));
    
    elemento->entrada = elem;
    elemento->prox = tabela[pos];
    tabela[pos] = elemento;
    
    return i;
    
}

void destroi_tabela(dados ** tabela, unsigned B){
    unsigned i;
    dados * aux1;
    dados * aux2;

    for(i=0;i<B;i++){
        aux1 = tabela[i];
        while(aux1 != NULL){
            aux2 = aux1;
            aux1 = aux1->prox;
            free(aux2);
        }
    }
    free(tabela);
    return;
}

int main(int argc, char const *argv[])
{
    const int N = 50000;
    const int M = 70000;
    const int B = 150001;

    unsigned colisoes_h_div = 0;
    unsigned colisoes_h_mul = 0;

    unsigned encontrados_h_div = 0;
    unsigned encontrados_h_mul = 0;

    string* insercoes = ler_strings("strings_entrada.txt", N);
    string* consultas = ler_strings("strings_busca.txt", M);
    

    // cria tabela hash com hash por multiplicação

    dados ** tabela = (dados **) malloc(B * sizeof(dados *));
    for(int i=0; i<B;i++){
        tabela[i] = NULL;
    }
    
    // inserção dos dados na tabela hash com hash por multiplicação
    inicia_tempo();
    for (int i = 0; i < N; i++) {
        // inserir insercoes[i] na tabela hash
        if(insercoes[i]!=NULL)
            colisoes_h_mul += insere_h_mul(insercoes[i],tabela,B);
    }
    double tempo_insercao_h_mul = finaliza_tempo();

    // busca dos dados na tabela hash com hash por multiplicação
    inicia_tempo();
    for (int i = 0; i < M; i++) {
        // buscar consultas[i] na tabela hash
        if(consultas[i]!=NULL)
            encontrados_h_mul += busca_h_mul(consultas[i],tabela,B);
    }
    double tempo_busca_h_mul = finaliza_tempo();

    // destroi tabela hash com hash por multiplicação

    destroi_tabela(tabela,B);


    // cria tabela hash com hash por divisão

    tabela = (dados **) malloc(B * sizeof(dados *));
    
    for(int i=0; i<B;i++){
        tabela[i] = NULL;
    }

    // inserção dos dados na tabela hash com hash por divisão
    inicia_tempo();
    for (int i = 0; i < N; i++) {
        // inserir insercoes[i] na tabela hash
        if(insercoes[i]!=NULL)
            colisoes_h_div += insere_h_div(insercoes[i],tabela,B);

    }
    double tempo_insercao_h_div = finaliza_tempo();

    // busca dos dados na tabela hash com hash por divisão
    inicia_tempo();
    for (int i = 0; i < M; i++) {
        // buscar consultas[i] na tabela hash
        if(consultas[i]!=NULL)
            encontrados_h_div += busca_h_div(consultas[i],tabela,B);
    }
    double tempo_busca_h_div = finaliza_tempo();

    // destroi tabela hash com hash por divisão



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

    return 0;
}
