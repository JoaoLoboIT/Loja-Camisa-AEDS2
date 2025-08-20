#include "Entidades/entidades.c"

void menu_hash()
{
    printf("\n--- MENU DE TESTES HASH ---\n");
    printf("1 - Inserir Novo Cliente\n");
    printf("2 - Buscar Cliente por Codigo\n");
    printf("3 - Remover Cliente por Codigo\n");
    printf("4 - Imprimir Tabela Hash Completa\n");
    printf("5 - Imprimir Base de Dados de Clientes\n");
    printf("0 - Voltar ao Menu Principal\n"); // ALTERAÇÃO: Melhor descrição
    printf("Digite a opção desejada: ");
}

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
    printf("10 - Imprimir bases de pedidos\n");
    printf("11 - Ordenar base cliente\n");
    printf("12 - Ordenar base camisa\n");
    printf("13 - Ordenar base pedido\n");
    printf("14 - Ordenar Base Clientes (Selecao Natural + Intercalacao Otima)\n");
    printf("15 - Embaralhar Base de Clientes\n");
    printf("16 - Menu Hash\n");
    printf("0 - Sair\n");
    printf("Digite a opção desejada: ");
}

int main()
{
#ifdef _WIN32
    system("chcp 65001");
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

    FILE *arq_hash_clientes = fopen("dat/hash_clientes.dat", "r+b");
    if (arq_hash_clientes == NULL)
    {
        arq_hash_clientes = fopen("dat/hash_clientes.dat", "w+b");
    }

    if (arqClientes == NULL || arqCamisas == NULL || arqPedidos == NULL || arq_hash_clientes == NULL)
    {
        printf("Erro ao abrir um dos arquivos de dados\n");
        exit(1);
    }

    int TAMANHO_BASE;
    printf("Qual o tamanho da base de dados inicial (clientes e camisas)? ");
    scanf("%d", &TAMANHO_BASE);

    // Cria as bases de dados
    criarBaseCamisa(arqCamisas, TAMANHO_BASE);
    criarBaseCliente(arqClientes, TAMANHO_BASE);

    // Constrói a tabela hash a partir dos dados recém-criados
    construir_hash_da_base_existente(arq_hash_clientes, arqClientes);
    imprimir_tabela_hash_completa(arq_hash_clientes, arqClientes);

    int M = 6;
    int F = 5;
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
            scanf("%d", &cpf);

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
            int codPedido;
            printf("Digite o código do pedido: ");
            scanf("%d", &codPedido);

            // Criar Pedido com base nos IDs de cliente e camisa usando busca sequencial
            TPedido *novoPedido = pedido(codPedido, arqClientes, arqCamisas, log);
            if (novoPedido)
            {
                salvaPedido(novoPedido, arqPedidos);
                printf("\nPedido criado com sucesso!\n");
                imprimePedido(novoPedido);

                // Libera memória
                free(novoPedido);
            }
            break;
        }
        case 10:
        {
            ImprimirBasePedido(arqPedidos);
            break;
        }

        case 11:
        {

            int tamanho_base_clientes = tamanho_arquivo_Cliente(arqClientes);
            ShellsortCliente(arqClientes, tamanho_base_clientes);
            printf("\nBase de clientes ordenada com sucesso!\n");
            break;
        }
        case 12:
        {
            int tamanho_base_camisas = tamanho_arquivo_Camisa(arqCamisas);
            ShellsortCamisa(arqCamisas, tamanho_base_camisas);
            printf("\nBase de camisas ordenada com sucesso!\n");
            break;
        }
        case 13:
        {
            int tamanho_base_pedidos = tamanho_arquivo_Pedido(arqPedidos);
            ShellsortPedido(arqPedidos, tamanho_base_pedidos);
            printf("Base de pedidos ordenada com sucesso!\n");
            break;
        }
        case 14:
        {
            printf("\n-- INICIANDO ORDENACAO EXTERNA COMPLETA --\n");

            // ETAPA 1: Gerar partições ordenadas
            printf("Etapa 1: Gerando particoes com Selecao Natural...\n");
            int num_particoes = selecao_natural_TCliente(arqClientes, M);

            if (num_particoes >= 0)
            {
                printf(" > Foram geradas %d particoes.\n", num_particoes);

                // ETAPA 2: Intercalar as partições
                printf("Etapa 2: Intercalando particoes com Intercalacao Otima...\n");
                intercalacao_otima_TCliente(F, num_particoes, arqClientes);
                printf(" > Intercalacao concluida! O arquivo 'cliente.dat' foi ordenado.\n");
            }
            else
            {
                printf("\nOcorreu um erro durante a Selecao Natural.\n");
            }

            printf("-- ORDENACAO EXTERNA FINALIZADA --\n");
            break;
        }
        case 15:
        {
            printf("\nEmbaralhando a base de clientes...\n");
            // Chama a função existente para recriar e embaralhar a base
            criarBaseCliente(arqClientes, TAMANHO_BASE);
            printf("Base de clientes embaralhada com sucesso!\n");
            break;
        }
        case 16:
        {
            int opcao_hash;
            do
            {
                menu_hash();
                scanf("%d", &opcao_hash);
                switch (opcao_hash)
                {
                case 1:
                {
                    // ALTERAÇÃO: Chamando a nova função de interface correta
                    inserir_novo_cliente_hash(arq_hash_clientes, arqClientes);
                    break;
                }
                case 2:
                {
                    int cod_cliente;
                    printf("Digite o código do cliente a buscar: ");
                    scanf("%d", &cod_cliente);
                    TCliente *cliente_encontrado = buscar_cliente_hash(cod_cliente, arq_hash_clientes, arqClientes);
                    if (cliente_encontrado != NULL)
                    {
                        imprimeCliente(cliente_encontrado);
                        free(cliente_encontrado);
                    }
                    else
                    {
                        printf("Cliente não encontrado na tabela hash.\n");
                    }
                    break;
                }
                case 3:
                {
                    int cod_cliente;
                    printf("Digite o código do cliente a remover: ");
                    scanf("%d", &cod_cliente);
                    remover_cliente_hash(cod_cliente, arq_hash_clientes, arqClientes);
                    break;
                }
                case 4:
                {
                    imprimir_tabela_hash_completa(arq_hash_clientes, arqClientes);
                    break;
                }
                case 5:
                {
                    ImprimirBaseCliente(arqClientes);
                    break;
                }
                case 0:
                    printf("Voltando ao menu principal...\n");
                    break;
                default:
                    printf("Opção do menu hash inválida!\n");
                }
            } while (opcao_hash != 0);
            break; // Break do case 16
        }

        case 0:
            printf("Saindo...\n");
            break;

        default:
            printf("Opção inválida! Tente novamente.\n");
        }
    } while (opcao != 0);

    fclose(arqClientes);
    fclose(arqCamisas);
    fclose(arqPedidos);
    fclose(log);
    fclose(arq_hash_clientes);
    return 0;
}