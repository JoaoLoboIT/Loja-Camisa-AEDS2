#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "../Clientes/TCliente.c"
#include "../Camisa/TCamisa.c"

void menu()
{
    printf("\nEscolha uma opção:\n");
    printf("1 - Imprimir base de Camisas\n");
    printf("2 - Imprimir base de Cliente\n");
    printf("3 - Fazer busca sequencial de Camisas\n");
    printf("4 - Fazer busca binária de Camisas\n");
    printf("5 - Fazer busca sequencial de Clientes\n");
    printf("6 - Fazer busca binária de Clientes\n");
    printf("0 - Sair\n");
    printf("Digite a opção desejada: ");
}

int main()
{
#ifdef _WIN32
    system("chcp 65001");
#endif

    FILE *arqClientes = fopen("cliente.dat", "w+b");
    FILE *arqCamisas = fopen("camisa.dat", "w+b");
    FILE *arqPedidos = fopen("pedido.dat", "w+b");
    FILE *log;

    if (arqClientes == NULL || arqCamisas == NULL || arqPedidos == NULL)
    {
        printf("Erro ao abrir um dos arquivos\n");
        exit(1);
    }

    // Definir tamanho da base
    int TAMANHO_BASE;

    printf("Qual o tamanho da base de dados (clientes e camisas)? ");
    scanf("%d", &TAMANHO_BASE); // Solicita ao usuário o tamanho da base

    // Cria base de Camisas e Clientes
    criarBaseCamisa(arqCamisas, TAMANHO_BASE);
    // ImprimirBaseCamisa(arqCamisas);
    criarBaseCliente(arqClientes, TAMANHO_BASE);

    int opcao;
    do
    {
        menu();
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            ImprimirBaseCamisa(arqCamisas);
            break;

        case 2:
            ImprimirBaseCliente(arqClientes);
            break;

        case 3:
            TCamisa *ka1;
            int aux1;

            printf("Digite o código da camisa para buscar: ");
            scanf("%d", &aux1);

            // Buscar e imprimir camisas
            ka1 = buscaSequencialCamisa(aux1, arqCamisas, log);
            if (ka1 != NULL)
            {
                imprimeCamisa(ka1); // Imprime a camisa encontrada
            }
            else
            {
                printf("\nCamisa não encontrada!\n"); // Mensagem quando não encontrar a camisa
            }
            break;

        case 4:
            TCamisa *ka2;
            int aux2;

            printf("Digite o código da camisa para buscar: ");
            scanf("%d", &aux2);

            // Buscar e imprimir camisas
            ka2 = busca_binariaCamisa(aux2, arqCamisas, 0, tamanho_arquivo_Camisa(arqCamisas) - 1, log);
            if (ka2 != NULL)
            {
                imprimeCamisa(ka2); // Imprime a camisa encontrada
            }
            else
            {
                printf("\nCamisa não encontrada!\n"); // Mensagem quando não encontrar a camisa
            }
            break;

        case 5:
            TCliente *c1;
            int aux3;

            printf("Digite o código do cliente para buscar: ");
            scanf("%d", &aux3);
            c1 = buscaSequencialCliente(aux3, arqClientes, log);
            if (c1 != NULL)
            {
                imprimeCliente(c1);
            }
            else
            {
                printf("\nCliente não encontrado!\n");
            }
            break;
        
        case 6:
            TCliente *c2;
            int aux4;

            printf("Digite o código do cliente para buscar: ");
            scanf("%d", &aux4);
            c2 = busca_binariaCliente(aux4, arqClientes, 0, tamanho_arquivo_Cliente(arqClientes) - 1, log);
            if (c2 != NULL)
            {
                imprimeCliente(c2);
            }
            else
            {
                printf("\nCliente não encontrado!\n");
            }
            break;

        case 0:
            printf("Saindo...\n");
            break;

        default:
            printf("Opção inválida! Tente novamente.\n");
            break;
        }
    } while (opcao != 0);

    // Fechar arquivos
    fclose(arqClientes);
    fclose(arqCamisas);
    fclose(arqPedidos);
    fclose(log);

    return 0;
}
