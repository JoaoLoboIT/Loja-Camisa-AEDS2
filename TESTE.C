// #include <stdio.h>
// #include <stdlib.h>
// #include <time.h>
// #include <math.h>
// #include "../Clientes/TCliente.c"
// #include "../Camisa/TCamisa.c"
// #include "../Pedido/TPedido.c"

// void menu() {
//     printf("\nEscolha uma opção:\n");
//     printf("1 - Imprimir base de Camisas\n");
//     printf("2 - Imprimir base de Clientes\n");
//     printf("3 - Buscar Camisa (Sequencial)\n");
//     printf("4 - Buscar Cliente (Sequencial)\n");
//     printf("5 - Buscar Pedido (Sequencial)\n");
//     printf("6 - Buscar Camisa (Binária)\n");
//     printf("7 - Buscar Cliente (Binária)\n");
//     printf("8 - Buscar Pedido (Binária)\n");
//     printf("0 - Sair\n");
//     printf("Digite a opção desejada: ");
// }

// int main()
// {
// #ifdef _WIN32
//     system("chcp 65001");
// #endif

//     FILE *arq, *log;
//     TCliente *c1, *c2;
//     TCamisa *ka1, *ka2;
//     TPedido *pedido1, *pedido2;
    
//     if ((log = fopen("log.txt", "a+")) == NULL)
//     {
//         printf("Erro ao abrir arquivo\n");
//         exit(1);
//     }

//     if ((arq = fopen("cliente.dat", "w+b")) == NULL)
//     {
//         printf("Erro ao abrir arquivo\n");
//         exit(1);
//     }

//     // Criando as bases de Camisas e Clientes com 10 camisas e 5 clientes
//     criarBaseCamisa(arq, 10);
//     ImprimirBaseCamisa(arq);

//     criarBaseCliente(arq, 5);
//     ImprimirBaseCliente(arq);

//     int opcao;
//     do {
//         menu();
//         scanf("%d", &opcao);

//         switch (opcao) {
//             case 1:
//                 ImprimirBaseCamisa(arq);
//                 break;
//             case 2:
//                 ImprimirBaseCliente(arq);
//                 break;
//             case 3:
//                 printf("Buscar Camisa Sequencial:\n");
//                 ka1 = buscasequencialcamisa(2, arq, log);
//                 imprimeCamisa(ka1);
//                 break;
//             case 4:
//                 printf("Buscar Cliente Sequencial:\n");
//                 c1 = buscasequencialcliente(3, arq, log);
//                 imprimeCliente(c1);
//                 break;
//             case 5:
//                 printf("Buscar Pedido Sequencial:\n");
//                 pedido1 = buscasequencialpedido(1, arq, log);
//                 imprimePedido(pedido1);
//                 break;
//             case 6:
//                 printf("Buscar Camisa Binária:\n");
//                 ka2 = buscabinariacamisa(3, arq, 0, tamanho_arquivo_Camisa(arq) - 1, log);
//                 imprimeCamisa(ka2);
//                 break;
//             case 7:
//                 printf("Buscar Cliente Binária:\n");
//                 c2 = buscabinariacliente(3, arq, 0, tamanho_arquivo(arq) - 1, log);
//                 imprimeCliente(c2);
//                 break;
//             case 8:
//                 printf("Buscar Pedido Binária:\n");
//                 pedido2 = buscabinariopedido(1, arq, 0, tamanho_arquivo_Pedido(arq) - 1, log);
//                 imprimePedido(pedido2);
//                 break;
//             case 0:
//                 printf("Saindo...\n");
//                 break;
//             default:
//                 printf("Opção inválida! Tente novamente.\n");
//                 break;
//         }
//     } while (opcao != 0);

//     // Limpeza de memória
//     fclose(arq);
//     fclose(log);
//     return 0;
// }
