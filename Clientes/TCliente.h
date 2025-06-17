#include <stdlib.h>
#include <stdio.h>

typedef struct Cliente {
    int cod;
    int cpf;
    char nome[50];
} TCliente;

// Cria cliente. Lembrar de usar free(cliente)
TCliente *cliente(int cod, char *nome, int cpf);

// Salva cliente no arquivo out, na posicao atual do cursor
void salvaCliente(TCliente *cliente, FILE *out);

// Le um cliente do arquivo in na posicao atual do cursor
// Retorna um ponteiro para cliente lido do arquivo
TCliente *leCliente(FILE *in);

// Imprime cliente
void imprimeCliente(TCliente *cliente);

int tamanhoCliente();
int tamanho_arquivo(FILE *arq);

TCliente *buscaSequencialCliente(int chave, FILE *in, FILE *log);
TCliente *busca_binariaCliente(int chave, FILE *in, int inicio, int fim, FILE *log);

void criarBaseCliente(FILE *out, int tam);
void ImprimirBaseCliente(FILE *out);
void embaralhaCliente(int *vet, int tam);

