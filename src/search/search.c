#include "search.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../utils/colors.h"
#include "../core/form.h"
#include "../core/field.h"
#include "../core/record.h"

static void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

static void limpar_tela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Busca case-insensitive (similar a strcasestr)
static char* stristr(const char* haystack, const char* needle) {
    if (!haystack || !needle) return NULL;
    
    int len_h = strlen(haystack);
    int len_n = strlen(needle);
    
    if (len_n == 0) return (char*)haystack;
    
    for (int i = 0; i <= len_h - len_n; i++) {
        int j;
        for (j = 0; j < len_n; j++) {
            if (tolower((unsigned char)haystack[i+j]) != tolower((unsigned char)needle[j])) {
                break;
            }
        }
        if (j == len_n) return (char*)(haystack + i);
    }
    return NULL;
}

static void exibir_resultado(Record *record, Form *form) {
    printf(CYAN "────────────────────────────────────────────────────────\n" RESET);
    printf(BOLD_WHITE "Registro #%d\n" RESET, record->id);
    
    for (int j = 0; j < form->numFields; j++) {
        Field *field = form->fields[j];
        const char *value = obter_valor_campo(record, field->id);
        
        if (value && value[0]) {
            printf(GREEN "%-25s: " RESET "%s\n", field->label, value);
        }
    }
}

void buscar_texto_geral(Form *form, RecordSet *recordset) {
    char termo[100];
    
    printf(BOLD_CYAN "\n--- Busca Geral (Todos os Campos) ---\n" RESET);
    printf("Digite o termo para buscar: ");
    
    if (!fgets(termo, sizeof(termo), stdin)) return;
    termo[strcspn(termo, "\n")] = 0;
    
    if (strlen(termo) == 0) return;
    
    printf("\n" BOLD_WHITE "Resultados para '%s':\n" RESET, termo);
    
    int encontrados = 0;
    
    for (int i = 0; i < recordset->numRecords; i++) {
        Record *rec = recordset->records[i];
        int match = 0;
        
        for (int j = 0; j < form->numFields; j++) {
            const char *val = obter_valor_campo(rec, form->fields[j]->id);
            if (val && stristr(val, termo)) {
                match = 1;
                break;
            }
        }
        
        if (match) {
            exibir_resultado(rec, form);
            encontrados++;
        }
    }
    
    if (encontrados == 0) {
        printf(YELLOW "Nenhum registro encontrado.\n" RESET);
    } else {
        printf(GREEN "\nTotal encontrados: %d\n" RESET, encontrados);
    }
    
    printf("\nPressione ENTER para continuar...");
    getchar();
}

void buscar_por_campo(Form *form, RecordSet *recordset) {
    printf(BOLD_CYAN "\n--- Filtrar por Campo ---\n" RESET);
    
    for (int i = 0; i < form->numFields; i++) {
        printf("%d. %s\n", i + 1, form->fields[i]->label);
    }
    
    printf("\nEscolha o campo: ");
    int campoIdx;
    if (scanf("%d", &campoIdx) != 1 || campoIdx < 1 || campoIdx > form->numFields) {
        limpar_buffer();
        printf(RED "Campo inválido!\n" RESET);
        printf("Pressione ENTER...");
        getchar();
        return;
    }
    limpar_buffer();
    
    Field *field = form->fields[campoIdx - 1];
    
    char termo[100];
    printf("Buscar em '%s': ", field->label);
    if (!fgets(termo, sizeof(termo), stdin)) return;
    termo[strcspn(termo, "\n")] = 0;
    
    printf("\n" BOLD_WHITE "Resultados em '%s' contendo '%s':\n" RESET, field->label, termo);
    
    int encontrados = 0;
    
    for (int i = 0; i < recordset->numRecords; i++) {
        Record *rec = recordset->records[i];
        const char *val = obter_valor_campo(rec, field->id);
        
        if (val && stristr(val, termo)) {
            exibir_resultado(rec, form);
            encontrados++;
        }
    }
    
    if (encontrados == 0) {
        printf(YELLOW "Nenhum registro encontrado.\n" RESET);
    } else {
        printf(GREEN "\nTotal encontrados: %d\n" RESET, encontrados);
    }
    
    printf("\nPressione ENTER para continuar...");
    getchar();
}

void menu_busca(Form *form, RecordSet *recordset) {
    int opcao;
    
    do {
        limpar_tela();
        printf(BOLD_CYAN "╔════════════════════════════════════════════════════╗\n" RESET);
        printf(BOLD_CYAN "║              BUSCA DE REGISTROS                    ║\n" RESET);
        printf(BOLD_CYAN "╚════════════════════════════════════════════════════╝\n" RESET);
        
        printf(GREEN "\nFormulário: %s\n" RESET, form->displayName);
        printf(GREEN "Total de registros: %d\n" RESET, recordset->numRecords);
        
        printf(CYAN "\n────────────────────────────────────────────────────────\n" RESET);
        printf("1. 🔍 Busca Geral (Todos os campos)\n");
        printf("2. 🎯 Filtrar por Campo Específico\n");
        printf("0. 🔙 Voltar\n");
        printf(CYAN "────────────────────────────────────────────────────────\n" RESET);
        
        printf("\nEscolha: ");
        if (scanf("%d", &opcao) != 1) {
            limpar_buffer();
            continue;
        }
        limpar_buffer();
        
        switch (opcao) {
            case 1:
                buscar_texto_geral(form, recordset);
                break;
            case 2:
                buscar_por_campo(form, recordset);
                break;
            case 0:
                break;
            default:
                printf(RED "Opção inválida!\n" RESET);
                getchar();
        }
    } while (opcao != 0);
}