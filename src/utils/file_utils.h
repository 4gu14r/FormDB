#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <stdbool.h>
#include <stddef.h>

// Busca um arquivo no diretório ignorando case e retorna o caminho completo em out_path
bool encontrar_arquivo_case_insensitive(const char *base_dir, const char *filename, char *out_path, size_t size);

#endif