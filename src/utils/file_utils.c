#include "file_utils.h"
#include "string_utils.h"
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

bool encontrar_arquivo_case_insensitive(const char *base_dir, const char *filename, char *out_path, size_t size) {
    char temp_path[512];
    snprintf(temp_path, sizeof(temp_path), "%s/%s", base_dir, filename);

    // 1. Tenta acesso direto (caminho exato)
    if (access(temp_path, F_OK) != -1) {
        snprintf(out_path, size, "%s", temp_path);
        return true;
    }

    // 2. Busca no diretório ignorando case
    DIR *d = opendir(base_dir);
    if (!d) return false;

    struct dirent *dir;
    bool found = false;

    while ((dir = readdir(d)) != NULL) {
        if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) continue;

        if (str_equal_ignore_case(filename, dir->d_name)) {
            snprintf(out_path, size, "%s/%s", base_dir, dir->d_name);
            found = true;
            break;
        }
    }
    closedir(d);
    return found;
}