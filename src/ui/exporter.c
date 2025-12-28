#include "exporter.h"
#include "../utils/colors.h"
#include "../utils/ui_utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

void menu_exportar(Form *form, RecordSet *recordset) {
    if (!form || !recordset) {
        printf(RED "Erro: Formulário ou registros inválidos.\n" RESET);
        return;
    }
    
    int opcao;
    
    // Loop para responsividade
    while (1) {
        limpar_tela();
        desenhar_cabecalho("EXPORTAR DADOS");
        printf(GREEN"\nFormulário: %s\n"RESET, form->displayName);
        printf(GREEN"Total de registros: %d\n\n"RESET, recordset->numRecords);
        
        if (recordset->numRecords == 0) {
            printf(YELLOW "Sem dados para exportar.\n" RESET);
            printf("\nPressione ENTER para voltar...");
            limpar_buffer();
            return;
        }

        desenhar_separador();

        printf("Formatos disponíveis:\n");
        printf("1. CSV (Excel, LibreOffice)\n");
        printf("2. JSON (Sistemas, APIs)\n");
        printf("0. Cancelar\n");

        desenhar_separador();
        
        printf("\nEscolha o formato: ");
        int result = scanf("%d", &opcao);
        
        if (result == EOF && errno == EINTR) continue; // Redesenha
        
        limpar_buffer();
        break; // Sai do loop se leu corretamente
    }
    
    if (opcao == 0) return;
    
    char filename[200];
    char filepath[300];
    
    printf("Nome do arquivo (sem extensão): ");
    ler_string_segura(filename, sizeof(filename));
    
    bool sucesso = false;
    
    if (opcao == 1) {
        snprintf(filepath, sizeof(filepath), "exports/%s.csv", filename);
        printf("Exportando para %s...\n", filepath);
        sucesso = salvar_registros_csv(recordset, filepath);
    } else if (opcao == 2) {
        snprintf(filepath, sizeof(filepath), "exports/%s.json", filename);
        printf("Exportando para %s...\n", filepath);
        sucesso = exportar_json(recordset, filepath);
    } else {
        printf(RED "Opção inválida!\n" RESET);
        return;
    }
    
    if (sucesso) {
        printf(GREEN "\n✓ Exportação concluída com sucesso!\n" RESET);
        printf("Arquivo salvo em: %s\n", filepath);
    } else {
        printf(RED "\n✗ Erro ao exportar arquivo.\n" RESET);
        printf("Verifique se a pasta 'exports/' existe.\n");
    }
    
    printf("\nPressione ENTER para voltar...");
    limpar_buffer();
}