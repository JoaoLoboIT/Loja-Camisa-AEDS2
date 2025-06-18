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
void salvaCliente(TCliente *cliente, FILE *out)
{
    fwrite(&cliente->cod, sizeof(int), 1, out);
    // cliente->nome ao invés de &cliente->nome, pois string já é ponteiro
    fwrite(cliente->nome, sizeof(char), sizeof(cliente->nome), out);
    fwrite(&cliente->cpf, sizeof(int), 1, out);
}

// Le um cliente do arquivo in na posicao atual do cursor
// Retorna um ponteiro para cliente lido do arquivo
TCliente *leCliente(FILE *in)
{
    TCliente *cliente = (TCliente *)malloc(sizeof(TCliente));
    if (0 >= fread(&cliente->cod, sizeof(int), 1, in))
    {
        free(cliente);
        return NULL;
    }
    fread(cliente->nome, sizeof(char), sizeof(cliente->nome), in);
    fread(&cliente->cpf, sizeof(int), 1, in);
    return cliente;
}

// Imprime cliente
void imprimeCliente(TCliente *cliente)
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
int tamanhoCliente()
{
    return sizeof(int)         // cod
           + sizeof(char) * 50 // nome
           + sizeof(int);   // cpf
}

// Retorna a quantidade de registros no arquivo
int tamanho_arquivo_Cliente(FILE *arq) {
    fseek(arq, 0, SEEK_END);
    int tam = trunc(ftell(arq) / tamanhoCliente());
    return tam;
}

TCliente *buscaSequencialCliente(int chave, FILE *in, FILE *log){

    TCliente *c;
    int achou;
    int cont = 0;
    clock_t inicio, fim;
    double total;

    rewind(in);

    inicio = clock();

    while ((c = leCliente(in)) != NULL){

        cont ++;

        if(c->cod == chave){
           achou = 1;
           break;
        }
    }
        if(achou == 1){
            fprintf(log, "\nComparacoes Sequencial: %d ", cont);
            fim = clock();
            total = (fim - inicio)/CLOCKS_PER_SEC;
            fprintf(log, "\nTempo Sequencial: %f ", total);
            return c;
        }

        else {
            fprintf(log, "Comparacoes Sequencial: %d ", cont);
            fim = clock();
            total = (fim - inicio)/CLOCKS_PER_SEC;
            fprintf(log, "Tempo Sequencial: %f ", cont);
        }

        free(c);
        return NULL;
}

TCliente *busca_binariaCliente(int chave, FILE *in, int inicio, int fim, FILE *log) {

    TCliente *c = NULL;
    int cod = -1;
    int cont = 0;
    clock_t inicioT, fimT;
    double total;

    inicioT = clock();

    while (inicio <= fim && cod != chave) {

        int meio = trunc((inicio + fim) / 2);
        //printf("Inicio: %d; Fim: %d; Meio: %d\n", inicio, fim, meio);
        fseek(in, (meio -1) * tamanhoCliente(), SEEK_SET);
        c = leCliente(in);
        cod = c->cod;

        cont ++;

        if (c) {
            if (cod > chave) {
                fim = meio - 1;
            } else {
                inicio = meio + 1;
            }
        }
    }

    if (cod == chave) {
        fprintf(log, "\nComparacoes Binaria: %d ", cont);
        fimT = clock();
        total = (fimT - inicioT)/CLOCKS_PER_SEC;
        fprintf(log, "\nTempo Binaria: %f ", total);
        return c;
    }
    else return NULL;
}

void criarBaseCliente(FILE *out, int tam){
    int vet[tam];
    TCliente *c;

    for(int i = 0; i < tam; i++){
        vet[i] = i + 1;
    }

    //shuffle(vet, tam, (tam*60)/100);
    embaralhaCliente(vet, tam);                   

    printf("\nGerando a base de dados...\n");

    for(int i = 0; i<tam; i++){
        c = cliente(vet[i], "Fulano", 111*i);
        salvaCliente(c, out);
    }
    free(c);
}

void ImprimirBaseCliente(FILE *out){
    printf("\nImprimindo a base de dados...\n");
    rewind(out);
    TCliente *c;

    while((c = leCliente(out)) != NULL){
        imprimeCliente(c);
    }
    free(c);
}

void embaralhaCliente(int *vet, int tam) {
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