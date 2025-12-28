#include "search.h"
#include "../utils/colors.h"
#include "../utils/ui_utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

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

void menu_busca(Form *form, RecordSet *recordset) {
    if (!form || !recordset) return;
    
    char termo[100];
    
    // Loop de entrada do termo (responsivo)
    ler_texto_dialogo("BUSCAR REGISTROS", "\nDigite o termo para buscar (em qualquer campo) ou ENTER para voltar: ", termo, sizeof(termo));
    
    if (strlen(termo) == 0) return;
    
    // Loop de exibição dos resultados (responsivo)
    while (1) {
        limpar_tela();
        desenhar_cabecalho("RESULTADOS DA BUSCA");
        
        printf("Buscando por: " BOLD_WHITE "%s\n" RESET, termo);
        desenhar_separador();
        
        int encontrados = 0;
        
        for (int i = 0; i < recordset->numRecords; i++) {
            Record *rec = recordset->records[i];
            bool match = false;
            
            // Verifica ID
            char idStr[20];
            sprintf(idStr, "%d", rec->id);
            if (strcmp(idStr, termo) == 0) match = true;
            
            // Verifica campos
            if (!match) {
                for (int j = 0; j < form->numFields; j++) {
                    const char *val = obter_valor_campo(rec, form->fields[j]->id);
                    if (strcasestr_custom(val, termo)) {
                        match = true;
                        break;
                    }
                }
            }
            
            if (match) {
                encontrados++;
                printf(BOLD_WHITE "Registro #%d\n" RESET, rec->id);
                
                // Mostra os primeiros 3 campos para resumo
                int camposMostrados = 0;
                for (int j = 0; j < form->numFields && camposMostrados < 3; j++) {
                    const char *val = obter_valor_campo(rec, form->fields[j]->id);
                    if (strlen(val) > 0) {
                        printf("  %s: %s\n", form->fields[j]->label, val);
                        camposMostrados++;
                    }
                }
                desenhar_separador();
            }
        }
        
        if (encontrados == 0) {
            printf(YELLOW "Nenhum registro encontrado.\n" RESET);
        }
        
        printf("\nPressione ENTER para voltar...");
        if (esperar_enter_check_resize()) continue;
        break;
    }
}