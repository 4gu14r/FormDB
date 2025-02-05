#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pessoas.h"

// Função de comparação para o quicksort
int compararNomes(const void *a, const void *b) {
    const Pessoa *p1 = (const Pessoa*)a;
    const Pessoa *p2 = (const Pessoa*)b;
    return strcmp(p1->nome, p2->nome);
}

int main() {
    FILE *arquivo = fopen("lista.csv", "r");
    if (!arquivo) {
        printf("Arquivo 'lista.csv' não encontrado!\n");
        return 1;
    }

    // Ignora cabeçalho
    char linha[256];
    fgets(linha, sizeof(linha), arquivo);

    // Lê todas as pessoas
    Pessoa *pessoas = NULL;
    int quantidade = 0;

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

    // Ordena com quicksort
    qsort(pessoas, quantidade, sizeof(Pessoa), compararNomes);

    // Exibe os dados ordenados
    printf("\nDados ordenados:\n");
    for (int i = 0; i < quantidade; i++) {
        printf("Nome: %s\n", pessoas[i].nome);
        printf("Data de Nascimento: %s\n", pessoas[i].dataNascimento);
        printf("Sexo: %s\n", pessoas[i].sexo);
        printf("Maior de Idade: %s\n\n", pessoas[i].maiorIdade ? "Sim" : "Não");
    }

    free(pessoas);
    return 0;
}