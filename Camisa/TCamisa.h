#include <stdlib.h>
#include <stdio.h>

typedef struct Camisa {
    int cod;
    int tipo;
    char time[20];
    char ano[20];
} TCamisa;

// Cria cliente. Lembrar de usar free(cliente)
TCamisa *camisa(int cod, int tipo, char *ano, char *time);

// Salva cliente no arquivo out, na posicao atual do cursor
void salvaCamisa(TCamisa *camisa, FILE *out);

// Le um cliente do arquivo in na posicao atual do cursor
// Retorna um ponteiro para cliente lido do arquivo
TCamisa *leCamisa(FILE *in);

// Imprime cliente
void imprimeCamisa(TCamisa *cliente);

int tamanhoCamisa();
int tamanho_arquivo_Camisa(FILE *arq);

TCamisa *buscaSequencialCamisa(int chave, FILE *in, FILE *log);
TCamisa *busca_binariaCamisa(int chave, FILE *in, int inicio, int fim, FILE *log);

void criarBaseCamisa(FILE *out, int tam);
void ImprimirBaseCamisa(FILE *out);
void embaralhaCamisa(int *vet, int tam);

