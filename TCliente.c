#include "TCliente.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

// Cria cliente. Lembrar de usar free(cliente)
TCliente *cliente(int cod, char *nome, int cpf)
{
    TCliente *cliente = (TCliente *)malloc(sizeof(TCliente));
    // inicializa espaço de memória com ZEROS
    if (cliente)
        memset(cliente, 0, sizeof(TCliente));
    // copia valores para os campos de cliente
    cliente->cod = cod;
    strcpy(cliente->nome, nome);
    cliente->cpf = cpf;
    return cliente;
}

// Salva cliente no arquivo out, na posicao atual do cursor
void salva(TCliente *cliente, FILE *out)
{
    fwrite(&cliente->cod, sizeof(int), 1, out);
    // cliente->nome ao invés de &cliente->nome, pois string já é ponteiro
    fwrite(cliente->nome, sizeof(char), sizeof(cliente->nome), out);
    fwrite(cliente->cpf, sizeof(int), sizeof(cliente->cpf), out);
}

// Le um cliente do arquivo in na posicao atual do cursor
// Retorna um ponteiro para cliente lido do arquivo
TCliente *le(FILE *in)
{
    TCliente *cliente = (TCliente *)malloc(sizeof(TCliente));
    if (0 >= fread(&cliente->cod, sizeof(int), 1, in))
    {
        free(cliente);
        return NULL;
    }
    fread(cliente->nome, sizeof(char), sizeof(cliente->nome), in);
    fread(cliente->cpf, sizeof(int), sizeof(cliente->cpf), in);
    return cliente;
}

// Imprime cliente
void imprime(TCliente *cliente)
{
    printf("**********************************************");
    printf("\nCLiente de código ");
    printf("%d", cliente->cod);
    printf("\nNome: ");
    printf("%s", cliente->nome);
    printf("\nCPF: ");
    printf("%d", cliente->cpf);
    printf("\n**********************************************");
}

// Retorna tamanho do cliente em bytes
int tamanho()
{
    return sizeof(int)         // cod
           + sizeof(char) * 50 // nome
           + sizeof(int);   // cpf
}

// Retorna a quantidade de registros no arquivo
int tamanho_arquivo(FILE *arq) {
    fseek(arq, 0, SEEK_END);
    int tam = trunc(ftell(arq) / tamanho());
    return tam;
}