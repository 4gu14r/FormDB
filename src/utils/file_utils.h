#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <stdbool.h>
#include <stddef.h>

// Busca um arquivo no diretório ignorando case e retorna o caminho completo em out_path
bool encontrar_arquivo_case_insensitive(const char *base_dir, const char *filename, char *out_path, size_t size);

// Verifica se um arquivo existe
bool arquivo_existe(const char *path);

// Verifica se um diretório existe
bool diretorio_existe(const char *path);

// Cria um diretório se não existir (retorna true se já existe ou se criou com sucesso)
bool criar_diretorio(const char *path);

// Extrai o diretório pai de um caminho completo (ex: "dados/export/arq.csv" -> "dados/export")
void extrair_diretorio_pai(const char *full_path, char *out_dir, size_t size);

#endif