#include "exporter.h"
#include "../storage/csv_handler.h"
#include "../utils/ui_utils.h"
#include "../utils/file_utils.h"
#include "../utils/colors.h"
#include "../utils/app_context.h"
#include <stdio.h>
#include <string.h>

void menu_exportar(Form *form, RecordSet *recordset) {
    char opcao;
    
    while(1) {
        limpar_tela();
        desenhar_cabecalho("EXPORTAR DADOS");
        
        printf(CYAN "Formulário: " RESET "%s\n", form->displayName);
        printf(CYAN "Registros:  " RESET "%d\n\n", recordset->numRecords);
        
        printf("Escolha o formato:\n\n");
        printf(BOLD_WHITE "1. " RESET "CSV (Planilha Padrão)\n");
        printf(BOLD_WHITE "2. " RESET "Excel (Compatível)\n");
        printf(BOLD_WHITE "3. " RESET "JSON (Em breve)\n");
        printf(BOLD_WHITE "0. " RESET "Voltar\n");
        
        printf("\nOpção: ");
        char buffer[10];
        if (!ler_string_segura(buffer, sizeof(buffer))) return;
        opcao = buffer[0];
        
        if (opcao == '0') return;
        
        if (opcao == '1' || opcao == '2') {
            // CSV ou Excel
            char caminho[1024];
            char default_path[1024];
            
            snprintf(default_path, sizeof(default_path), "%s/%s.csv", APP.exports, form->name);
            
            // Pergunta o caminho
            char msg[2048];
            snprintf(msg, sizeof(msg), "Caminho do arquivo (Enter para: %s):", default_path);
            
            if (!ler_texto_dialogo("SALVAR ARQUIVO", msg, caminho, sizeof(caminho))) {
                continue;
            }
            
            // Se vazio, usa default
            if (strlen(caminho) == 0) {
                strcpy(caminho, default_path);
            }
            
            // Verifica/Cria diretório pai
            char dir_pai[1024];
            extrair_diretorio_pai(caminho, dir_pai, sizeof(dir_pai));
            
            if (!criar_diretorio(dir_pai)) {
                printf(RED "\nErro: Não foi possível criar o diretório '%s'.\n" RESET, dir_pai);
                esperar_enter_check_resize();
                continue;
            }
            
            // Verifica se arquivo existe
            if (arquivo_existe(caminho)) {
                char confirm = ler_confirmacao_dialogo("ARQUIVO EXISTE", 
                    "O arquivo já existe. Deseja sobrescrever? (S/N)");
                if (confirm != 'S' && confirm != 's') {
                    continue;
                }
            }
            
            // Salva
            bool sucesso = false;
            if (opcao == '1') sucesso = salvar_registros_csv(recordset, caminho);
            else sucesso = exportar_excel(recordset, caminho);
            
            limpar_tela();
            desenhar_cabecalho("EXPORTAÇÃO");
            if (sucesso) {
                printf(GREEN "\n✓ Dados exportados com sucesso!\n" RESET);
                printf("Arquivo salvo em: %s\n", caminho);
            } else {
                printf(RED "\n✗ Erro ao exportar dados.\n" RESET);
                printf("Verifique permissões ou espaço em disco.\n");
            }
            printf("\nPressione ENTER para continuar...");
            esperar_enter_check_resize();
            return; // Volta pro menu anterior após sucesso/erro
            
        } else if (opcao == '3') {
            printf(YELLOW "\nExportação JSON em desenvolvimento.\n" RESET);
            esperar_enter_check_resize();
        }
    }
}