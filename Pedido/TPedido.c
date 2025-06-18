#include "TPedido.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include "../Clientes/TCliente.c"
#include "../Camisa/TCamisa.c"
#include <math.h>


// Cria um pedido. Lembrar de usar free(pedido)
TPedido *pedido(int cod, TCliente *cliente, TCamisa *camisa)
{
    TPedido *pedido = (TPedido *)malloc(sizeof(TPedido));
    if (pedido)
        memset(pedido, 0, sizeof(TPedido));

    // Copia os valores para os campos de pedido
    pedido->cod = cod;
    pedido->cliente = *cliente;  // Copiar os dados do cliente
    pedido->camisa = *camisa;    // Copiar os dados da camisa

    return pedido;
}

// Salva pedido no arquivo out, na posição atual do cursor
void salvaPedido(TPedido *pedido, FILE *out)
{
    fwrite(&pedido->cod, sizeof(int), 1, out);
    // Salvar dados do cliente
    fwrite(&pedido->cliente.cod, sizeof(int), 1, out);
    fwrite(pedido->cliente.nome, sizeof(char), sizeof(pedido->cliente.nome), out);
    fwrite(&pedido->cliente.cpf, sizeof(int), 1, out);
    // Salvar dados da camisa
    fwrite(&pedido->camisa.cod, sizeof(int), 1, out);
    fwrite(&pedido->camisa.tipo, sizeof(int), 1, out);
    fwrite(pedido->camisa.ano, sizeof(char), sizeof(pedido->camisa.ano), out);
    fwrite(pedido->camisa.time, sizeof(char), sizeof(pedido->camisa.time), out);
}

// Lê um pedido do arquivo in na posição atual do cursor
TPedido *lePedido(FILE *in)
{
    TPedido *pedido = (TPedido *)malloc(sizeof(TPedido));
    if (0 >= fread(&pedido->cod, sizeof(int), 1, in))
    {
        free(pedido);
        return NULL;
    }

    // Ler dados do cliente
    fread(&pedido->cliente.cod, sizeof(int), 1, in);
    fread(pedido->cliente.nome, sizeof(char), sizeof(pedido->cliente.nome), in);
    fread(&pedido->cliente.cpf, sizeof(int), 1, in);
    
    // Ler dados da camisa
    fread(&pedido->camisa.cod, sizeof(int), 1, in);
    fread(&pedido->camisa.tipo, sizeof(int), 1, in);
    fread(pedido->camisa.ano, sizeof(char), sizeof(pedido->camisa.ano), in);
    fread(pedido->camisa.time, sizeof(char), sizeof(pedido->camisa.time), in);

    return pedido;
}

// Imprime pedido
void imprimePedido(TPedido *pedido)
{
    printf("**********************************************\n");
    printf("Pedido de código %d\n", pedido->cod);
    
    // Imprimir dados do cliente
    printf("Cliente: %d\n", pedido->cliente.cod);
    printf("Nome: %s\n", pedido->cliente.nome);
    printf("CPF: %d\n", pedido->cliente.cpf);

    // Imprimir dados da camisa
    printf("Camisa: %d\n", pedido->camisa.cod);
    printf("Tipo: %d\n", pedido->camisa.tipo);
    printf("Ano: %s\n", pedido->camisa.ano);
    printf("Time: %s\n", pedido->camisa.time);

    printf("**********************************************\n");
}

// Retorna tamanho do pedido em bytes
int tamanhoPedido()
{
    return sizeof(int)           // cod
           + sizeof(TCliente)    // cliente
           + sizeof(TCamisa);    // camisa
}

// Retorna a quantidade de registros no arquivo
int tamanho_arquivo_Pedido(FILE *arq) {
    fseek(arq, 0, SEEK_END);
    int tam = trunc(ftell(arq) / tamanhoPedido());
    return tam;
}

// Busca sequencial de um pedido no arquivo
TPedido *buscaSequencialPedido(int chave, FILE *in, FILE *log)
{
    TPedido *pedido;
    int achou;
    int cont = 0;
    clock_t inicio, fim;
    double total;

    rewind(in);

    inicio = clock();

    while ((pedido = lePedido(in)) != NULL)
    {
        cont++;

        if (pedido->cod == chave)
        {
            achou = 1;
            break;
        }
    }

    if (achou == 1)
    {
        fprintf(log, "\nComparacoes Sequenciais: %d ", cont);
        fim = clock();
        total = (fim - inicio) / CLOCKS_PER_SEC;
        fprintf(log, "\nTempo Sequencial: %f ", total);
        return pedido;
    }
    else
    {
        fprintf(log, "\nComparacoes Sequenciais: %d ", cont);
        fim = clock();
        total = (fim - inicio) / CLOCKS_PER_SEC;
        fprintf(log, "\nTempo Sequencial: %f ", total);
    }

    free(pedido);
    return NULL;
}

// Busca binária de um pedido no arquivo
TPedido *busca_binariaPedido(int chave, FILE *in, int inicio, int fim, FILE *log)
{
    TPedido *pedido = NULL;
    int cod = -1;
    int cont = 0;
    clock_t inicioT, fimT;
    double total;

    inicioT = clock();

    while (inicio <= fim && cod != chave)
    {
        int meio = trunc((inicio + fim) / 2);
        fseek(in, (meio - 1) * tamanhoPedido(), SEEK_SET);
        pedido = lePedido(in);
        cod = pedido->cod;

        cont++;

        if (pedido)
        {
            if (cod > chave)
            {
                fim = meio - 1;
            }
            else
            {
                inicio = meio + 1;
            }
        }
    }

    if (cod == chave)
    {
        fprintf(log, "\nComparacoes Binarias: %d ", cont);
        fimT = clock();
        total = (fimT - inicioT) / CLOCKS_PER_SEC;
        fprintf(log, "\nTempo Binario: %f ", total);
        return pedido;
    }
    else
    {
        return NULL;
    }
}

// Imprime a base de dados de pedidos
void ImprimirBasePedido(FILE *out)
{
    printf("\nImprimindo a base de pedidos...\n");
    rewind(out);
    TPedido *pedido;

    while ((pedido = lePedido(out)) != NULL)
    {
        imprimePedido(pedido);
        free(pedido);
    }
}

void embaralhaPedido(int *vet, int tam) {
    int tmp;
    srand(time(NULL));
    int trocas = (tam*60)/100;

    for(int t = 1; t < trocas; t++){
        int i = rand() % tam;
        int j = rand() % tam;
        tmp = vet[i];
        vet[i] = vet[j];
        vet[j] = tmp;
    }
}
