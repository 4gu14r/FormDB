#include "file_utils.h"
#include "string_utils.h"
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

#ifdef _WIN32
    #include <direct.h>
#endif

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

bool arquivo_existe(const char *path) {
    return access(path, F_OK) != -1;
}

bool diretorio_existe(const char *path) {
    struct stat st;
    if (stat(path, &st) == 0) {
        return S_ISDIR(st.st_mode);
    }
    return false;
}

bool criar_diretorio(const char *path) {
    if (diretorio_existe(path)) return true;
    
    #ifdef _WIN32
        return _mkdir(path) == 0;
    #else
        return mkdir(path, 0755) == 0;
    #endif
}

void extrair_diretorio_pai(const char *full_path, char *out_dir, size_t size) {
    strncpy(out_dir, full_path, size - 1);
    out_dir[size - 1] = '\0';
    
    char *last_sep = strrchr(out_dir, '/');
    #ifdef _WIN32
    char *last_sep_win = strrchr(out_dir, '\\');
    if (last_sep_win > last_sep) last_sep = last_sep_win;
    #endif
    
    if (last_sep) {
        *last_sep = '\0'; // Corta a string no separador
    } else {
        // Se não tem separador, assume diretório atual "."
        out_dir[0] = '.';
        out_dir[1] = '\0';
    }
}