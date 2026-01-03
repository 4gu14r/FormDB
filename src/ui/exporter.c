// Habilita extensões POSIX (necessário para realpath)
#define _XOPEN_SOURCE 600
#define _DEFAULT_SOURCE

#include "exporter.h"
#include "../storage/csv_handler.h"
#include "../storage/json_handler.h"
#include "../utils/ui_utils.h"
#include "../utils/file_utils.h"
#include "../utils/colors.h"
#include "../utils/app_context.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void menu_exportar(Form *form, RecordSet *recordset) {
    char opcao;
    
    while(1) {
        limpar_tela();
        desenhar_cabecalho("EXPORTAR DADOS");
        
        printf(CYAN "Formulário: " RESET "%s\n", form->displayName);
        printf(CYAN "Registros:  " RESET "%d\n\n", recordset->numRecords);
        
        printf("Escolha o formato:\n\n");
        printf(BOLD_WHITE "1. " RESET "CSV\n");
        printf(BOLD_WHITE "2. " RESET "JSON\n");
        printf(BOLD_WHITE "0. " RESET "Voltar\n");
        
        printf("\nOpção: ");
        char buffer[10];
        if (!ler_string_segura(buffer, sizeof(buffer))) return;
        opcao = buffer[0];
        
        if (opcao == '0') return;
        
        if (opcao == '1' || opcao == '2') {
            // CSV ou JSON
            char caminho[1024];
            
            // Resolve caminho absoluto do DIRETÓRIO para exibição
            char abs_export_dir[2048] = {0};
            #ifdef _WIN32
                if (_fullpath(abs_export_dir, APP.exports, sizeof(abs_export_dir)) == NULL) 
                    strncpy(abs_export_dir, APP.exports, sizeof(abs_export_dir)-1);
            #else
                if (realpath(APP.exports, abs_export_dir) == NULL) {
                    strncpy(abs_export_dir, APP.exports, sizeof(abs_export_dir)-1);
                }
            #endif
            
            // Pergunta o nome do arquivo
            char msg[4096];
            char nome_arquivo[256];
            
            snprintf(msg, sizeof(msg),
                DIM "Diretório de destino: " RESET CYAN "%s" RESET "\n\n"
                BOLD_WHITE "Digite o nome do arquivo para salvar (ou ENTER para voltar):" RESET, 
                abs_export_dir);
            
            if (!ler_texto_dialogo("SALVAR ARQUIVO", msg, nome_arquivo, sizeof(nome_arquivo))) {
                continue;
            }
            
            // Se vazio, volta
            if (strlen(nome_arquivo) == 0) {
                continue;
            }
            
            // Monta o caminho completo
            const char *ext = (opcao == '1') ? ".csv" : ".json";
            snprintf(caminho, sizeof(caminho), "%s/%s%s", APP.exports, nome_arquivo, ext);
            
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
            else sucesso = exportar_json(recordset, caminho);
            
            limpar_tela();
            desenhar_cabecalho("EXPORTAÇÃO");
            if (sucesso) {
                char abs_final_path[2048] = {0};
                #ifdef _WIN32
                    if (_fullpath(abs_final_path, caminho, sizeof(abs_final_path)) == NULL) 
                        strncpy(abs_final_path, caminho, sizeof(abs_final_path)-1);
                #else
                    if (realpath(caminho, abs_final_path) == NULL) {
                        strncpy(abs_final_path, caminho, sizeof(abs_final_path)-1);
                    }
                #endif
                
                printf(GREEN "\n✓ Dados exportados com sucesso!\n\n" RESET);
                printf("Arquivo salvo em:"CYAN" %s\n" RESET, abs_final_path);
            } else {
                printf(RED "\n✗ Erro ao exportar dados.\n\n" RESET);
                printf("Verifique permissões ou espaço em disco.\n");
            }
            printf("\nPressione ENTER para continuar...");
            esperar_enter_check_resize();
            return; // Volta pro menu anterior após sucesso/erro
        }
    }
}