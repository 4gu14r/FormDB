#ifndef CADASTRO_H
#define CADASTRO_H

#include "../lib/pessoas.h"

typedef struct TreeNode {
    Pessoa pessoa;
    struct TreeNode *esquerda;
    struct TreeNode *direita;
} TreeNode;

// Funções de cadastro
void executar_cadastro();

#endif