#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "../Clientes/TCliente.c"
#include "../Camisa/TCamisa.c"


int main()
{
#ifdef _WIN32
    system("chcp 65001");
#endif

    FILE *arq;
    FILE *out;
    FILE *log;

    TCliente *c1, *c2;
    TCamisa *ka1, *ka2;
    int qtdparticoes;

    // TMetadados metadados;
    // metadados.topo_vagos = -1;

    /*if ((out = fopen("intercalado.dat", "w+b")) == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }*/

    if ((log = fopen("log.txt", "a+")) == NULL)
    {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }

    if ((arq = fopen("cliente.dat", "w+b")) == NULL)
    {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }

    else
    {
        criarBaseCamisa(arq, 5);
        ImprimirBaseCamisa(arq);
        
        // ******************************************************************************************************************************
        // CRIA E IMPRIME BASE DO CLIETE 
        //criarBaseCliente(arq, 5);
        //ImprimirBaseCliente(arq);
        // ******************************************************************************************************************************


        // ANOTAÇÕES TATIANA 
        // insertionSortContando(arq,tamanho_arquivo(arq), log);
        // classificacao_interna(arq, 10);
        // printf("\n-------------------------------------------------\n");
        // printf("\nCLASSIFICAÇÃO DEPOIS DO INSERTION\n");
        // printf("\n-------------------------------------------------\n");
        // ImprimirBase(arq);
        // printf("\n\n\nORDENANDO ORDENANDO ORDENANDO ORDENANDO ORDENANDO ORDENANDO ORDENANDO\n\n\n");
        // system("pause");
        // f = buscaSequencial(450000, arq, log);
        // imprime(f);


        // ******************************************************************************************************************************
        // TESTE DE CLIENTES
        // c1 = busca_binariaCliente(3, arq, 0, tamanho_arquivo(arq) - 1, log);
        // imprimeCliente(c1);
        // c2 = buscaSequencialCliente(3, arq, log);
        // imprimeCliente(c2);
        // ******************************************************************************************************************************
        
        ka1 = busca_binariaCamisa(2, arq, 0, tamanho_arquivo_Camisa(arq) - 1, log);
        imprimeCamisa(ka1);
        ka2 = buscaSequencialCamisa(3, arq, log);
        imprimeCamisa(ka2);


        // ANOTAÇÕES TATIANA
        // free(f);
        // insertionSort(arq,tamanho_arquivo(arq));
        // imprimirBase(arq);
        // qtdparticoes = classificacao_interna(arq, 10);
        // printf("%d\n\n\n\n\n\n\n\n\n", qtdparticoes);
        // intercalacao_basica(out, qtdparticoes);
        // imprimirBase(out);
        
    }
}
