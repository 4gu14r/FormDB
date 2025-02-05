#include "pessoas.h"
#include <stdio.h>
#include <time.h>

int calcularIdade(const char *dataNascimento) {
    int dia, mes, ano;
    if (sscanf(dataNascimento, "%d/%d/%d", &dia, &mes, &ano) != 3) {
        return -1;
    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    return tm.tm_year + 1900 - ano;
}

void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}