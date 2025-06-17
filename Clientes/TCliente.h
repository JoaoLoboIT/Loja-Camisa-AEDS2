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
void salva(TCliente *cliente, FILE *out);

// Le um cliente do arquivo in na posicao atual do cursor
// Retorna um ponteiro para cliente lido do arquivo
TCliente *le(FILE *in);

// Imprime cliente
void imprime(TCliente *cliente);
int tamanho_arquivo(FILE *arq);

TCliente *buscaSequencial(int chave, FILE *in, FILE *log);
TCliente *busca_binaria(int chave, FILE *in, int inicio, int fim, FILE *log);

void criarBase(FILE *out, int tam);
void ImprimirBase(FILE *out);
void embaralha(int *vet, int tam);

