#include "search.h"
#include "../utils/colors.h"
#include "../utils/ui_utils.h"
#include "../utils/record_utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>

// Função auxiliar para busca case-insensitive
static char *strcasestr_custom(const char *haystack, const char *needle) {
    if (!haystack || !needle) return NULL;
    
    // Implementação simples de busca
    // Nota: strcasestr é uma extensão GNU, então fazemos uma versão manual simples
    // para garantir compatibilidade
    
    int len_h = strlen(haystack);
    int len_n = strlen(needle);
    
    if (len_n > len_h) return NULL;
    
    for (int i = 0; i <= len_h - len_n; i++) {
        int j;
        for (j = 0; j < len_n; j++) {
            if (tolower(haystack[i+j]) != tolower(needle[j])) {
                break;
            }
        }
        if (j == len_n) return (char*)(haystack + i);
    }
    return NULL;
}

static void exibir_resultados(Form *form, RecordSet *recordset, int fieldId, const char *termo) {
    // Cria um RecordSet temporário para armazenar os resultados
    RecordSet results;
    results.form = form;
    results.numRecords = 0;
    results.capacity = recordset->numRecords;
    results.records = malloc(sizeof(Record*) * results.capacity);
    
    if (!results.records) {
        printf(RED "Erro de memória ao filtrar resultados.\n" RESET);
        return;
    }

    // Filtra os registros
    for (int i = 0; i < recordset->numRecords; i++) {
        Record *rec = recordset->records[i];
        bool match = false;
        
        if (fieldId != -1) {
            // Busca em campo específico
            const char *val = obter_valor_campo(rec, fieldId);
            if (strcasestr_custom(val, termo)) {
                match = true;
            }
        } else {
            // Busca Geral
            char idStr[20];
            sprintf(idStr, "%d", rec->id);
            if (strcmp(idStr, termo) == 0) match = true;
            
            if (!match) {
                for (int j = 0; j < form->numFields; j++) {
                    const char *val = obter_valor_campo(rec, form->fields[j]->id);
                    if (strcasestr_custom(val, termo)) {
                        match = true;
                        break;
                    }
                }
            }
        }
        
        if (match) {
            results.records[results.numRecords++] = rec;
        }
    }

    // Exibe usando a função padrão de visualização
    char titulo[200];
    snprintf(titulo, sizeof(titulo), "RESULTADOS: %s", termo);
    
    visualizar_lista_registros(&results, titulo, "Nenhum registro encontrado.");

    // Limpa apenas o array de ponteiros (os registros pertencem ao recordset original)
    free(results.records);
}

static void buscar_geral(Form *form, RecordSet *recordset) {
    char termo[100];
    if (ler_texto_dialogo("BUSCA GERAL", "\nDigite o termo para buscar (em qualquer campo): ", termo, sizeof(termo))) {
        if (strlen(termo) > 0) {
            exibir_resultados(form, recordset, -1, termo);
        }
    }
}

static void buscar_por_campo(Form *form, RecordSet *recordset) {
    while(1) {
        limpar_tela();
        desenhar_cabecalho("BUSCAR POR CAMPO");
        
        printf("\nEscolha o campo para pesquisar:\n\n");
        for(int i=0; i<form->numFields; i++) {
            printf(CYAN "  %d. " RESET "%s\n", i+1, form->fields[i]->label);
        }
        printf(CYAN "  0. " RESET "Voltar\n");
        desenhar_separador();
        
        printf("\nOpção: ");
        int opcao;
        int result = scanf("%d", &opcao);
        
        if (result == EOF && errno == EINTR) continue;
        if (result != 1) { limpar_buffer(); continue; }
        limpar_buffer();
        
        if (opcao == 0) return;
        if (opcao < 1 || opcao > form->numFields) continue;
        
        Field *field = form->fields[opcao-1];
        char termo[100];
        char prompt[200];
        snprintf(prompt, sizeof(prompt), "\nDigite o valor para buscar em '%s': ", field->label);
        
        if (ler_texto_dialogo("BUSCAR POR CAMPO", prompt, termo, sizeof(termo))) {
            if (strlen(termo) > 0) {
                exibir_resultados(form, recordset, field->id, termo);
            }
        }
    }
}

void menu_busca(Form *form, RecordSet *recordset) {
    if (!form || !recordset) return;
    
    int opcao;
    do {
        limpar_tela();
        desenhar_cabecalho("MENU DE BUSCA");
        printf(GREEN "\nFormulário: %s\n" RESET, form->displayName);
        printf(GREEN "Total de registros: %d\n\n" RESET, recordset->numRecords);
        desenhar_separador();
        printf("1. 🔍 Busca Geral (Todos os campos)\n");
        printf("2. 🎯 Busca por Campo Específico\n");
        printf("0. 🔙 Voltar\n");
        desenhar_separador();
        printf("\nEscolha: ");
        
        int result = scanf("%d", &opcao);
        if (result == EOF && errno == EINTR) continue;
        if (result != 1) { limpar_buffer(); continue; }
        limpar_buffer();
        
        switch(opcao) {
            case 1: buscar_geral(form, recordset); break;
            case 2: buscar_por_campo(form, recordset); break;
            case 0: break;
            default: break;
        }
    } while(opcao != 0);
}