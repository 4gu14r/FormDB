#include "record_utils.h"
#include "ui_utils.h"
#include "colors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void visualizar_lista_registros(RecordSet *recordset, const char *titulo, const char *msg_vazio) {
    if (!recordset || !recordset->form) return;
    
    Form *form = recordset->form;
    
    int pagina_atual = 0;
    const int REGISTROS_POR_PAGINA = 3; // Define quantos registros aparecem por tela
    
    while (1) {
        limpar_tela();
        desenhar_cabecalho(titulo);
        
        printf(GREEN "\nFormulário: %s\n" RESET, form->displayName);
        printf(GREEN "Total de registros: %d\n" RESET, recordset->numRecords);
        
        if (recordset->numRecords == 0) {
            printf(YELLOW "\n%s\n" RESET, msg_vazio);
            printf("\nPressione ENTER para continuar...");
            if (esperar_enter_check_resize()) continue;
            return;
        }
        
        printf("\n");
        
        // Cálculos da paginação
        int total_paginas = (recordset->numRecords + REGISTROS_POR_PAGINA - 1) / REGISTROS_POR_PAGINA;
        if (total_paginas == 0) total_paginas = 1;
        
        // Garante que a página atual é válida
        if (pagina_atual >= total_paginas) pagina_atual = total_paginas - 1;
        if (pagina_atual < 0) pagina_atual = 0;
        
        int inicio = pagina_atual * REGISTROS_POR_PAGINA;
        int fim = inicio + REGISTROS_POR_PAGINA;
        if (fim > recordset->numRecords) fim = recordset->numRecords;
        
        // Exibe apenas os registros da página atual
        for (int i = inicio; i < fim; i++) {
            Record *record = recordset->records[i];
            
            desenhar_separador();
            printf(BOLD_WHITE "Registro #%d\n" RESET, record->id);
            
            for (int j = 0; j < form->numFields; j++) {
                Field *field = form->fields[j];
                const char *value = obter_valor_campo(record, field->id);
                
                printf(GREEN "%-25s: " RESET, field->label);
                
                if (value && value[0]) {
                    // Formatação especial para alguns tipos
                    if (field->type == FIELD_RATING) {
                        int rating = atoi(value);
                        for (int k = 0; k < 5; k++) {
                            printf(k < rating ? "★" : "☆");
                        }
                        printf(" (%s/5)", value);
                    } else if (field->type == FIELD_BOOLEAN) {
                        if (strcmp(value, "Sim") == 0 || strcmp(value, "1") == 0) {
                            printf(GREEN "✓ Sim" RESET);
                        } else {
                            printf(RED "✗ Não" RESET);
                        }
                    } else if (field->type == FIELD_MONEY) {
                        printf("R$ %s", value);
                    } else {
                        printf("%s", value);
                    }
                } else {
                    printf(DIM "(vazio)" RESET);
                }
                
                printf("\n");
            }
        }
        
        desenhar_separador();
        
        // Rodapé com navegação
        printf("Página %d de %d | Total: %d registros\n", pagina_atual + 1, total_paginas, recordset->numRecords);
        printf(CYAN "[N] Próxima  [P] Anterior  [0] Voltar\n" RESET);
        printf("\nEscolha: ");
        
        // Leitura do comando (manual para tratar resize corretamente aqui)
        char buffer[100];
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            if (errno == EINTR) continue; // Resize detectado, redesenha
            break;
        }
        
        // Processa a opção
        char opcao = buffer[0];
        if (opcao == 'n' || opcao == 'N') {
            if (pagina_atual < total_paginas - 1) pagina_atual++;
        }
        else if (opcao == 'p' || opcao == 'P') {
            if (pagina_atual > 0) pagina_atual--;
        }
        else if (opcao == '0') {
            break;
        }
    }
}

void visualizar_registros(RecordSet *recordset) {
    visualizar_lista_registros(recordset, "REGISTROS CADASTRADOS", "Nenhum registro cadastrado ainda.");
}