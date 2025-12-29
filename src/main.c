// Habilita definições POSIX (necessário para sigaction com -std=c11)
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include "core/form.h"
#include "core/field.h"
#include "core/record.h"
#include "ui/form_builder.h"
#include "ui/data_entry.h"
#include "ui/form_browser.h"
#include "ui/form_manager.h"
#include "ui/importer.h"
#include "ui/exporter.h"
#include "search/search.h"
#include "utils/colors.h"
#include "utils/ui_utils.h"
#include "utils/record_utils.h"

void criar_diretorios() {
    struct stat st = {0};
    
    if (stat("data", &st) == -1) {
        mkdir("data", 0700);
    }
    if (stat("data/forms", &st) == -1) {
        mkdir("data/forms", 0700);
    }
    if (stat("data/records", &st) == -1) {
        mkdir("data/records", 0700);
    }
    if (stat("templates", &st) == -1) {
        mkdir("templates", 0700);
    }
    if (stat("exports", &st) == -1) {
        mkdir("exports", 0700);
    }
    if (stat("backups", &st) == -1) {
        mkdir("backups", 0700);
    }
}

void exibir_banner() {
    printf(BOLD_CYAN);
    printf("\n");
    printf("  ███████╗ ██████╗ ██████╗ ███╗   ███╗██████╗ ██████╗ \n");
    printf("  ██╔════╝██╔═══██╗██╔══██╗████╗ ████║██╔══██╗██╔══██╗\n");
    printf("  █████╗  ██║   ██║██████╔╝██╔████╔██║██║  ██║██████╔╝\n");
    printf("  ██╔══╝  ██║   ██║██╔══██╗██║╚██╔╝██║██║  ██║██╔══██╗\n");
    printf("  ██║     ╚██████╔╝██║  ██║██║ ╚═╝ ██║██████╔╝██████╔╝\n");
    printf("  ╚═╝      ╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚═════╝ ╚═════╝ \n");
    printf(RESET);
    printf(CYAN "  Sistema Universal de Formulários e Banco de Dados\n" RESET);
    printf(DIM   "  v1.0 - Crie formulários customizados para qualquer coisa!\n" RESET);
    printf("\n");
}

RecordSet* preparar_recordset(Form *form, int exibir_mensagem) {
    RecordSet *recordset = criar_recordset(form);
    
    char filepath[300];
    snprintf(filepath, sizeof(filepath), "data/records/%s.csv", form->name);
    
    RecordSet *loaded = carregar_registros_csv(form, filepath);
    if (loaded) {
        liberar_recordset(recordset);
        if (exibir_mensagem) {
            printf(GREEN "\n✓ Dados carregados de %s\n" RESET, filepath);
        }
        return loaded;
    }
    return recordset;
}

// Handler vazio apenas para interromper o scanf quando redimensionar
void handle_resize(int sig) {
    (void)sig;
}

void menu_principal() {
    int opcao;
    Form *formAtual = NULL;
    char msgErro[512] = "";
    
    // Configurar captura de sinal de redimensionamento (Linux/Mac)
    #ifndef _WIN32
    struct sigaction sa;
    sa.sa_handler = handle_resize;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0; // Importante: NÃO usar SA_RESTART para que o scanf seja interrompido
    sigaction(SIGWINCH, &sa, NULL);
    #endif
    
    do {
        limpar_tela(); // Limpa a tela a cada iteração para redesenhar corretamente
        desenhar_cabecalho("MENU PRINCIPAL");
        printf("   1. " GREEN "📝 Criar Novo Formulário\n" RESET);
        printf("   2. " GREEN "📋 Abrir Formulário Existente\n" RESET);
        printf("   3. " GREEN "✏️  Cadastrar Dados\n" RESET);
        printf("   4. " GREEN "📊 Ver Registros\n" RESET);
        printf("   5. " GREEN "🔍 Buscar/Filtrar\n" RESET);
        printf("   6. " GREEN "📤 Exportar Dados\n" RESET);
        printf("   7. " GREEN "📥 Importar Dados\n" RESET);
        printf("   8. " GREEN "📈 Relatórios e Estatísticas\n" RESET);
        printf("   9. " GREEN "🗂️  Gerenciar Formulários\n" RESET);
        printf("   10." GREEN "💾 Templates Prontos\n" RESET);
        printf("   0. " RED "🚪 Sair\n" RESET);
        
        if (formAtual) {
            printf(GREEN "\n✓ Formulário ativo: %s (%d campos, %d registros)\n" RESET,
                   formAtual->displayName, formAtual->numFields, formAtual->totalRecords);
        } else {
            printf(YELLOW "\n⚠ Nenhum formulário aberto\n" RESET);
        }
        
        // Exibe mensagem de erro se houver (persiste após limpar tela)
        if (msgErro[0] != '\0') {
            printf("%s", msgErro);
            msgErro[0] = '\0';
        }
        
        printf("\n" BOLD_WHITE "Escolha uma opção: " RESET);
        
        int result = scanf("%d", &opcao);
        
        // Se foi interrompido pelo resize (EINTR), apenas redesenha
        if (result == EOF && errno == EINTR) {
            continue;
        }
        
        if (result != 1) {
            limpar_buffer();
            printf(RED "\n✗ Opção inválida!\n" RESET);
            continue;
        }
        limpar_buffer();
        
        switch (opcao) {
            case 1: {
                // Criar novo formulário
                if (formAtual) {
                    liberar_formulario(formAtual);
                }
                Form *novoForm = construir_formulario_interativo();
                if (novoForm) {
                    formAtual = novoForm;
                    printf(GREEN "\n✓ Formulário criado e ativado!\n" RESET);
                }
                break;
            }
            
            case 2: {
                // Abrir formulário existente
                Form *novoForm = selecionar_formulario_interativo();
                if (novoForm) {
                    if (formAtual) {
                        liberar_formulario(formAtual);
                    }
                    formAtual = novoForm;
                    
                    // Atualiza a contagem de registros lendo do disco
                    RecordSet *rs = preparar_recordset(formAtual, 0);
                    if (rs) {
                        formAtual->totalRecords = rs->numRecords;
                        liberar_recordset(rs);
                    }
                }
                break;
            }
            
            case 3: {
                // Cadastrar dados
                if (!formAtual) {
                    snprintf(msgErro, sizeof(msgErro), RED "\n✗ Nenhum formulário aberto." RESET DIM " Crie ou abra um formulário primeiro!\n" RESET);
                } else {
                    RecordSet *recordset = preparar_recordset(formAtual, 1);
                    
                    menu_cadastro(formAtual, recordset);
                    
                    // Atualiza a contagem de registros no formulário principal
                    if (recordset) {
                        formAtual->totalRecords = recordset->numRecords;
                    }
                    liberar_recordset(recordset);
                }
                break;
            }
            
            case 4: {
                // Ver registros
                if (!formAtual) {
                    snprintf(msgErro, sizeof(msgErro), RED "\n✗ Nenhum formulário aberto." RESET DIM " Crie ou abra um formulário primeiro!\n" RESET);
                } else {
                    RecordSet *recordset = preparar_recordset(formAtual, 0);
                    
                    visualizar_registros(recordset);
                    
                    liberar_recordset(recordset);
                }
                break;
            }
            
            case 5: {
                // Buscar
                if (!formAtual) {
                    snprintf(msgErro, sizeof(msgErro), RED "\n✗ Nenhum formulário aberto." RESET DIM " Crie ou abra um formulário primeiro!\n" RESET);
                } else {
                    RecordSet *recordset = preparar_recordset(formAtual, 0);
                    
                    menu_busca(formAtual, recordset);
                    
                    liberar_recordset(recordset);
                }
                break;
            }
            
            case 6: {
                // Exportar
                if (!formAtual) {
                    snprintf(msgErro, sizeof(msgErro), RED "\n✗ Nenhum formulário aberto." RESET DIM " Crie ou abra um formulário primeiro!\n" RESET);
                } else {
                    RecordSet *recordset = preparar_recordset(formAtual, 0);
                    menu_exportar(formAtual, recordset);
                    liberar_recordset(recordset);
                }
                break;
            }
            
            case 7: {
                // Importar
                menu_importar();
                break;
            }
            
            case 8: {
                // Relatórios
                printf(YELLOW "\n[Relatórios - Em desenvolvimento]\n" RESET);
                printf("Estatísticas, gráficos e dashboards\n");
                printf("Pressione ENTER para continuar...");
                getchar();
                break;
            }
            
            case 9: {
                // Gerenciar formulários
                menu_gerenciar_formularios();
                break;
            }
            
            case 10: {
                // Templates
                Form *novoForm = selecionar_template();
                if (novoForm) {
                    if (formAtual) {
                        liberar_formulario(formAtual);
                    }
                    formAtual = novoForm;
                }
                break;
            }
            
            case 0:
                printf(BOLD_GREEN "\n👋 Até logo! Obrigado por usar o FormDB!\n" RESET);
                if (formAtual) {
                    liberar_formulario(formAtual);
                }
                break;
                
            default:
                printf(RED "\n✗ Opção inválida! Tente novamente.\n" RESET);
        }
        
    } while (opcao != 0);
}

int main() {
    // Criar estrutura de diretórios
    criar_diretorios();
    
    // Exibir banner
    exibir_banner();
    
    printf(BOLD_WHITE "\n🚀 Bem-vindo ao FormDB!\n" RESET);
    printf("Sistema que permite criar formulários customizados\n");
    printf("para cadastrar QUALQUER tipo de dado!\n");
    
    printf("\nPressione ENTER para começar...");
    getchar();
    
    // Menu principal
    menu_principal();
    
    return 0;
}