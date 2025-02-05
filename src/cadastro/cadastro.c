#include "cadastro.h"
#include "../lib/pessoas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>
#include "../lib/cores.h"
#include <unistd.h>

static TreeNode* inserirNaArvore(TreeNode *raiz, Pessoa p) {
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

static void salvarEmOrdem(TreeNode *raiz, FILE *arquivo) {
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

static void liberarArvore(TreeNode *raiz) {
    if (!raiz) return;
    liberarArvore(raiz->esquerda);
    liberarArvore(raiz->direita);
    free(raiz);
}

void executar_cadastro() {
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

        printf("\nContinuar? (s/n): ");
        opcao = tolower(getchar());
        limparBufferEntrada();
    } while (opcao == 's');

    FILE *arquivo = fopen("data/lista.csv", "w");
    if (arquivo) {
        fprintf(arquivo, "Nome,Data de Nascimento,Sexo,Maior de Idade\n");
        salvarEmOrdem(raiz, arquivo);
        fclose(arquivo);
        printf(GREEN "\nArquivo 'data/lista.csv' gerado!\n" RESET);
    } else {
        printf(RED "Erro ao salvar arquivo!\n" RESET);
    }

    sleep(3);

    liberarArvore(raiz);
}