#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "src/lib/pessoas.h"

#define RED "\033[1;31m"
#define RESET "\033[0m"

typedef struct TreeNode {
    Pessoa pessoa;
    struct TreeNode *esquerda;
    struct TreeNode *direita;
} TreeNode;

TreeNode* inserirNaArvore(TreeNode *raiz, Pessoa p) {
    if (!raiz) {
        TreeNode *novo = malloc(sizeof(TreeNode));
        novo->pessoa = p;
        novo->esquerda = novo->direita = NULL;
        return novo;
    }
    if (strcmp(p.nome, raiz->pessoa.nome) < 0) {
        raiz->esquerda = inserirNaArvore(raiz->esquerda, p);
    } else {
        raiz->direita = inserirNaArvore(raiz->direita, p);
    }
    return raiz;
}

void salvarEmOrdem(TreeNode *raiz, FILE *arquivo) {
    if (!raiz) return;
    salvarEmOrdem(raiz->esquerda, arquivo);
    fprintf(arquivo, "%s,%s,%s,%s\n",
        raiz->pessoa.nome,
        raiz->pessoa.dataNascimento,
        raiz->pessoa.sexo,
        raiz->pessoa.maiorIdade ? "Sim" : "Não"
    );
    salvarEmOrdem(raiz->direita, arquivo);
}

void liberarArvore(TreeNode *raiz) {
    if (!raiz) return;
    liberarArvore(raiz->esquerda);
    liberarArvore(raiz->direita);
    free(raiz);
}

int validarDataNascimento(const char *data) {
    if (strlen(data) != 10) return 0;
    if (data[2] != '/' || data[5] != '/') return 0;
    for (int i = 0; i < 10; i++) {
        if (i != 2 && i != 5 && !isdigit(data[i])) return 0;
    }
    return 1;
}

int main() {
    TreeNode *raiz = NULL;
    char opcao;

    struct stat st = {0};
    if (stat("data", &st) == -1) {
        mkdir("data", 0700);
    }

    do {
        Pessoa p;
        printf("\nNome: ");
        fgets(p.nome, sizeof(p.nome), stdin);
        p.nome[strcspn(p.nome, "\n")] = '\0';

        do {
            printf("Data de Nascimento (dd/mm/aaaa): ");
            fgets(p.dataNascimento, sizeof(p.dataNascimento), stdin);
            p.dataNascimento[strcspn(p.dataNascimento, "\n")] = '\0';
            if (!validarDataNascimento(p.dataNascimento)) {
                printf(RED "Formato inválido! Tente novamente.\n" RESET);
                limparBufferEntrada();
            }
        } while (!validarDataNascimento(p.dataNascimento));

        limparBufferEntrada();

        char sexoEntrada[10];
        int sexoOpcao;
        do {
            printf("Sexo (1 - Masculino, 2 - Feminino): ");
            fgets(sexoEntrada, sizeof(sexoEntrada), stdin);
            if (sscanf(sexoEntrada, "%d", &sexoOpcao) != 1 || (sexoOpcao != 1 && sexoOpcao != 2)) {
                printf(RED "Opção inválida! Tente novamente.\n" RESET);
                sexoOpcao = 0;
            }
        } while (sexoOpcao != 1 && sexoOpcao != 2);

        strcpy(p.sexo, (sexoOpcao == 1) ? "Masculino" : "Feminino");

        p.maiorIdade = (calcularIdade(p.dataNascimento) >= 18);

        raiz = inserirNaArvore(raiz, p);

        printf("Continuar? (s/n): ");
        opcao = tolower(getchar());
        limparBufferEntrada();
    } while (opcao == 's');

    FILE *arquivo = fopen("data/lista.csv", "w");
    if (arquivo) {
        fprintf(arquivo, "Nome,Data de Nascimento,Sexo,Maior de Idade\n");
        salvarEmOrdem(raiz, arquivo);
        fclose(arquivo);
        printf("\nArquivo 'data/lista.csv' gerado!\n");
    } else {
        printf("Erro ao salvar arquivo!\n");
    }

    liberarArvore(raiz);
    return 0;
}
