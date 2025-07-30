#ifndef ENTIDADES_C
#define ENTIDADES_C

#include "entidades.h"

/*//////////////////////////////////////////////////////////////////////////////*/
//                                                                            //
//                 TADS FILA PARA FAZER INTERCALAÇÃO ÓTIMA                    //
//                                                                            //
/*//////////////////////////////////////////////////////////////////////////////*/

typedef struct estrutura
{
    char particao[40];
} TStruct;

typedef struct celula
{
    TStruct particao;
    struct celula *prox;
} TCelula;

typedef struct fila
{
    TCelula *frente;
    TCelula *tras;
    int tamanho;
} TFila;

/*      FUNÇÕES DE MANIPULAÇÃO      */

void FFVazia(TFila *Fila)
{
    Fila->frente = (TCelula *)malloc(sizeof(TCelula));
    Fila->tras = Fila->frente;
    Fila->frente->prox = NULL;
    Fila->tamanho = 0;
}

int FilaVazia(TFila fila)
{
    return (fila.frente == fila.tras);
}

void Enfileirar(TStruct particao, TFila *Fila)
{
    Fila->tras->prox = (TCelula *)malloc(sizeof(TCelula));
    Fila->tras = Fila->tras->prox;
    Fila->tras->particao = particao;
    Fila->tras->prox = NULL;
    Fila->tamanho++;
}

void Desenfileirar(TFila *fila, TStruct *particao)
{
    TCelula *aux;
    if (!FilaVazia(*fila))
    {
        aux = fila->frente->prox;
        fila->frente->prox = aux->prox;
        *particao = aux->particao;
        free(aux);
        if (fila->frente->prox == NULL)
        {
            fila->tras = fila->frente;
        }
        fila->tamanho--;
    }
}

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

void salvar_log_shellsort(const char *descricao, long long int comparacoes, double tempo)
{
    FILE *logs = fopen("log/log_shellsort.txt", "a"); // Salva em um arquivo separado
    if (logs == NULL)
    {
        printf("Erro ao abrir o ficheiro de logs do Shell Sort.\n");
        return;
    }
    fprintf(logs, "*****************************************\n");
    fprintf(logs, "%s:\n", descricao);
    fprintf(logs, "Numero de comparacoes: %lld\n", comparacoes);
    fprintf(logs, "Tempo de execucao: %.4f segundos\n\n", tempo);
    fclose(logs);
}

void ShellsortCliente(FILE *arq, int tam)
{
    long long int comparacoes = 0;
    clock_t inicio, fim;
    double tempo_execucao;

    inicio = clock(); // Inicia o cronômetro
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

            if (fi != NULL)
            { // Garante que não estamos comparando um ponteiro nulo
                comparacoes++;
            }

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
                    if (fi != NULL)
                    {
                        comparacoes++;
                    }
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
    fim = clock();
    tempo_execucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    salvar_log_shellsort("Shellsort - TCliente (Sua Versao)", comparacoes, tempo_execucao);

    printf("\nShell Sort concluido! Resultados salvos em 'log/log_shellsort.txt'\n");
}

// Função AUXILIAR de SELEÇÃO NATURAL para verificar se o array em memória está vazio
int is_array_empty_TCliente(TCliente **array, int tamanho)
{
    for (int i = 0; i < tamanho; i++)
    {
        if (array[i] != NULL)
        {
            return 0; // Não está vazio
        }
    }
    return 1; // Está vazio
}

// Função AUXILIAR de SELEÇÃO NATURAL para salvar logs
// Função para salvar logs (pode ser útil)
void salvar_informacoes_logs_sn(const char *descricao, int particoes, double tempo)
{
    FILE *logs = fopen("log/logs_classificacao.txt", "a");
    if (logs == NULL)
    {
        printf("Erro ao abrir o ficheiro de logs.\n");
        return;
    }
    fprintf(logs, "*****************************************\n");
    fprintf(logs, "%s:\n", descricao);
    fprintf(logs, "Numero de particoes criadas: %d\n", particoes);
    fprintf(logs, "Tempo de execucao: %.2f segundos\n\n", tempo);
    fclose(logs);
}

void imprimir_conteudo_particao(const char *nome_arquivo, const char *titulo)
{
    printf("\n%s: %s\n", titulo, nome_arquivo);
    printf(" > Conteudo [ ");

    FILE *particao = fopen(nome_arquivo, "rb");
    if (particao == NULL)
    {
        printf("ERRO AO ABRIR PARA LEITURA! ]\n");
        return;
    }

    TCliente *c;
    while ((c = leCliente(particao)) != NULL)
    {
        printf("%d ", c->cod);
        free(c);
    }

    printf("]\n");
    fclose(particao);
}

// Função PRINCIPAL da Seleção Natural para a entidade TCliente
int selecao_natural_TCliente(FILE *arq, int M)
{
    double tempoInicial = clock();

    int qtdParticoes = 0;
    TCliente *registros[M]; // Array em memória principal
    char nomeParticao[40];

    int reg_lidos_total = 0;
    int tam_total_arquivo = tamanho_arquivo_Cliente(arq);

    rewind(arq);

    FILE *reservatorio = fopen("dat/reservatorio_cliente.dat", "w+b");
    if (reservatorio == NULL)
    {
        printf("Erro ao criar arquivo de reservatorio\n");
        return -1;
    }

    // 1. Carrega a memória inicial com M registros
    for (int i = 0; i < M; i++)
    {
        registros[i] = leCliente(arq);
        if (registros[i] == NULL)
            break;
        reg_lidos_total++;
    }

    // Loop principal: continua enquanto houver registros a serem lidos
    while (reg_lidos_total < tam_total_arquivo)
    {
        // Cria um novo arquivo de partição
        sprintf(nomeParticao, "dat/particao_cliente_%d.dat", qtdParticoes);
        FILE *part = fopen(nomeParticao, "w+b");
        if (part == NULL)
        {
            printf("Erro ao criar arquivo de particao\n");
            fclose(reservatorio);
            return -1;
        }

        // Loop para gerar UMA partição
        int registros_no_reservatorio = 0;
        fseek(reservatorio, 0, SEEK_END); // vai para o final do reservatorio
        registros_no_reservatorio = ftell(reservatorio) / tamanhoCliente();

        while (registros_no_reservatorio < M && reg_lidos_total < tam_total_arquivo)
        {
            TCliente *menor = NULL;
            int posicao_menor = -1;

            // Encontra o menor registro na memória
            for (int j = 0; j < M; j++)
            {
                if (registros[j] != NULL)
                {
                    if (menor == NULL || registros[j]->cod < menor->cod)
                    {
                        menor = registros[j];
                        posicao_menor = j;
                    }
                }
            }

            if (menor == NULL)
                break;

            salvaCliente(menor, part);
            TCliente *ultimo_salvo = menor;

            // Substitui o registro salvo por um novo
            TCliente *novo = leCliente(arq);

            if (novo != NULL)
            {
                reg_lidos_total++;
                if (novo->cod < ultimo_salvo->cod)
                {
                    salvaCliente(novo, reservatorio);
                    fseek(reservatorio, 0, SEEK_END);
                    registros_no_reservatorio = ftell(reservatorio) / tamanhoCliente();
                    free(novo); // Libera o 'novo' que foi para o reservatório
                    registros[posicao_menor] = NULL;
                }
                else
                {
                    registros[posicao_menor] = novo;
                }
            }
            else
            {
                registros[posicao_menor] = NULL;
            }
        }

        qtdParticoes++;
        fclose(part);
        imprimir_conteudo_particao(nomeParticao, "Particao da Selecao Natural Criada");

        fseek(reservatorio, 0, SEEK_END);
        if (ftell(reservatorio) == 0)
            break;

        // Prepara para a próxima partição
        rewind(reservatorio);
        for (int i = 0; i < M; i++)
        {
            if (registros[i] != NULL)
                free(registros[i]); // Libera o que sobrou na memória
            registros[i] = leCliente(reservatorio);
        }

        fclose(reservatorio);
        reservatorio = fopen("dat/reservatorio_cliente.dat", "w+b");
    }

    // Esvaziamento final
    if (!is_array_empty_TCliente(registros, M))
    {
        sprintf(nomeParticao, "dat/particao_cliente_%d.dat", qtdParticoes);
        FILE *part = fopen(nomeParticao, "w+b");

        while (!is_array_empty_TCliente(registros, M))
        {
            TCliente *menor = NULL;
            int posicao_menor = -1;

            for (int i = 0; i < M; i++)
            {
                if (registros[i] != NULL)
                {
                    if (menor == NULL || registros[i]->cod < menor->cod)
                    {
                        menor = registros[i];
                        posicao_menor = i;
                    }
                }
            }
            if (menor)
            {
                salvaCliente(menor, part);
                free(registros[posicao_menor]); // Libera o ponteiro que foi salvo
                registros[posicao_menor] = NULL;
            }
        }
        qtdParticoes++;
        fclose(part);
        imprimir_conteudo_particao(nomeParticao, "Particao Final (Resto da Memoria) Criada");
    }

    double tempoFinal = clock();
    double tempoDeExecucao = (tempoFinal - tempoInicial) / CLOCKS_PER_SEC;

    salvar_informacoes_logs_sn("Selecao Natural - TCliente", qtdParticoes, tempoDeExecucao);
    fclose(reservatorio);

    return qtdParticoes;
}

// Função AUXILIAR de INTERCALAÇÃO ÓTIMA para salvar logs
// Função para log da intercalação
void salvar_informacoes_logs_intercalacao(const char *descricao, int passadas, double tempo)
{
    FILE *logs = fopen("log/logs_intercalacao.txt", "a");
    if (logs == NULL)
    {
        printf("Erro ao abrir o ficheiro de logs.\n");
        return;
    }
    fprintf(logs, "*****************************************\n%s: \nNumero de comparações: %d\nTempo de execucao: %.2f segundos\n\n", descricao, passadas, tempo);
    fclose(logs);
}

// Função AUXILIAR de INTERCALAÇÃO ÓTIMA para copiar o arquivo final ordenado de volta para o original
void copiar_dados_cliente(FILE *origem, FILE *destino)
{
    rewind(origem);
    rewind(destino);
    // Trunca o arquivo de destino para garantir que ele fique limpo
    ftruncate(fileno(destino), 0);

    TCliente *c;
    while ((c = leCliente(origem)) != NULL)
    {
        salvaCliente(c, destino);
        free(c);
    }
    fflush(destino); // Garante que tudo foi salvo em disco
}

// Função PRINCIPAL da INTERCALAÇÃO ÓTIMA, adaptada para TCliente
void intercalacao_otima_TCliente(int F, int total_particoes, FILE *arq_principal)
{
    char nomeParticao[40];
    double tempoInicial = clock();
    int particao_atual_id = total_particoes;

    TFila fila;
    FFVazia(&fila);

    // Array de ponteiros para os arquivos de entrada da intercalação
    FILE **arqs_entrada = (FILE **)malloc(F * sizeof(FILE *));
    // Array de memória para o menor registro de cada arquivo de entrada
    TCliente *buffer[F];

    // Salvando o nome
    // 1. Enfileirar todas as partições iniciais criadas pela Seleção Natural
    for (int i = 0; i < total_particoes; i++)
    {
        sprintf(nomeParticao, "dat/particao_cliente_%d.dat", i); // ADAPTADO: caminho e nome
        TStruct p;
        strcpy(p.particao, nomeParticao);
        Enfileirar(p, &fila);
    }

    // 2. Loop principal: continua enquanto houver mais de uma partição na fila
    while (fila.tamanho > 1)
    {
        // Define o número de partições a intercalar nesta passada
        int num_a_intercalar = (fila.tamanho < F) ? fila.tamanho : F;

        // Abre os arquivos de entrada e preenche o buffer inicial
        for (int i = 0; i < num_a_intercalar; i++)
        {
            TStruct p_aux;
            Desenfileirar(&fila, &p_aux);
            arqs_entrada[i] = fopen(p_aux.particao, "rb");
            if (arqs_entrada[i] != NULL)
            {
                buffer[i] = leCliente(arqs_entrada[i]); // ADAPTADO: leCliente
            }
            else
            {
                buffer[i] = NULL;
            }
        }

        // Cria a nova partição de saída para esta passada
        sprintf(nomeParticao, "dat/particao_cliente_%d.dat", particao_atual_id);
        FILE *saida = fopen(nomeParticao, "w+b");

        // 3. Loop da passada: encontra o menor, salva, e substitui no buffer
        while (1)
        {
            TCliente *menor = NULL;
            int menor_indice = -1;

            // Encontra o menor registro no buffer
            for (int i = 0; i < num_a_intercalar; i++)
            {
                if (buffer[i] != NULL)
                {
                    if (menor == NULL || buffer[i]->cod < menor->cod)
                    { // ADAPTADO: usa ->cod
                        menor = buffer[i];
                        menor_indice = i;
                    }
                }
            }

            if (menor_indice == -1)
            { // Se não encontrou nenhum menor, a passada acabou
                break;
            }

            salvaCliente(menor, saida); // ADAPTADO: salvaCliente
            free(menor);                // Libera o ponteiro do menor que foi salvo

            // Lê o próximo registro do mesmo arquivo de onde veio o menor
            buffer[menor_indice] = leCliente(arqs_entrada[menor_indice]);
        }

        // Fecha todos os arquivos de entrada e o de saída desta passada
        for (int i = 0; i < num_a_intercalar; i++)
        {
            if (arqs_entrada[i] != NULL)
                fclose(arqs_entrada[i]);
        }
        fclose(saida);
        imprimir_conteudo_particao(nomeParticao, "Particao de Intercalacao Criada");

        // Adiciona a nova partição (maior) de volta na fila
        TStruct p_nova;
        strcpy(p_nova.particao, nomeParticao);
        Enfileirar(p_nova, &fila);
        particao_atual_id++;
    }

    // 4. Finalização: Copia a última partição (que é o arquivo completo e ordenado) para o arquivo original
    TStruct p_final;
    Desenfileirar(&fila, &p_final);
    FILE *arq_final_ordenado = fopen(p_final.particao, "rb");

    copiar_dados_cliente(arq_final_ordenado, arq_principal);

    fclose(arq_final_ordenado);
    free(arqs_entrada);
    free(fila.frente); // Libera a cabeça da fila

    double tempoFinal = clock();
    double tempoDeExecucao = (tempoFinal - tempoInicial) / CLOCKS_PER_SEC;
    salvar_informacoes_logs_intercalacao("Intercalacao Otima - TCliente", (particao_atual_id - total_particoes), tempoDeExecucao);
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
// Função para criar Pedido com base em Cliente e Camisa encontrados via busca sequencial
TPedido *pedido(int cod, FILE *arqClientes, FILE *arqCamisas, FILE *log)
{
    int codCliente, codCamisa;

    // Solicitar os códigos do cliente e da camisa
    printf("Digite o código do cliente: ");
    scanf("%d", &codCliente);

    // Realizar busca sequencial para cliente
    TCliente *cliente = buscaSequencialCliente(codCliente, arqClientes, log);
    if (cliente == NULL)
    {
        printf("\nCliente não encontrado!\n");
        return NULL;
    }

    printf("Digite o código da camisa: ");
    scanf("%d", &codCamisa);

    // Realizar busca sequencial para camisa
    TCamisa *camisa = buscaSequencialCamisa(codCamisa, arqCamisas, log);
    if (camisa == NULL)
    {
        printf("\nCamisa não encontrada!\n");
        free(cliente);
        return NULL;
    }

    // Criar pedido
    TPedido *novoPedido = (TPedido *)malloc(sizeof(TPedido));
    if (novoPedido)
    {
        novoPedido->cod = cod;
        novoPedido->cliente = *cliente;
        novoPedido->camisa = *camisa;
    }

    // Libera memória dos objetos cliente e camisa
    free(cliente);
    free(camisa);

    return novoPedido;
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