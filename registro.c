#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "src/lib/pessoas.h"

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

int main() {
    TreeNode *raiz = NULL;
    char opcao;

    do {
        Pessoa p;
        printf("\nNome: ");
        fgets(p.nome, sizeof(p.nome), stdin);
        p.nome[strcspn(p.nome, "\n")] = '\0';

        printf("Data de Nascimento (dd/mm/aaaa): ");
        fgets(p.dataNascimento, sizeof(p.dataNascimento), stdin);
        p.dataNascimento[strcspn(p.dataNascimento, "\n")] = '\0';

        int sexoOpcao;
        do {
            printf("Sexo (1 - Masculino, 2 - Feminino): ");
            scanf("%d", &sexoOpcao);
            limparBufferEntrada();
        } while (sexoOpcao != 1 && sexoOpcao != 2);

        strcpy(p.sexo, (sexoOpcao == 1) ? "Masculino" : "Feminino");

        p.maiorIdade = (calcularIdade(p.dataNascimento) >= 18);

        raiz = inserirNaArvore(raiz, p);

        printf("Continuar? (s/n): ");
        opcao = tolower(getchar());
        limparBufferEntrada();
    } while (opcao == 's');

    FILE *arquivo = fopen("lista.csv", "w");
    if (arquivo) {
        fprintf(arquivo, "Nome,Data de Nascimento,Sexo,Maior de Idade\n");
        salvarEmOrdem(raiz, arquivo);
        fclose(arquivo);
        printf("\nArquivo 'lista.csv' gerado!\n");
    } else {
        printf("Erro ao salvar arquivo!\n");
    }

    liberarArvore(raiz);
    return 0;
}
