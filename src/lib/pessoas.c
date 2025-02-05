#include "pessoas.h"
#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include <string.h>

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

int validarDataNascimento(const char *data) {
    if (strlen(data) != 10) return 0;
    if (data[2] != '/' || data[5] != '/') return 0;
    for (int i = 0; i < 10; i++) {
        if (i != 2 && i != 5 && !isdigit(data[i])) return 0;
    }
    return 1;
}