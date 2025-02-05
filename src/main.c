#include <stdio.h>
#include "cadastro/cadastro.h"
#include "leitura/leitura.h"
#include "lib/cores.h"
#include <unistd.h>

void mostrar_menu() {
    int escolha;
    do {
        printf(ORANGE"\n\n====== MENU PRINCIPAL ======\n"RESET);
        printf(ORANGE"1."RESET" Cadastrar novas pessoas\n");
        printf(ORANGE"2."RESET" Ler lista registrada\n");
        printf(ORANGE"3."RESET" Sair\n");
        printf(ORANGE "============================\n" RESET);
        printf("\nEscolha: ");
        scanf("%d", &escolha);
        limparBufferEntrada();

        switch (escolha) {
            case 1:
                executar_cadastro();
                printf("\nVoltando para o menu principal...\n");
                sleep(3);
                break;
            case 2:
                executar_leitura();
                printf("Voltando para o menu principal\n");
                sleep(3);
                break;
            case 3:
                printf("Saindo...\n");
                sleep(3);
                break;
            default:
                printf(RED "Opção inválida!\n" RESET);
        }
    } while (escolha != 3);
}

int main() {
    mostrar_menu();
    return 0;
}