#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "core/form.h"
#include "core/field.h"
#include "core/record.h"
#include "ui/form_builder.h"
#include "ui/data_entry.h"
#include "utils/colors.h"

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

void limpar_buffer_input() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void listar_formularios() {
    printf(BOLD_CYAN "\n╔════════════════════════════════════════════════╗\n" RESET);
    printf(BOLD_CYAN "║          FORMULÁRIOS CADASTRADOS               ║\n" RESET);
    printf(BOLD_CYAN "╚════════════════════════════════════════════════╝\n" RESET);
    
    // TODO: Implementar leitura de diretório data/forms/
    printf("\n" YELLOW "Nenhum formulário cadastrado ainda.\n" RESET);
    printf(GREEN "Crie seu primeiro formulário na opção 1!\n" RESET);
}

void menu_principal() {
    int opcao;
    Form *formAtual = NULL;
    
    do {
        printf(BOLD_CYAN "\n╔════════════════════════════════════════════════╗\n" RESET);
        printf(BOLD_CYAN "║              MENU PRINCIPAL                    ║\n" RESET);
        printf(BOLD_CYAN "╠════════════════════════════════════════════════╣\n" RESET);
        printf(CYAN "║                                                ║\n" RESET);
        printf(CYAN "║  " RESET "1. " GREEN "📝 Criar Novo Formulário" CYAN "                   ║\n" RESET);
        printf(CYAN "║  " RESET "2. " GREEN "📋 Abrir Formulário Existente" CYAN "              ║\n" RESET);
        printf(CYAN "║  " RESET "3. " GREEN "✏️  Cadastrar Dados" CYAN "                        ║\n" RESET);
        printf(CYAN "║  " RESET "4. " GREEN "📊 Ver Registros" CYAN "                          ║\n" RESET);
        printf(CYAN "║  " RESET "5. " GREEN "🔍 Buscar/Filtrar" CYAN "                         ║\n" RESET);
        printf(CYAN "║  " RESET "6. " GREEN "📤 Exportar Dados" CYAN "                         ║\n" RESET);
        printf(CYAN "║  " RESET "7. " GREEN "📥 Importar Dados" CYAN "                         ║\n" RESET);
        printf(CYAN "║  " RESET "8. " GREEN "📈 Relatórios e Estatísticas" CYAN "              ║\n" RESET);
        printf(CYAN "║  " RESET "9. " GREEN "🗂️  Gerenciar Formulários" CYAN "                 ║\n" RESET);
        printf(CYAN "║  " RESET "10." GREEN " 💾 Templates Prontos" CYAN "                     ║\n" RESET);
        printf(CYAN "║  " RESET "0. " RED "🚪 Sair" CYAN "                                   ║\n" RESET);
        printf(CYAN "║                                                ║\n" RESET);
        printf(BOLD_CYAN "╚════════════════════════════════════════════════╝\n" RESET);
        
        if (formAtual) {
            printf(GREEN "\n✓ Formulário ativo: %s (%d campos, %d registros)\n" RESET,
                   formAtual->displayName, formAtual->numFields, formAtual->totalRecords);
        } else {
            printf(YELLOW "\n⚠ Nenhum formulário aberto\n" RESET);
        }
        
        printf("\n" BOLD_WHITE "Escolha uma opção: " RESET);
        
        if (scanf("%d", &opcao) != 1) {
            limpar_buffer_input();
            printf(RED "\n✗ Opção inválida!\n" RESET);
            continue;
        }
        limpar_buffer_input();
        
        switch (opcao) {
            case 1: {
                // Criar novo formulário
                Form *novoForm = construir_formulario_interativo();
                if (novoForm) {
                    formAtual = novoForm;
                    printf(GREEN "\n✓ Formulário criado e ativado!\n" RESET);
                }
                break;
            }
            
            case 2: {
                // Abrir formulário existente
                listar_formularios();
                printf(YELLOW "\n[Em desenvolvimento]\n" RESET);
                printf("Pressione ENTER para continuar...");
                getchar();
                break;
            }
            
            case 3: {
                // Cadastrar dados
                if (!formAtual) {
                    printf(RED "\n✗ Nenhum formulário aberto!\n" RESET);
                    printf("Crie ou abra um formulário primeiro.\n");
                } else {
                    // Cria ou carrega recordset
                    RecordSet *recordset = criar_recordset(formAtual);
                    
                    // Tenta carregar dados existentes
                    char filepath[300];
                    snprintf(filepath, sizeof(filepath), "data/records/%s.csv", formAtual->name);
                    
                    RecordSet *loaded = carregar_registros_csv(formAtual, filepath);
                    if (loaded) {
                        liberar_recordset(recordset);
                        recordset = loaded;
                        printf(GREEN "\n✓ Dados carregados de %s\n" RESET, filepath);
                    }
                    
                    menu_cadastro(formAtual, recordset);
                    
                    liberar_recordset(recordset);
                }
                break;
            }
            
            case 4: {
                // Ver registros
                if (!formAtual) {
                    printf(RED "\n✗ Nenhum formulário aberto!\n" RESET);
                } else {
                    RecordSet *recordset = criar_recordset(formAtual);
                    
                    char filepath[300];
                    snprintf(filepath, sizeof(filepath), "data/records/%s.csv", formAtual->name);
                    
                    RecordSet *loaded = carregar_registros_csv(formAtual, filepath);
                    if (loaded) {
                        liberar_recordset(recordset);
                        recordset = loaded;
                    }
                    
                    visualizar_registros(recordset);
                    
                    liberar_recordset(recordset);
                }
                break;
            }
            
            case 5: {
                // Buscar
                printf(YELLOW "\n[Sistema de busca - Em desenvolvimento]\n" RESET);
                printf("Pressione ENTER para continuar...");
                getchar();
                break;
            }
            
            case 6: {
                // Exportar
                printf(YELLOW "\n[Exportação - Em desenvolvimento]\n" RESET);
                printf("Formatos disponíveis: CSV, JSON, Excel, PDF\n");
                printf("Pressione ENTER para continuar...");
                getchar();
                break;
            }
            
            case 7: {
                // Importar
                printf(YELLOW "\n[Importação - Em desenvolvimento]\n" RESET);
                printf("Formatos suportados: CSV, JSON, Excel\n");
                printf("Pressione ENTER para continuar...");
                getchar();
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
                printf(YELLOW "\n[Gerenciamento - Em desenvolvimento]\n" RESET);
                printf("Editar, excluir, duplicar formulários\n");
                printf("Pressione ENTER para continuar...");
                getchar();
                break;
            }
            
            case 10: {
                // Templates
                printf(BOLD_CYAN "\n╔════════════════════════════════════════════════╗\n" RESET);
                printf(BOLD_CYAN "║          TEMPLATES DISPONÍVEIS                 ║\n" RESET);
                printf(BOLD_CYAN "╚════════════════════════════════════════════════╝\n" RESET);
                printf("\n" GREEN "Negócios:\n" RESET);
                printf("  • Controle de Estoque/Produtos\n");
                printf("  • Cadastro de Clientes\n");
                printf("  • Pedidos e Vendas\n");
                printf("  • Fornecedores\n");
                printf("\n" GREEN "Saúde:\n" RESET);
                printf("  • Pacientes\n");
                printf("  • Consultas\n");
                printf("  • Animais (Veterinária)\n");
                printf("\n" GREEN "Educação:\n" RESET);
                printf("  • Alunos\n");
                printf("  • Professores\n");
                printf("  • Cursos\n");
                printf("\n" GREEN "Pessoal:\n" RESET);
                printf("  • Contatos\n");
                printf("  • Tarefas\n");
                printf("  • Finanças Pessoais\n");
                printf("\n" YELLOW "[Implementação em desenvolvimento]\n" RESET);
                printf("Pressione ENTER para continuar...");
                getchar();
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