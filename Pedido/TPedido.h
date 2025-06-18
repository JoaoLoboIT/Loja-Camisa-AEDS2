#include <stdlib.h>
#include <stdio.h>
#include "../Clientes/TCliente.c"
#include "../Camisa/TCamisa.c"

typedef struct Pedido {
    int cod;
    TCliente cliente;
    TCamisa camisa;
} TPedido;

// Cria um pedido. Lembrar de usar free(pedido)
TPedido *pedido(int cod, TCliente *cliente, TCamisa *camisa);

// Salva pedido no arquivo out, na posição atual do cursor
void salvaPedido(TPedido *pedido, FILE *out);

// Lê um pedido do arquivo in na posição atual do cursor
TPedido *lePedido(FILE *in);

// Imprime pedido
void imprimePedido(TPedido *pedido);

// Retorna tamanho do pedido em bytes
int tamanhoPedido();

// Retorna a quantidade de registros no arquivo
int tamanho_arquivo_Pedido(FILE *arq);

// Busca sequencial de um pedido no arquivo
TPedido *buscaSequencialPedido(int chave, FILE *in, FILE *log);

// Busca binária de um pedido no arquivo
TPedido *busca_binariaPedido(int chave, FILE *in, int inicio, int fim, FILE *log);

// Imprime a base de dados de pedidos
void ImprimirBasePedido(FILE *out);

void embaralhaPedido(int *vet, int tam);
