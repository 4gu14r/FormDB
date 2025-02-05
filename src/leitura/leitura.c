#include "leitura.h"
#include "../lib/pessoas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/cores.h"
#include <unistd.h>

int compararNomes(const void *a, const void *b) {
    const Pessoa *p1 = (const Pessoa*)a;
    const Pessoa *p2 = (const Pessoa*)b;
    return strcmp(p1->nome, p2->nome);
}

void executar_leitura() {
    FILE *arquivo = fopen("data/lista.csv", "r");
    if (!arquivo) {
        printf(RED "Arquivo não encontrado!\n" RESET);
        return;
    }

    Pessoa *pessoas = NULL;
    int quantidade = 0;
    char linha[256];

    // Ignora o cabeçalho do CSV
    fgets(linha, sizeof(linha), arquivo);

    while (fgets(linha, sizeof(linha), arquivo)) {
        Pessoa p;
        char *token = strtok(linha, ",");
        strcpy(p.nome, token);

        token = strtok(NULL, ",");
        strcpy(p.dataNascimento, token);

        token = strtok(NULL, ",");
        strcpy(p.sexo, token);

        token = strtok(NULL, ",\n");
        p.maiorIdade = (strcmp(token, "Sim") == 0);

        pessoas = realloc(pessoas, (quantidade + 1) * sizeof(Pessoa));
        pessoas[quantidade] = p;
        quantidade++;
    }

    fclose(arquivo);

    // Ordena as pessoas por nome
    qsort(pessoas, quantidade, sizeof(Pessoa), compararNomes);

    printf("\n\nCarregando dados ...\n");
    sleep(3);

    printf(GREEN "\n===== PESSOAS REGISTRADAS =====\n\n" RESET);
    sleep(1);

    for (int i = 0; i < quantidade; i++) {
        printf(GREEN ">> Pessoa %d\n" RESET, i + 1);
        printf("-------------------------------\n");
        printf("Nome:                %s\n", pessoas[i].nome);
        printf("Data de Nascimento:  %s\n", pessoas[i].dataNascimento);
        printf("Sexo:                %s\n", pessoas[i].sexo);
        printf("Maior de Idade:      %s\n", pessoas[i].maiorIdade ? "Sim" : "Não");
        printf("-------------------------------\n\n");
    }

    printf("Aperte Enter para continuar...");
    getchar();

    free(pessoas);
}
