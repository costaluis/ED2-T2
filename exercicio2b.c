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


unsigned h_div(unsigned x, unsigned i, unsigned B)
{
    return ((x % B) + i) % B;
}

unsigned h_mul(unsigned x, unsigned i, unsigned B)
{
    const double A = 0.6180;
    return  ((int) ((fmod(x * A, 1) * B) + i)) % B;
}

unsigned h_duplo(unsigned x, unsigned i, unsigned B)
{
    return (h_mul(x,i,B) + i*h_div(x,i,B))%B;
}

int insere_h_duplo(string elem, string * tabela, unsigned B){
    unsigned elem_int;
    unsigned i;
    unsigned pos;
    elem_int = converter(elem);
    for(i=0; i<B; i++){
        pos = h_duplo(elem_int,i,B);
        if(!strcmp(tabela[pos],"-1")){
            tabela[pos] = elem;
            return i;
        }
        if(!strcmp(tabela[pos],elem))
            return 0;
    }
    return i;
}

int busca_h_duplo(string elem, string * tabela, unsigned B){
    unsigned i;
    unsigned elem_int;
    unsigned pos;
    elem_int = converter(elem);
    for(i=0 ; i<B ; i++){
        pos = h_duplo(elem_int,i,B);
        if(!strcmp(tabela[pos],elem))
            return 1;
        if(!strcmp(tabela[pos],"-1"))
            return 0;
    }
    return 0;
}

int main(int argc, char const *argv[])
{
    const int N = 50000;
    const int M = 70000;
    const int B = 150001;

    unsigned colisoes = 0;
    unsigned encontrados = 0;

    string* insercoes = ler_strings("strings_entrada.txt", N);
    string* consultas = ler_strings("strings_busca.txt", M);


    // cria tabela hash com hash por hash duplo

    string * tabela = (string*) malloc(B * sizeof(string));
    for(int i=0;i<B;i++)
        tabela[i]="-1";

    // inserção dos dados na tabela hash
    inicia_tempo();
    for (int i = 0; i < N; i++) {
        // inserir insercoes[i] na tabela hash
        if(insercoes[i]!=NULL)
            colisoes += insere_h_duplo(insercoes[i],tabela,B);
    }
    double tempo_insercao = finaliza_tempo();

    // busca dos dados na tabela hash
    inicia_tempo();
    for (int i = 0; i < M; i++) {
        // buscar consultas[i] na tabela hash
        if(consultas[i]!=NULL)
            encontrados += busca_h_duplo(consultas[i],tabela,B);
    }
    double tempo_busca = finaliza_tempo();


    printf("Colisões na inserção: %d\n", colisoes);
    printf("Tempo de inserção   : %fs\n", tempo_insercao);
    printf("Tempo de busca      : %fs\n", tempo_busca);
    printf("Itens encontrados   : %d\n", encontrados);

    return 0;
}
