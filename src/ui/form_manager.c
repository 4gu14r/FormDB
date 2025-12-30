#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "../core/form.h"
#include "../utils/colors.h"
#include "../utils/ui_utils.h"
#include "../utils/string_utils.h"
<<<<<<< HEAD
=======
#include "../utils/app_context.h"
>>>>>>> cd0de64bb0fd98426ae2df168632884f345e13cd
#include "form_browser.h"

void gerenciar_editar(char *msgErro) {
    Form *form = selecionar_formulario_interativo();
    if (!form) {
        if (msgErro) snprintf(msgErro, 512, YELLOW "\n⚠ Nenhuma seleção realizada.\n" RESET);
        return;
    }
    
    char prompt[512];
    snprintf(prompt, sizeof(prompt), 
        "\nEditando: " BOLD_CYAN "%s" RESET "\n"
        DIM "Nota: Apenas o nome de exibição pode ser alterado para manter a integridade dos dados.\n" RESET
        "\nNovo Nome de Exibição (Enter para manter '%s'): ", 
        form->displayName, form->displayName);

    char buffer[100];
    ler_texto_dialogo("EDITAR FORMULÁRIO", prompt, buffer, sizeof(buffer));
    
    if (strlen(buffer) > 0) {
        strncpy(form->displayName, buffer, sizeof(form->displayName) - 1);
        
        printf(YELLOW "\nSalvando alterações...\n" RESET);
        // Tenta salvar. Se salvar_formulario não estiver implementado no core, isso falhará na linkagem
        // ou precisará de um stub.
<<<<<<< HEAD
        char filepath[300];
        snprintf(filepath, sizeof(filepath), "data/forms/%s.form", form->name);
=======
        char filepath[1024];
        snprintf(filepath, sizeof(filepath), "%s/%s.form", APP.forms, form->name);
>>>>>>> cd0de64bb0fd98426ae2df168632884f345e13cd
        if (salvar_formulario(form, filepath)) {
            if (msgErro) snprintf(msgErro, 512, GREEN "\n✓ Formulário '%s' atualizado com sucesso!\n" RESET, form->displayName);
            else printf(GREEN "✓ Formulário atualizado com sucesso!\n" RESET);
        } else {
            if (msgErro) snprintf(msgErro, 512, RED "\n✗ Erro ao salvar formulário.\n" RESET);
            else printf(RED "✗ Erro ao salvar formulário.\n" RESET);
        }
    } else {
        if (msgErro) snprintf(msgErro, 512, YELLOW "\nNenhuma alteração feita em '%s'.\n" RESET, form->displayName);
        else printf(YELLOW "Nenhuma alteração feita.\n" RESET);
    }
    
    liberar_formulario(form);
}

void gerenciar_duplicar(char *msgErro) {
    Form *origem = selecionar_formulario_interativo();
    if (!origem) {
        if (msgErro) snprintf(msgErro, 512, YELLOW "\n⚠ Nenhuma seleção realizada.\n" RESET);
        return;
    }
    
    char prompt[512];
    snprintf(prompt, sizeof(prompt), 
        "\nFormulário selecionado: %s (%s)\n"
        "Novo nome interno (sem espaços, ex: produtos_v2): ", 
        origem->displayName, origem->name);

    char novo_nome_interno[50];
    ler_texto_dialogo("DUPLICAR FORMULÁRIO", prompt, novo_nome_interno, sizeof(novo_nome_interno));
    
    // Atualiza estrutura na memória
    char novo_display[150];
    snprintf(novo_display, sizeof(novo_display), "%s (Cópia)", origem->displayName);
    
    // Configura a cópia
    strncpy(origem->name, novo_nome_interno, sizeof(origem->name) - 1);
    strncpy(origem->displayName, novo_display, sizeof(origem->displayName) - 1);
    origem->totalRecords = 0; // Zera contagem de registros para o novo form
    
    printf(YELLOW "Criando cópia...\n" RESET);
    
<<<<<<< HEAD
    char filepath[300];
    snprintf(filepath, sizeof(filepath), "data/forms/%s.form", origem->name);
=======
    char filepath[1024];
    snprintf(filepath, sizeof(filepath), "%s/%s.form", APP.forms, origem->name);
>>>>>>> cd0de64bb0fd98426ae2df168632884f345e13cd
    if (salvar_formulario(origem, filepath)) {
        if (msgErro) snprintf(msgErro, 512, GREEN "\n✓ Formulário duplicado: %s\n" RESET, novo_nome_interno);
        else printf(GREEN "\n✓ Formulário duplicado com sucesso!\n" RESET);
    } else {
        if (msgErro) snprintf(msgErro, 512, RED "\n✗ Erro ao salvar o novo formulário.\n" RESET);
        else printf(RED "\n✗ Erro ao salvar o novo formulário.\n" RESET);
    }
    
    liberar_formulario(origem);
}

void gerenciar_excluir(char *msgErro) {
    Form *form = selecionar_formulario_interativo();
    if (!form) {
        if (msgErro) snprintf(msgErro, 512, YELLOW "\n⚠ Nenhuma seleção realizada.\n" RESET);
        return;
    }
    
    char prompt[512];
    snprintf(prompt, sizeof(prompt), 
        RED "\n⚠ ATENÇÃO: Excluir '%s' apagará a DEFINIÇÃO e TODOS OS DADOS!\n" RESET
        "Esta ação não pode ser desfeita.\n"
        "Digite 'CONFIRMAR' para apagar: ", form->displayName);

    char confirm[20];
    ler_texto_dialogo("EXCLUIR FORMULÁRIO", prompt, confirm, sizeof(confirm));
    
    if (str_equal_ignore_case(confirm, "CONFIRMAR")) {
<<<<<<< HEAD
        char path_form[256];
        char path_json[256];
        char path_csv[256];
        
        // Define caminhos
        snprintf(path_form, sizeof(path_form), "data/forms/%s.form", form->name);
        snprintf(path_json, sizeof(path_json), "data/forms/%s.json", form->name);
        snprintf(path_csv, sizeof(path_csv), "data/records/%s.csv", form->name);
=======
        // AppContext *app = get_app_context(); // APP is a global variable
        char path_form[1024];
        char path_json[1024];
        char path_csv[1024];
        
        // Define caminhos
        snprintf(path_form, sizeof(path_form), "%s/%s.form", APP.forms, form->name);
        snprintf(path_json, sizeof(path_json), "%s/%s.json", APP.forms, form->name);
        snprintf(path_csv, sizeof(path_csv), "%s/%s.csv", APP.records, form->name);
>>>>>>> cd0de64bb0fd98426ae2df168632884f345e13cd
        
        // Remove arquivos
        int r1 = remove(path_form);
        int r2 = remove(path_json); // Tenta remover json também caso exista
        remove(path_csv);
        
        if (r1 == 0 || r2 == 0) {
            if (msgErro) snprintf(msgErro, 512, GREEN "\n✓ Formulário '%s' excluído.\n" RESET, form->displayName);
            else printf(GREEN "\n✓ Formulário excluído.\n" RESET);
        } else {
            if (msgErro) snprintf(msgErro, 512, RED "\n✗ Erro ao excluir arquivo de formulário.\n" RESET);
            else printf(RED "\n✗ Erro ao excluir arquivo de formulário (pode não existir).\n" RESET);
        }
        
        // if (r3 == 0) printf(GREEN "✓ Dados excluídos.\n" RESET);
    } else {
        if (msgErro) snprintf(msgErro, 512, YELLOW "\nOperação de exclusão cancelada.\n" RESET);
        else printf(YELLOW "\nOperação cancelada.\n" RESET);
    }
    
    liberar_formulario(form);
}

void menu_gerenciar_formularios() {
    int opcao;
    char msgErro[512] = "";
    
    do {
        limpar_tela();
        desenhar_cabecalho("GERENCIAR FORMULÁRIOS");
        printf("\n");
        desenhar_separador();
        printf("   1. " BLUE "✏️  Editar Metadados\n" RESET);
        printf("   2. " BLUE "📑 Duplicar Formulário\n" RESET);
        printf("   3. " RED  "🗑️  Excluir Formulário\n" RESET);
        printf("   0. " WHITE  "🔙 Voltar\n" RESET);
        desenhar_separador();
        printf("\n");
        if (msgErro[0] != '\0') {
            printf("%s", msgErro);
            msgErro[0] = '\0';
        }
        
        printf("\n" BOLD_WHITE "Opção: " RESET);
        
        int result = scanf("%d", &opcao);
        
        if (result == EOF && errno == EINTR) {
            continue;
        }
        
        if (result != 1) {
            limpar_buffer();
            snprintf(msgErro, sizeof(msgErro), RED "\n✗ Opção inválida!\n" RESET);
            continue;
        }
        limpar_buffer();
        
        switch(opcao) {
            case 1: gerenciar_editar(msgErro); break;
            case 2: gerenciar_duplicar(msgErro); break;
            case 3: gerenciar_excluir(msgErro); break;
            case 0: break;
            default: snprintf(msgErro, sizeof(msgErro), RED "\n✗ Opção inválida.\n" RESET);
        }
    } while (opcao != 0);
}