#include "Entidades/entidades.c"

void menu()
{
    printf("\nEscolha uma opção:\n");
    printf("1 - Imprimir base de Camisas\n");
    printf("2 - Imprimir base de Cliente\n");
    printf("3 - Fazer busca sequencial de Camisas\n");
    printf("4 - Fazer busca binária de Camisas\n");
    printf("5 - Fazer busca sequencial de Clientes\n");
    printf("6 - Fazer busca binária de Clientes\n");
    printf("7 - Criar Cliente\n");
    printf("8 - Criar Camisa\n");
    printf("9 - Criar Pedido\n");
    printf("0 - Sair\n");
    printf("Digite a opção desejada: ");
}

int main()
{
    #ifdef _WIN32
    system("chcp 65001"); // Configura a codificação para UTF-8
    #endif

    FILE *arqClientes = fopen("dat/cliente.dat", "r+b");
    if (arqClientes == NULL)
        arqClientes = fopen("dat/cliente.dat", "w+b");

    FILE *arqCamisas = fopen("dat/camisa.dat", "r+b");
    if (arqCamisas == NULL)
        arqCamisas = fopen("dat/camisa.dat", "w+b");

    FILE *arqPedidos = fopen("dat/pedido.dat", "r+b");
    if (arqPedidos == NULL)
        arqPedidos = fopen("dat/pedido.dat", "w+b");

    FILE *log = fopen("log/log.txt", "w");
    if (log == NULL)
    {
        printf("Erro ao criar arquivo de log\n");
        exit(1);
    }

    if (arqClientes == NULL || arqCamisas == NULL || arqPedidos == NULL)
    {
        printf("Erro ao abrir um dos arquivos\n");
        exit(1);
    }

    // Definir tamanho da base
    int TAMANHO_BASE;

    printf("Qual o tamanho da base de dados (clientes e camisas)? ");
    scanf("%d", &TAMANHO_BASE);

    // Cria base de Camisas e Clientes
    criarBaseCamisa(arqCamisas, TAMANHO_BASE);
    criarBaseCliente(arqClientes, TAMANHO_BASE);
    TCamisa *ka = camisa(0, 0, "", "");
    salvaCamisa(ka, arqCamisas);
    TCliente *c = cliente(0, "", 0);
    salvaCliente(c, arqClientes);

    TPedido *pedido1 = pedido(0, c, ka);

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
        {
            TCamisa *ka1;
            int aux1;

            printf("Digite o código da camisa para buscar: ");
            scanf("%d", &aux1);

            ka1 = buscaSequencialCamisa(aux1, arqCamisas, log);
            if (ka1 != NULL)
            {
                imprimeCamisa(ka1);
                free(ka1);
            }
            else
            {
                printf("\nCamisa não encontrada!\n");
            }
            break;
        }

        case 4:
        {
            TCamisa *ka2;
            int aux2;

            printf("Digite o código da camisa para buscar: ");
            scanf("%d", &aux2);

            ka2 = busca_binariaCamisa(aux2, arqCamisas, 0, tamanho_arquivo_Camisa(arqCamisas) - 1, log);
            if (ka2 != NULL)
            {
                imprimeCamisa(ka2);
                free(ka2);
            }
            else
            {
                printf("\nCamisa não encontrada!\n");
            }
            break;
        }

        case 5:
        {
            TCliente *c1;
            int aux3;

            printf("Digite o código do cliente para buscar: ");
            scanf("%d", &aux3);
            c1 = buscaSequencialCliente(aux3, arqClientes, log);
            if (c1 != NULL)
            {
                imprimeCliente(c1);
                free(c1);
            }
            else
            {
                printf("\nCliente não encontrado!\n");
            }
            break;
        }

        case 6:
        {
            TCliente *c2;
            int aux4;

            printf("Digite o código do cliente para buscar: ");
            scanf("%d", &aux4);
            c2 = busca_binariaCliente(aux4, arqClientes, 0, tamanho_arquivo_Cliente(arqClientes) - 1, log);
            if (c2 != NULL)
            {
                imprimeCliente(c2);
                free(c2);
            }
            else
            {
                printf("\nCliente não encontrado!\n");
            }
            break;
        }

        case 7:
        {
            int cod1;
            char nome[50];
            int cpf;

            printf("Digite o código do cliente: ");
            scanf("%d", &cod1);
            getchar(); // Limpar buffer
            printf("Digite o nome do cliente: ");
            fgets(nome, sizeof(nome), stdin);
            nome[strcspn(nome, "\n")] = '\0';

            printf("Digite o CPF do cliente: ");
            scanf("%d", cpf);

            TCliente *cNovo = cliente(cod1, nome, cpf);
            salvaCliente(cNovo, arqClientes);

            printf("Novo cliente criado com sucesso!\n");
            imprimeCliente(cNovo);
            free(cNovo);
            break;
        }

        case 8:
        {
            int cod2, tipo;
            char ano[20], time[20];

            printf("Digite o código da camisa: ");
            scanf("%d", &cod2);
            printf("Digite o tipo da camisa: ");
            scanf("%d", &tipo);
            getchar(); // Limpar buffer
            printf("Digite o ano da camisa: ");
            fgets(ano, sizeof(ano), stdin);
            ano[strcspn(ano, "\n")] = '\0';

            printf("Digite o time da camisa: ");
            fgets(time, sizeof(time), stdin);
            time[strcspn(time, "\n")] = '\0';

            TCamisa *kaNovo = camisa(cod2, tipo, ano, time);
            salvaCamisa(kaNovo, arqCamisas);

            printf("Nova camisa criada com sucesso!\n");
            imprimeCamisa(kaNovo);
            free(kaNovo);
            break;
        }
        case 9:
        {
            // Criar Cliente
            int codCliente, cpf;
            char nomeCliente[50];

            printf("Digite o código do cliente: ");
            scanf("%d", &codCliente);
            getchar();
            printf("Digite o nome do cliente: ");
            fgets(nomeCliente, sizeof(nomeCliente), stdin);
            nomeCliente[strcspn(nomeCliente, "\n")] = '\0';
            printf("Digite o CPF do cliente: ");
            scanf("%d", &cpf);

            TCliente *novoCliente = cliente(codCliente, nomeCliente, cpf);
            salvaCliente(novoCliente, arqClientes);

            // Criar Camisa
            int codCamisa, tipoCamisa;
            char anoCamisa[20], timeCamisa[20];

            printf("Digite o código da camisa: ");
            scanf("%d", &codCamisa);
            printf("Digite o tipo da camisa: ");
            scanf("%d", &tipoCamisa);
            getchar();
            printf("Digite o ano da camisa: ");
            fgets(anoCamisa, sizeof(anoCamisa), stdin);
            anoCamisa[strcspn(anoCamisa, "\n")] = '\0';
            printf("Digite o time da camisa: ");
            fgets(timeCamisa, sizeof(timeCamisa), stdin);
            timeCamisa[strcspn(timeCamisa, "\n")] = '\0';

            TCamisa *novaCamisa = camisa(codCamisa, tipoCamisa, anoCamisa, timeCamisa);
            salvaCamisa(novaCamisa, arqCamisas);

            // Criar Pedido
            int codPedido;
            printf("Digite o código do pedido: ");
            scanf("%d", &codPedido);

            TPedido *novoPedido = pedido(codPedido, novoCliente, novaCamisa);
            salvaPedido(novoPedido, arqPedidos);

            printf("\nPedido criado com sucesso!\n");
            imprimePedido(novoPedido);

            // Libera memória
            free(novoCliente);
            free(novaCamisa);
            free(novoPedido);

            break;
        }
        case 10:
        {
            imprimePedido(pedido1);
            break;
        }

        case 0:
            printf("Saindo...\n");
            break;

        default:
            printf("Opção inválida! Tente novamente.\n");
        }

    } while (opcao != 0);

    // Fechar arquivos
    fclose(arqClientes);
    fclose(arqCamisas);
    fclose(arqPedidos);
    fclose(log);

    return 0;
}