#include "TCamisa.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

// Cria camisa. Lembrar de usar free(camisa)
TCamisa *camisa(int cod, int tipo, char *ano, char *time)
{
    TCamisa *camisa = (TCamisa *)malloc(sizeof(TCamisa));
    // inicializa espaço de memória com ZEROS
    if (camisa)
        memset(camisa, 0, sizeof(TCamisa));
    // copia valores para os campos de camisa
    camisa->cod = cod;
    camisa->tipo = tipo;
    strcpy(camisa->ano, ano);
    strcpy(camisa->time, time);
    return camisa;
}

// Salva camisa no arquivo out, na posicao atual do cursor
void salvaCamisa(TCamisa *camisa, FILE *out)
{
    fwrite(&camisa->cod, sizeof(int), 1, out);
    fwrite(&camisa->tipo, sizeof(int), 1, out);
    fwrite(camisa->ano, sizeof(char), sizeof(camisa->ano), out);
    fwrite(camisa->time, sizeof(char), sizeof(camisa->time), out);
}

// Le uma camisa do arquivo in na posicao atual do cursor
// Retorna um ponteiro para camisa lido do arquivo
TCamisa *leCamisa(FILE *in)
{
    TCamisa *camisa = (TCamisa *)malloc(sizeof(TCamisa));
    if (0 >= fread(&camisa->cod, sizeof(int), 1, in))
    {
        free(camisa);
        return NULL;
    }
    fread(&camisa->tipo, sizeof(int), 1, in);
    fread(camisa->ano, sizeof(char), sizeof(camisa->ano), in);
    fread(camisa->time, sizeof(char), sizeof(camisa->time), in);
    return camisa;
}

// Imprime Camisa
void imprimeCamisa(TCamisa *camisa)
{
    printf("**********************************************");
    printf("\nCamisa de código ");
    printf("%d", camisa->cod);
    printf("\nTipo: ");
    printf("%d", camisa->tipo);
    printf("\nAno: ");
    printf("%s", camisa->ano);
    printf("\nTime: ");
    printf("%s", camisa->time);
    printf("\n**********************************************");
}

// Retorna tamanho de Camisa em bytes
int tamanhoCamisa()
{
    return sizeof(int)         // cod
           + sizeof(char) * 20 // time
           + sizeof(char) * 20 // ano
           + sizeof(int);      // tipo
}

// Retorna a quantidade de registros no arquivo
int tamanho_arquivo_Camisa(FILE *arq)
{
    fseek(arq, 0, SEEK_END);
    int tam = trunc(ftell(arq) / tamanhoCamisa());
    return tam;
}

TCamisa *buscaSequencialCamisa(int chave, FILE *in, FILE *log)
{

    TCamisa *ka;
    int achou;
    int cont = 0;
    clock_t inicio, fim;
    double total;

    rewind(in);

    inicio = clock();

    while ((ka = leCamisa(in)) != NULL)
    {

        cont++;

        if (ka->cod == chave)
        {
            achou = 1;
            break;
        }
    }
    if (achou == 1)
    {
        fprintf(log, "\nComparacoes Sequencial: %d ", cont);
        fim = clock();
        total = (fim - inicio) / CLOCKS_PER_SEC;
        fprintf(log, "\nTempo Sequencial: %f ", total);
        return ka;
    }

    else
    {
        fprintf(log, "Comparacoes Sequencial: %d ", cont);
        fim = clock();
        total = (fim - inicio) / CLOCKS_PER_SEC;
        fprintf(log, "Tempo Sequencial: %f ", cont);
    }

    free(ka);
    return NULL;
}

TCamisa *busca_binariaCamisa(int chave, FILE *in, int inicio, int fim, FILE *log)
{

    TCamisa *ka = NULL;
    int cod = -1;
    int cont = 0;
    clock_t inicioT, fimT;
    double total;

    inicioT = clock();

    while (inicio <= fim && cod != chave)
    {

        int meio = trunc((inicio + fim) / 2);
        // printf("Inicio: %d; Fim: %d; Meio: %d\n", inicio, fim, meio);
        fseek(in, (meio - 1) * tamanhoCamisa(), SEEK_SET);
        ka = leCamisa(in);
        cod = ka->cod;

        cont++;

        if (ka)
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
        fprintf(log, "\nComparacoes Binaria: %d ", cont);
        fimT = clock();
        total = (fimT - inicioT) / CLOCKS_PER_SEC;
        fprintf(log, "\nTempo Binaria: %f ", total);
        return ka;
    }
    else
        return NULL;
}

void criarBaseCamisa(FILE *out, int tam)
{
    int vet[tam];
    TCamisa *ka;

    for (int i = 0; i < tam; i++)
    {
        vet[i] = i + 1;
    }

    // shuffle(vet, tam, (tam*60)/100);
    embaralhaCamisa(vet, tam);

    printf("\nGerando a base de dados...\n");

    for (int i = 0; i < tam; i++)
    {
        ka = camisa(vet[i], 1, "2003", "Cruzeiro");
        salvaCamisa(ka, out);
    }
    free(ka);
}

void ImprimirBaseCamisa(FILE *out)
{
    printf("\nImprimindo a base de dados...\n");
    rewind(out);
    TCamisa *ka;

    while ((ka = leCamisa(out)) != NULL)
    {
        imprimeCamisa(ka);
    }
    free(ka);
}

void embaralhaCamisa(int *vet, int tam) {
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