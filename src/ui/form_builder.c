<<<<<<< HEAD
=======
// Habilita extensões POSIX (necessário para realpath com -std=c11)
#define _XOPEN_SOURCE 600
#define _DEFAULT_SOURCE

>>>>>>> cd0de64bb0fd98426ae2df168632884f345e13cd
#include "form_builder.h"
#include "../core/form.h"
#include "../core/field.h"
#include "../storage/json_handler.h"
#include "../utils/colors.h"
#include "../utils/ui_utils.h"
#include "../utils/string_utils.h"
<<<<<<< HEAD
=======
#include "../utils/app_context.h"
>>>>>>> cd0de64bb0fd98426ae2df168632884f345e13cd
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <dirent.h>

void exibir_tipos_campo() {
    desenhar_cabecalho("TIPOS DE CAMPO DISPONÍVEIS");
    printf("\n" GREEN "Textos:\n" RESET);
    printf("  1. Texto Curto        (até 100 caracteres)\n");
    printf("  2. Texto Longo        (até 1000 caracteres)\n");
    printf("  3. Email              (com validação)\n");
    printf("  4. URL                (links)\n");
    
    printf("\n" GREEN "Números:\n" RESET);
    printf("  5. Número Inteiro     (0, 1, 2, 3...)\n");
    printf("  6. Número Decimal     (1.5, 2.75...)\n");
    printf("  7. Dinheiro           (R$ 1.234,56)\n");
    
    printf("\n" GREEN "Datas e Horas:\n" RESET);
    printf("  8. Data               (dd/mm/aaaa)\n");
    printf("  9. Hora               (HH:MM)\n");
    printf(" 10. Data e Hora        (dd/mm/aaaa HH:MM)\n");
    
    printf("\n" GREEN "Documentos:\n" RESET);
    printf(" 11. CPF                (com validação)\n");
    printf(" 12. CNPJ               (com validação)\n");
    printf(" 13. CEP                (auto-completa endereço)\n");
    printf(" 14. Telefone           ((XX) XXXXX-XXXX)\n");
    
    printf("\n" GREEN "Escolhas:\n" RESET);
    printf(" 15. Sim/Não            (verdadeiro/falso)\n");
    printf(" 16. Lista (única)      (dropdown)\n");
    printf(" 17. Lista (múltipla)   (checkboxes)\n");
    printf(" 18. Avaliação          (★★★★★)\n");
    
    printf("\n");
    desenhar_separador();
}

FieldType selecionar_tipo_campo() {
    int opcao;
    bool erro = false;
    
    do {
        limpar_tela();
        exibir_tipos_campo();
        
        if (erro) {
            printf(RED "\nOpção inválida! Tente novamente.\n" RESET);
            erro = false;
        }
        
        printf("\nEscolha o tipo (1-18): ");
        
        int result = scanf("%d", &opcao);
        
        if (result == EOF && errno == EINTR) continue;
        
        if (result != 1) {
            limpar_buffer();
            erro = true;
            opcao = 0;
            continue;
        }
        limpar_buffer();
        
        if (opcao < 1 || opcao > 18) {
            erro = true;
        }
    } while (opcao < 1 || opcao > 18);
    
    // Mapeia opção para FieldType
    FieldType tipos[] = {
        FIELD_TEXT_SHORT, FIELD_TEXT_LONG, FIELD_EMAIL, FIELD_URL,
        FIELD_NUMBER, FIELD_DECIMAL, FIELD_MONEY,
        FIELD_DATE, FIELD_TIME, FIELD_DATETIME,
        FIELD_CPF, FIELD_CNPJ, FIELD_CEP, FIELD_PHONE,
        FIELD_BOOLEAN, FIELD_CHOICE_SINGLE, FIELD_CHOICE_MULTIPLE, FIELD_RATING
    };
    
    return tipos[opcao - 1];
}

void configurar_validacao(Field *field) {
    char opcao;
    
    printf("\n" CYAN "═══ CONFIGURAR VALIDAÇÕES ═══\n" RESET);
    
    // Campo obrigatório
    printf("\nCampo obrigatório? (s/n): ");
    opcao = ler_confirmacao();
    field->validation.required = (opcao == 's' || opcao == 'S');
    
    // Valor único
    printf("Valor deve ser único? (s/n): ");
    opcao = ler_confirmacao();
    field->validation.unique = (opcao == 's' || opcao == 'S');
    
    if (field->validation.unique) {
        printf("Ativar sugestão de auto-incremento (ex: 001 -> 002)? (s/n): ");
        opcao = ler_confirmacao();
        field->validation.autoIncrement = (opcao == 's' || opcao == 'S');
    }
    
    // Validações específicas por tipo
    switch (field->type) {
        case FIELD_NUMBER:
        case FIELD_DECIMAL:
        case FIELD_MONEY: {
            printf("\nDefinir valor mínimo? (s/n): ");
            opcao = ler_confirmacao();
            if (opcao == 's' || opcao == 'S') {
                printf("Valor mínimo: ");
                ler_int_seguro(&field->validation.minValue);
                limpar_buffer();
                field->validation.hasMin = true;
            }
            
            printf("Definir valor máximo? (s/n): ");
            opcao = ler_confirmacao();
            if (opcao == 's' || opcao == 'S') {
                printf("Valor máximo: ");
                ler_int_seguro(&field->validation.maxValue);
                limpar_buffer();
                field->validation.hasMax = true;
            }
            break;
        }
        
        case FIELD_TEXT_SHORT:
        case FIELD_TEXT_LONG: {
            printf("\nTamanho mínimo (0 para nenhum): ");
            ler_int_seguro(&field->validation.minLength);
            limpar_buffer();
            
            printf("Tamanho máximo: ");
            ler_int_seguro(&field->validation.maxLength);
            limpar_buffer();
            break;
        }
        
        default:
            break;
    }
    
    printf(GREEN "\nValidações configuradas!\n" RESET);
}

void adicionar_opcoes_escolha(Field *field) {
    if (field->type != FIELD_CHOICE_SINGLE && field->type != FIELD_CHOICE_MULTIPLE) {
        return;
    }
    
    char opcao[200];
    char prompt[100];
    int count = 0;
    
    while (1) {
        snprintf(prompt, sizeof(prompt), "\nDigite a opção %d (vazio para terminar): ", count + 1);
        
        ler_texto_dialogo("ADICIONAR OPÇÕES", prompt, opcao, sizeof(opcao));
        
        if (strlen(opcao) == 0) {
            break;
        }
        
        adicionar_opcao_escolha(field, opcao);
        count++;
    }
    
    printf(GREEN "\n%d opções adicionadas!\n" RESET, count);
}

Field* construir_campo_interativo() {
    char name[100], label[100], desc[200];
    
    // Nome interno
    ler_texto_dialogo("CRIAR NOVO CAMPO", "\nNome interno (sem espaços, ex: 'preco_produto'): ", name, sizeof(name));
    
    // Label
    ler_texto_dialogo("CRIAR NOVO CAMPO", "\nNome exibido (ex: 'Preço do Produto'): ", label, sizeof(label));
    
    // Descrição
    ler_texto_dialogo("CRIAR NOVO CAMPO", "\nDescrição/Ajuda (opcional): ", desc, sizeof(desc));
    
    // Tipo
    FieldType tipo = selecionar_tipo_campo();
    
    // Criar campo
    Field *field = criar_campo(name, label, tipo);
    if (desc[0] != '\0') {
        strncpy(field->description, desc, sizeof(field->description) - 1);
    }
    
    // Configurar validações
    char opcao = ler_confirmacao_dialogo("CONFIGURAÇÃO", "\nConfigurar validações? (s/n): ");
    if (opcao == 's' || opcao == 'S') {
        configurar_validacao(field);
    }
    
    // Se for campo de escolha, adicionar opções
    if (field->type == FIELD_CHOICE_SINGLE || field->type == FIELD_CHOICE_MULTIPLE) {
        adicionar_opcoes_escolha(field);
    }
    
    // Valor padrão
    opcao = ler_confirmacao_dialogo("CONFIGURAÇÃO", "\nDefinir valor padrão? (s/n): ");
    if (opcao == 's' || opcao == 'S') {
        printf("Valor padrão: ");
        ler_string_segura(field->defaultValue, sizeof(field->defaultValue));
    }
    
    printf(GREEN "\n✓ Campo criado com sucesso!\n" RESET);
    
    return field;
}

Form* construir_formulario_interativo() {
    char name[100], displayName[100], desc[300];
    
    // Nome do formulário
    ler_texto_dialogo("CONSTRUTOR DE FORMULÁRIOS", "\nNome interno do formulário (ex: 'produtos'): ", name, sizeof(name));
    
    // Normalizar para minúsculas para evitar problemas de case-sensitivity
    str_to_lower(name);
    
    ler_texto_dialogo("CONSTRUTOR DE FORMULÁRIOS", "\nNome exibido (ex: 'Cadastro de Produtos'): ", displayName, sizeof(displayName));
    
    ler_texto_dialogo("CONSTRUTOR DE FORMULÁRIOS", "\nDescrição (opcional): ", desc, sizeof(desc));
    
    // Criar formulário
    Form *form = criar_formulario(name, displayName);
    if (desc[0] != '\0') {
        strncpy(form->description, desc, sizeof(form->description) - 1);
    }
    
    printf(GREEN "\n✓ Formulário criado!\n" RESET);
    printf("\nAgora vamos adicionar os campos...\n");
    
    // Adicionar campos
    char continuar = 's';
    while (continuar == 's' || continuar == 'S') {
        Field *field = construir_campo_interativo();
        adicionar_campo_formulario(form, field);
        
        printf("\n" CYAN "Campo adicionado! Total de campos: %d\n" RESET, form->numFields);
        
        if (form->numFields < MAX_FIELDS) {
            continuar = ler_confirmacao_dialogo("CONTINUAR?", "\nAdicionar outro campo? (s/n): ");
        } else {
            printf(YELLOW "\nLimite máximo de campos atingido!\n" RESET);
            break;
        }
    }
    
    // Resumo
    limpar_tela();
    desenhar_cabecalho("FORMULÁRIO CRIADO COM SUCESSO!");
    
    exibir_estrutura_formulario(form);
    
    // Salvar
    char salvar = ler_confirmacao_dialogo("FINALIZAR", "\nSalvar formulário? (s/n): ");
    
    if (salvar == 's' || salvar == 'S') {
<<<<<<< HEAD
        char filepath[300];
        snprintf(filepath, sizeof(filepath), "data/forms/%s.form", form->name);
=======
        char filepath[1024];
        snprintf(filepath, sizeof(filepath), "%s/%s.form", APP.forms, form->name);
>>>>>>> cd0de64bb0fd98426ae2df168632884f345e13cd
        
        if (salvar_formulario(form, filepath)) {
            printf(GREEN "\n✓ Formulário salvo em: %s\n" RESET, filepath);
        } else {
            printf(RED "\n✗ Erro ao salvar formulário!\n" RESET);
        }
    }
    
    return form;
}

// --- TEMPLATES ---

Form* selecionar_template() {
    struct dirent *de;
<<<<<<< HEAD
    DIR *dr = opendir("templates");
=======
    DIR *dr = opendir(APP.templates);
>>>>>>> cd0de64bb0fd98426ae2df168632884f345e13cd
    char templates[20][100];
    int count = 0;

    if (dr == NULL) {
        printf(RED "Erro ao abrir pasta de templates!\n" RESET);
        return NULL;
    }

    // Lista arquivos .json
    while ((de = readdir(dr)) != NULL && count < 20) {
<<<<<<< HEAD
        if (strstr(de->d_name, ".json")) {
=======
        if (de->d_name[0] == '.') continue; 

        char *dot = strrchr(de->d_name, '.');
        if (dot && strcmp(dot, ".json") == 0) {
>>>>>>> cd0de64bb0fd98426ae2df168632884f345e13cd
            strncpy(templates[count], de->d_name, 99);
            count++;
        }
    }
    closedir(dr);

    while(1) {
        char menu_text[4096] = "";
        
<<<<<<< HEAD
        if (count == 0) {
            strcat(menu_text, YELLOW "Nenhum template encontrado na pasta 'templates/'.\n" RESET);
=======
        // Resolve caminho absoluto para orientar o usuário
        char abs_path[1024] = {0};
        #ifdef _WIN32
            if (_fullpath(abs_path, APP.templates, sizeof(abs_path)) == NULL) strncpy(abs_path, APP.templates, sizeof(abs_path)-1);
        #else
            if (realpath(APP.templates, abs_path) == NULL) strncpy(abs_path, APP.templates, sizeof(abs_path)-1);
        #endif

        char msg[1200];
        snprintf(msg, sizeof(msg), DIM "Local: " RESET BLUE "%s" RESET "\n" DIM "Adicione arquivos .json nesta pasta para aparecerem aqui.\n\n" RESET, abs_path);
        strcat(menu_text, msg);

        if (count == 0) {
            strcat(menu_text, YELLOW "Nenhum template encontrado.\n" RESET);
>>>>>>> cd0de64bb0fd98426ae2df168632884f345e13cd
        }

        for (int i = 0; i < count; i++) {
            // Remove extensão .json para exibição
            char display[100];
            strcpy(display, templates[i]);
            char *dot = strrchr(display, '.');
            if (dot) *dot = '\0';
            
            // Capitaliza primeira letra
            display[0] = toupper(display[0]);
            
            char line[200];
            snprintf(line, sizeof(line), "   %d. " GREEN "%s\n" RESET, i + 1, display);
            strcat(menu_text, line);
        }
<<<<<<< HEAD
        strcat(menu_text, "   0. " RED "Voltar\n" RESET);
        strcat(menu_text, "\nEscolha um template para criar: ");
=======

        strcat(menu_text, "   0. " RED "Voltar\n" RESET);
        strcat(menu_text, "\nEscolha uma opção: ");
>>>>>>> cd0de64bb0fd98426ae2df168632884f345e13cd
        
        int opcao;
        if (!ler_int_dialogo("TEMPLATES DISPONÍVEIS", menu_text, &opcao)) {
            continue;
        }
        
        if (opcao == 0) return NULL;
        
        if (opcao < 1 || opcao > count) continue;

<<<<<<< HEAD
        char filepath[200];
        snprintf(filepath, sizeof(filepath), "templates/%s", templates[opcao-1]);
=======
        char filepath[1024];
        snprintf(filepath, sizeof(filepath), "%s/%s", APP.templates, templates[opcao-1]);
>>>>>>> cd0de64bb0fd98426ae2df168632884f345e13cd
        
        Form *form = importar_formulario_json(filepath);
        
        if (form) {
            char preview_text[4096];
            snprintf(preview_text, sizeof(preview_text), 
                BOLD_WHITE "Formulário: " RESET "%s\n"
                DIM "%s\n" RESET
                "\n" CYAN "Campos definidos:\n" RESET,
                form->displayName, form->description);
            
            for (int i = 0; i < form->numFields; i++) {
                Field *f = form->fields[i];
                char line[256];
                snprintf(line, sizeof(line), "  %2d. " BOLD "%s" RESET " (" DIM "%s" RESET ")\n", 
                       i + 1, f->label, tipo_campo_string(f->type));
                if (strlen(preview_text) + strlen(line) < sizeof(preview_text) - 100) {
                    strcat(preview_text, line);
                }
            }
            strcat(preview_text, "\n" YELLOW "Deseja usar este template? (s/n): " RESET);

            char confirm = ler_confirmacao_dialogo("PRÉ-VISUALIZAÇÃO", preview_text);
            
            if (confirm != 's' && confirm != 'S') {
                liberar_formulario(form);
                continue;
            }

            char novoNome[100];
            char prompt[200];
            snprintf(prompt, sizeof(prompt), "\nNome do arquivo para salvar (Enter para '%s'): ", form->name);
            
            ler_texto_dialogo("SALVAR TEMPLATE", prompt, novoNome, sizeof(novoNome));
            
            if (strlen(novoNome) > 0) {
                strncpy(form->name, novoNome, sizeof(form->name)-1);
                str_to_lower(form->name);
            }
            
<<<<<<< HEAD
            char filepath[300];
            snprintf(filepath, sizeof(filepath), "data/forms/%s.form", form->name);
=======
            char filepath[1024];
            snprintf(filepath, sizeof(filepath), "%s/%s.form", APP.forms, form->name);
>>>>>>> cd0de64bb0fd98426ae2df168632884f345e13cd
            
            if (salvar_formulario(form, filepath)) {
                printf(GREEN "\n✓ Formulário criado com sucesso!\n" RESET);
                printf("Salvo em: %s\n", filepath);
                printf("\nPressione ENTER para continuar...");
                esperar_enter_check_resize();
                return form;
            }
        }
    }
}