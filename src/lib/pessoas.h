#ifndef PESSOAS_H
#define PESSOAS_H

#include <stdbool.h>

typedef struct {
    char nome[100];
    char dataNascimento[11]; // dd/mm/aaaa
    char sexo[10];
    bool maiorIdade;
} Pessoa;


// Funções compartilhadas
int calcularIdade(const char *dataNascimento);
void limparBufferEntrada();

#endif