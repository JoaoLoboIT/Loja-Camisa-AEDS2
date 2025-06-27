#ifndef ENTIDADES_C
#define ENTIDADES_C

#include "entidades.h"

/*//////////////////////////////////////////////////////////////////////////////*/
//                                                                            //
//                         Funções TCliente                                   //
//                                                                            //
/*//////////////////////////////////////////////////////////////////////////////*/

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
           + sizeof(int);      // cpf
}

// Retorna a quantidade de registros no arquivo
int tamanho_arquivo_Cliente(FILE *arq)
{
    fseek(arq, 0, SEEK_END);
    int tam = trunc(ftell(arq) / tamanhoCliente());
    return tam;
}

TCliente *buscaSequencialCliente(int chave, FILE *in, FILE *log)
{

    TCliente *c;
    int achou;
    int cont = 0;
    clock_t inicio, fim;
    double total;

    rewind(in);

    inicio = clock();

    while ((c = leCliente(in)) != NULL)
    {

        cont++;

        if (c->cod == chave)
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
        return c;
    }

    else
    {
        fprintf(log, "Comparacoes Sequencial: %d ", cont);
        fim = clock();
        total = (fim - inicio) / CLOCKS_PER_SEC;
        fprintf(log, "Tempo Sequencial: %f ", cont);
    }

    free(c);
    return NULL;
}

TCliente *busca_binariaCliente(int chave, FILE *in, int inicio, int fim, FILE *log)
{

    TCliente *c = NULL;
    int cod = -1;
    int cont = 0;
    clock_t inicioT, fimT;
    double total;

    inicioT = clock();

    while (inicio <= fim && cod != chave)
    {

        int meio = trunc((inicio + fim) / 2);
        // printf("Inicio: %d; Fim: %d; Meio: %d\n", inicio, fim, meio);
        fseek(in, (meio - 1) * tamanhoCliente(), SEEK_SET);
        c = leCliente(in);
        cod = c->cod;

        cont++;

        if (c)
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
        return c;
    }
    else
        return NULL;
}

void criarBaseCliente(FILE *out, int tam)
{
    int vet[tam];
    TCliente *c;

    for (int i = 0; i < tam; i++)
    {
        vet[i] = i + 1;
    }

    // shuffle(vet, tam, (tam*60)/100);
    embaralhaCliente(vet, tam);

    printf("\nGerando a base de dados...\n");

    for (int i = 0; i < tam; i++)
    {
        c = cliente(vet[i], "Fulano", 111 * i);
        salvaCliente(c, out);
    }
    free(c);
}

void ImprimirBaseCliente(FILE *out)
{
    printf("\nImprimindo a base de dados...\n");
    rewind(out);
    TCliente *c;

    while ((c = leCliente(out)) != NULL)
    {
        imprimeCliente(c);
    }
    free(c);
}

void embaralhaCliente(int *vet, int tam)
{
    int tmp;
    srand(time(NULL));
    int trocas = (tam * 60) / 100;

    for (int t = 1; t < trocas; t++)
    {
        int i = rand() % tam;
        int j = rand() % tam;
        tmp = vet[i];
        vet[i] = vet[j];
        vet[j] = tmp;
    }
}

void ShellsortCliente(FILE *arq, int tam)
{
    int h = 1;

    // Encontrar o maior valor de h possível (sequência de Knuth: h = 3*h + 1)
    do
    {
        h = h * 3 + 1;
    } while (h < tam);

    // Realiza a ordenação com a técnica de Shell Sort
    do
    {
        h /= 3;

        // Para cada elemento a partir da posição h+1
        for (int j = h + 1; j <= tam; j++)
        {
            // Lê o registro j (elemento a ser inserido)
            fseek(arq, (j - 1) * tamanhoCliente(), SEEK_SET);
            TCliente *fj = leCliente(arq);

            int i = j - h;

            // Lê o registro i (para comparação)
            fseek(arq, (i - 1) * tamanhoCliente(), SEEK_SET);
            TCliente *fi = leCliente(arq);

            // Move elementos maiores que fj para a direita (com intervalo h)
            while (i > 0 && fi->cod > fj->cod)
            {
                // Move fi para a posição i + h
                fseek(arq, (i + h - 1) * tamanhoCliente(), SEEK_SET);
                salvaCliente(fi, arq);

                // Próxima posição a verificar
                i -= h;

                // Se ainda há elementos para verificar, lê o próximo
                if (i > 0)
                {
                    free(fi); // Libera a memória do registro anterior
                    fseek(arq, (i - 1) * tamanhoCliente(), SEEK_SET);
                    fi = leCliente(arq);
                }
            }

            // Posiciona no local correto para salvar o registro j
            fseek(arq, (i + h - 1) * tamanhoCliente(), SEEK_SET);
            salvaCliente(fj, arq);

            // Libera memória
            free(fj);
            if (i > 0)
                free(fi);
        }
    } while (h > 1);

    // Descarrega o buffer para garantir que os dados sejam salvos
    fflush(arq);
}

/*//////////////////////////////////////////////////////////////////////////////*/
//                                                                            //
//                         Funções TCamisa                                    //
//                                                                            //
/*//////////////////////////////////////////////////////////////////////////////*/

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

void embaralhaCamisa(int *vet, int tam)
{
    int tmp;
    srand(time(NULL));
    int trocas = (tam * 60) / 100;

    for (int t = 1; t < trocas; t++)
    {
        int i = rand() % tam;
        int j = rand() % tam;
        tmp = vet[i];
        vet[i] = vet[j];
        vet[j] = tmp;
    }
}

// Shell Sort para TCamisa
void ShellsortCamisa(FILE *arq, int tam)
{
    int h = 1;

    // Encontrar o maior valor de h possível (sequência de Knuth: h = 3*h + 1)
    do
    {
        h = h * 3 + 1;
    } while (h < tam);

    // Realiza a ordenação com a técnica de Shell Sort
    do
    {
        h /= 3;

        // Para cada elemento a partir da posição h+1
        for (int j = h + 1; j <= tam; j++)
        {
            // Lê o registro j (elemento a ser inserido)
            fseek(arq, (j - 1) * tamanhoCamisa(), SEEK_SET);
            TCamisa *fj = leCamisa(arq);

            int i = j - h;

            // Lê o registro i (para comparação)
            fseek(arq, (i - 1) * tamanhoCamisa(), SEEK_SET);
            TCamisa *fi = leCamisa(arq);

            // Move elementos maiores que fj para a direita (com intervalo h)
            while (i > 0 && fi->cod > fj->cod)
            {
                // Move fi para a posição i + h
                fseek(arq, (i + h - 1) * tamanhoCamisa(), SEEK_SET);
                salvaCamisa(fi, arq);

                // Próxima posição a verificar
                i -= h;

                // Se ainda há elementos para verificar, lê o próximo
                if (i > 0)
                {
                    free(fi); // Libera a memória do registro anterior
                    fseek(arq, (i - 1) * tamanhoCamisa(), SEEK_SET);
                    fi = leCamisa(arq);
                }
            }

            // Posiciona no local correto para salvar o registro j
            fseek(arq, (i + h - 1) * tamanhoCamisa(), SEEK_SET);
            salvaCamisa(fj, arq);

            // Libera memória
            free(fj);
            if (i > 0)
                free(fi);
        }
    } while (h > 1);

    // Descarrega o buffer para garantir que os dados sejam salvos
    fflush(arq);
}

/*//////////////////////////////////////////////////////////////////////////////*/
//                                                                            //
//                         Funções TPedido                                    //
//                                                                            //
/*//////////////////////////////////////////////////////////////////////////////*/

// Cria um pedido. Lembrar de usar free(pedido)
TPedido *pedido(int cod, TCliente *cliente, TCamisa *camisa)
{
    TPedido *pedido = (TPedido *)malloc(sizeof(TPedido));
    if (pedido)
        memset(pedido, 0, sizeof(TPedido));

    // Copia os valores para os campos de pedido
    pedido->cod = cod;
    pedido->cliente = *cliente; // Copiar os dados do cliente
    pedido->camisa = *camisa;   // Copiar os dados da camisa

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
    return sizeof(int)        // cod
           + sizeof(TCliente) // cliente
           + sizeof(TCamisa); // camisa
}

// Retorna a quantidade de registros no arquivo
int tamanho_arquivo_Pedido(FILE *arq)
{
    fseek(arq, 0, SEEK_END);
    int tam = trunc(ftell(arq) / tamanhoPedido());
    return tam;
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

void embaralhaPedido(int *vet, int tam)
{
    int tmp;
    srand(time(NULL));
    int trocas = (tam * 60) / 100;

    for (int t = 1; t < trocas; t++)
    {
        int i = rand() % tam;
        int j = rand() % tam;
        tmp = vet[i];
        vet[i] = vet[j];
        vet[j] = tmp;
    }
}

// Shell Sort para TPedido
void ShellsortPedido(FILE *arq, int tam)
{
    int h = 1;

    // Encontrar o maior valor de h possível (sequência de Knuth: h = 3*h + 1)
    do
    {
        h = h * 3 + 1;
    } while (h < tam);

    // Realiza a ordenação com a técnica de Shell Sort
    do
    {
        h /= 3;

        // Para cada elemento a partir da posição h+1
        for (int j = h + 1; j <= tam; j++)
        {
            // Lê o registro j (elemento a ser inserido)
            fseek(arq, (j - 1) * tamanhoPedido(), SEEK_SET);
            TPedido *fj = lePedido(arq);

            int i = j - h;

            // Lê o registro i (para comparação)
            fseek(arq, (i - 1) * tamanhoPedido(), SEEK_SET);
            TPedido *fi = lePedido(arq);

            // Move elementos maiores que fj para a direita (com intervalo h)
            while (i > 0 && fi->cod > fj->cod)
            {
                // Move fi para a posição i + h
                fseek(arq, (i + h - 1) * tamanhoPedido(), SEEK_SET);
                salvaPedido(fi, arq);

                // Próxima posição a verificar
                i -= h;

                // Se ainda há elementos para verificar, lê o próximo
                if (i > 0)
                {
                    free(fi); // Libera a memória do registro anterior
                    fseek(arq, (i - 1) * tamanhoPedido(), SEEK_SET);
                    fi = lePedido(arq);
                }
            }

            // Posiciona no local correto para salvar o registro j
            fseek(arq, (i + h - 1) * tamanhoPedido(), SEEK_SET);
            salvaPedido(fj, arq);

            // Libera memória
            free(fj);
            if (i > 0)
                free(fi);
        }
    } while (h > 1);

    // Descarrega o buffer para garantir que os dados sejam salvos
    fflush(arq);
}

#endif