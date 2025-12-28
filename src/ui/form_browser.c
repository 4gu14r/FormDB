/* /workspaces/RegistroPessoas/src/ui/form_browser.c */
#include "form_browser.h"
#include "../utils/colors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h> // Para access()

void listar_formularios() {
    printf(BOLD_CYAN "\n╔════════════════════════════════════════════════╗\n" RESET);
    printf(BOLD_CYAN "║          FORMULÁRIOS CADASTRADOS               ║\n" RESET);
    printf(BOLD_CYAN "╚════════════════════════════════════════════════╝\n" RESET);
    
    DIR *d;
    struct dirent *dir;
    d = opendir("data/forms");
    if (d) {
        int count = 0;
        while ((dir = readdir(d)) != NULL) {
            if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) continue;
            
            char *dot = strrchr(dir->d_name, '.');
            if (dot && strcmp(dot, ".form") == 0) {
                printf(CYAN "  • %s\n" RESET, dir->d_name);
                count++;
            }
        }
        closedir(d);
        
        if (count == 0) {
            printf(YELLOW "  (Nenhum formulário encontrado)\n" RESET);
        }
    } else {
        printf(RED "  Erro ao abrir diretório data/forms\n" RESET);
    }
    printf("\n");
}

Form* selecionar_formulario_interativo() {
    listar_formularios();
    
    char filename[100];
    printf("Digite o nome do arquivo (ex: produtos.form) ou ENTER para voltar: ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = '\0';
    
    if (strlen(filename) == 0) {
        return NULL;
    }
    
    // Adiciona extensão se necessário
    if (strlen(filename) < 5 || strcmp(filename + strlen(filename) - 5, ".form") != 0) {
        strcat(filename, ".form");
    }
    
    char filepath[300];
    snprintf(filepath, sizeof(filepath), "data/forms/%s", filename);
    
    if (access(filepath, F_OK) != -1) {
        Form *form = carregar_formulario(filepath);
        if (form) {
            printf(GREEN "\n✓ Formulário '%s' carregado com sucesso!\n" RESET, form->displayName);
            return form;
        } else {
            printf(RED "\n✗ Erro ao carregar formulário!\n" RESET);
        }
    } else {
        printf(RED "\n✗ Arquivo não encontrado: %s\n" RESET, filepath);
    }
    
    printf("\nPressione ENTER para continuar...");
    getchar();
    
    return NULL; // Retorna NULL em caso de erro, o main mantém o form anterior se houver
}
