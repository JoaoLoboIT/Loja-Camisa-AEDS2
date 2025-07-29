#ifndef ENTIDADES_H
#define ENTIDADES_H

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <limits.h> 
#include <math.h>
#include <stdio.h>
#include <unistd.h>

typedef struct Cliente {
    int cod;
    int cpf;
    char nome[50];
} TCliente;

typedef struct Camisa {
    int cod;
    int tipo;
    char time[20];
    char ano[20];
} TCamisa;

typedef struct Pedido {
    int cod;
    TCliente cliente;
    TCamisa camisa;
} TPedido;

/*//////////////////////////////////////////////////////////////////////////////*/
//                                                                            //
//                         Funções TCamisa                                    //
//                                                                            //
/*//////////////////////////////////////////////////////////////////////////////*/

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

void ShellsortCamisa(FILE *arq, int tam);


/*//////////////////////////////////////////////////////////////////////////////*/
//                                                                            //
//                         Funções TCliente                                   //
//                                                                            //
/*//////////////////////////////////////////////////////////////////////////////*/

TCliente *cliente(int cod, char *nome, int cpf);

// Salva cliente no arquivo out, na posicao atual do cursor
void salvaCliente(TCliente *cliente, FILE *out);

// Le um cliente do arquivo in na posicao atual do cursor
// Retorna um ponteiro para cliente lido do arquivo
TCliente *leCliente(FILE *in);

// Imprime cliente
void imprimeCliente(TCliente *cliente);

int tamanhoCliente();
int tamanho_arquivo_Cliente(FILE *arq);

TCliente *buscaSequencialCliente(int chave, FILE *in, FILE *log);
TCliente *busca_binariaCliente(int chave, FILE *in, int inicio, int fim, FILE *log);

void criarBaseCliente(FILE *out, int tam);
void ImprimirBaseCliente(FILE *out);
void embaralhaCliente(int *vet, int tam);

void ShellsortCliente(FILE *arq, int tam);

void imprimir_conteudo_particao(const char* nome_arquivo, const char* titulo);
int selecao_natural_TCliente(FILE *arq, int M);
void intercalacao_otima_TCliente(int F, int total_particoes, FILE *arq_principal);

/*//////////////////////////////////////////////////////////////////////////////*/
//                                                                            //
//                         Funções TPedido                                    //
//                                                                            //
/*//////////////////////////////////////////////////////////////////////////////*/

// Cria um pedido. Lembrar de usar free(pedido)
TPedido *pedido(int cod, FILE *arqClientes, FILE *arqCamisas, FILE *log);

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

// Imprime a base de dados de pedidos
void ImprimirBasePedido(FILE *out);

void embaralhaPedido(int *vet, int tam);

void ShellsortPedido(FILE *arq, int tam);


#endif 