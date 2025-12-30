/* /workspaces/RegistroPessoas/src/ui/form_browser.c */
// Habilita usleep (necessário com -std=c11)
<<<<<<< HEAD
=======
#define _XOPEN_SOURCE 600
>>>>>>> cd0de64bb0fd98426ae2df168632884f345e13cd
#define _DEFAULT_SOURCE

#include "form_browser.h"
#include "../utils/colors.h"
#include "../utils/ui_utils.h"
#include "../utils/file_utils.h"
<<<<<<< HEAD
=======
#include "../utils/app_context.h"
>>>>>>> cd0de64bb0fd98426ae2df168632884f345e13cd
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h> // Para access()
#include <ctype.h>

#ifdef _WIN32
#include <windows.h>
#endif

// Helper para obter lista de formulários em string
static void obter_lista_formularios(char *buffer, size_t size) {
    buffer[0] = '\0';
<<<<<<< HEAD
    DIR *d;
    struct dirent *dir;
    d = opendir("data/forms");
=======
    
        // Resolve caminho absoluto para orientar o usuário
        char abs_path[1024] = {0};
        #ifdef _WIN32
            // _fullpath converte ".\tools\..." para "C:\Program Files\..."
            if (_fullpath(abs_path, APP.forms, sizeof(abs_path)) == NULL) strncpy(abs_path, APP.forms, sizeof(abs_path)-1);
        #else
            if (realpath(APP.forms, abs_path) == NULL) strncpy(abs_path, APP.forms, sizeof(abs_path)-1);
        #endif
    

    char msg[1200];
    snprintf(msg, sizeof(msg), DIM "Local: " RESET BLUE "%s" RESET "\n" DIM "Adicione arquivos .form nesta pasta para aparecerem aqui.\n" RESET, abs_path);
    if (strlen(buffer) + strlen(msg) < size - 1) strcat(buffer, msg);

    DIR *d;
    struct dirent *dir;
    d = opendir(APP.forms);
>>>>>>> cd0de64bb0fd98426ae2df168632884f345e13cd
    
    strcat(buffer, "\n");
    
    int count = 0;
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) continue;
            
            char *dot = strrchr(dir->d_name, '.');
            if (dot && strcmp(dot, ".form") == 0) {
                char line[300];
                snprintf(line, sizeof(line), CYAN "  • %s\n" RESET, dir->d_name);
                if (strlen(buffer) + strlen(line) < size - 100) {
                    strcat(buffer, line);
                }
                count++;
            }
        }
        closedir(d);
    }
    
    if (count == 0) {
        strcat(buffer, YELLOW "  (Nenhum formulário encontrado)\n" RESET);
    }
    strcat(buffer, "\n");
}

void listar_formularios() {
    char buffer[4096];
    obter_lista_formularios(buffer, sizeof(buffer));
    
    limpar_tela();
    desenhar_cabecalho("FORMULÁRIOS CADASTRADOS");
    printf("%s", buffer);
}

Form* selecionar_formulario_interativo() {
    char list_buffer[4096];
    char prompt_buffer[5000];
    char filename[100];
    
    // Prepara lista
    obter_lista_formularios(list_buffer, sizeof(list_buffer));
    
    // Monta prompt
    snprintf(prompt_buffer, sizeof(prompt_buffer), "%sDigite o nome do arquivo (ex: produtos.form) ou ENTER para voltar: ", list_buffer);
    
    // Input responsivo
    ler_texto_dialogo("FORMULÁRIOS CADASTRADOS", prompt_buffer, filename, sizeof(filename));
    
    if (strlen(filename) == 0) {
        return NULL;
    }
    
    // Adiciona extensão se necessário
    if (strlen(filename) < 5 || strcmp(filename + strlen(filename) - 5, ".form") != 0) {
        strcat(filename, ".form");
    }
    
    // Usa a função utilitária para encontrar o arquivo correto
<<<<<<< HEAD
    char filepath[300];
    bool encontrado = encontrar_arquivo_case_insensitive("data/forms", filename, filepath, sizeof(filepath));
=======
    char filepath[1024];
    bool encontrado = encontrar_arquivo_case_insensitive(APP.forms, filename, filepath, sizeof(filepath));
>>>>>>> cd0de64bb0fd98426ae2df168632884f345e13cd
    
    if (encontrado) {
        Form *form = carregar_formulario(filepath);
        if (form) {
            printf(GREEN "\n✓ Formulário '%s' carregado com sucesso!\n" RESET, form->displayName);
            #ifdef _WIN32
            Sleep(1000);
            #else
            usleep(1000000);
            #endif
            return form;
        } else {
            while(1) {
                limpar_tela();
                desenhar_cabecalho("ERRO");
                printf(RED "\n✗ Erro ao carregar formulário!\n" RESET);
                printf("\nPressione ENTER para continuar...");
                if (esperar_enter_check_resize()) continue;
                break;
            }
        }
    } else {
        while(1) {
            limpar_tela();
            desenhar_cabecalho("FORMULÁRIOS CADASTRADOS");
            printf("%s", list_buffer);
            printf(RED "\n✗ Arquivo não encontrado: %s\n" RESET, filepath);
            printf("\nPressione ENTER para continuar...");
            if (esperar_enter_check_resize()) continue;
            break;
        }
    }
    
    return NULL; // Retorna NULL em caso de erro, o main mantém o form anterior se houver
}
