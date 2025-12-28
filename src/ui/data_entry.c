// Habilita extensões POSIX (necessário para strcasecmp com -std=c11)
#define _DEFAULT_SOURCE

#include "data_entry.h"
#include "../core/form.h"
#include "../core/field.h"
#include "../core/record.h"
#include "../utils/colors.h"
#include "../utils/ui_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <errno.h>

static void exibir_campo_info(Field *field) {
    printf(BOLD_CYAN "\n%s" RESET, field->label);
    
    if (field->validation.required) {
        printf(RED " *" RESET);
    }
    
    printf(" (%s)\n", tipo_campo_string(field->type));
    
    if (field->description[0]) {
        printf(DIM "  %s\n" RESET, field->description);
    }
    
    // Mostra restrições
    if (field->validation.hasMin) {
        printf(YELLOW "  • Mínimo: %d\n" RESET, field->validation.minValue);
    }
    if (field->validation.hasMax) {
        printf(YELLOW "  • Máximo: %d\n" RESET, field->validation.maxValue);
    }
    if (field->validation.minLength > 0) {
        printf(YELLOW "  • Mín. caracteres: %d\n" RESET, field->validation.minLength);
    }
    if (field->type == FIELD_TEXT_SHORT && field->validation.maxLength < 100) {
        printf(YELLOW "  • Máx. caracteres: %d\n" RESET, field->validation.maxLength);
    }
    
    // Mostra opções se for escolha
    if (field->choices && field->choices->numOptions > 0) {
        printf(CYAN "  Opções:\n" RESET);
        for (int i = 0; i < field->choices->numOptions; i++) {
            printf(DIM "    %d. %s\n" RESET, i + 1, field->choices->options[i]);
        }
    }
    
    // Mostra valor padrão
    if (field->defaultValue[0]) {
        printf(GREEN "  [Padrão: %s]\n" RESET, field->defaultValue);
    }
}

static bool ler_valor_campo(Field *field, char *value, size_t maxLen) {
    printf(BOLD_WHITE "\n→ " RESET);
    
    // Para campos booleanos, simplifica
    if (field->type == FIELD_BOOLEAN) {
        printf("(s/n): ");
        char resposta = ler_confirmacao();
        
        if (resposta == 's' || resposta == 'S') {
            strcpy(value, "Sim");
        } else if (resposta == 'n' || resposta == 'N') {
            strcpy(value, "Não");
        } else {
            // Se entrada vazia/inválida, verifica valor padrão
            if (field->defaultValue[0] && (strcmp(field->defaultValue, "1") == 0 || 
                strcasecmp(field->defaultValue, "sim") == 0 || 
                strcasecmp(field->defaultValue, "true") == 0)) {
                strcpy(value, "Sim");
            } else {
                strcpy(value, "Não");
            }
        }
        return true;
    }
    
    // Para escolhas, mostra menu
    if (field->type == FIELD_CHOICE_SINGLE && field->choices) {
        printf("Digite o número da opção: ");
        
        char input[50];
        if (!ler_string_segura(input, sizeof(input))) {
            return false;
        }
        
        // Se vazio e tem padrão, usa o padrão
        if (strlen(input) == 0 && field->defaultValue[0]) {
            strncpy(value, field->defaultValue, maxLen - 1);
            return true;
        }
        
        int opcao = atoi(input);
        
        if (opcao < 1 || opcao > field->choices->numOptions) {
            printf(RED "Opção inválida!\n" RESET);
            return false;
        }
        
        strncpy(value, field->choices->options[opcao - 1], maxLen - 1);
        return true;
    }
    
    // Para avaliação (estrelas)
    if (field->type == FIELD_RATING) {
        printf("Avaliação (1-5 estrelas): ");
        
        char input[50];
        if (!ler_string_segura(input, sizeof(input))) {
            return false;
        }
        
        // Se vazio e tem padrão, usa o padrão
        if (strlen(input) == 0 && field->defaultValue[0]) {
            strncpy(value, field->defaultValue, maxLen - 1);
            return true;
        }
        
        int rating = atoi(input);
        if (rating < 1 || rating > 5) {
            printf(RED "Use valores entre 1 e 5\n" RESET);
            return false;
        }
        snprintf(value, maxLen, "%d", rating);
        return true;
    }
    
    // Para outros tipos, leitura normal
    if (!ler_string_segura(value, maxLen)) {
        return false;
    }
    
    // Se vazio e tem valor padrão, usa o padrão
    if (strlen(value) == 0 && field->defaultValue[0]) {
        strcpy(value, field->defaultValue);
    }
    
    return true;
}

// Verifica se o valor é único no conjunto de registros
static bool verificar_unicidade(RecordSet *rs, int fieldId, const char *valor) {
    if (!rs || rs->numRecords == 0) return true;

    for (int i = 0; i < rs->numRecords; i++) {
        const char *existingVal = obter_valor_campo(rs->records[i], fieldId);
        if (existingVal && strcmp(existingVal, valor) == 0) {
            return false;
        }
    }
    return true;
}

// Calcula o próximo valor numérico para campos auto-incremento
static void calcular_proximo_valor_auto(RecordSet *rs, int fieldId, char *buffer, size_t maxLen) {
    long maxVal = 0;
    bool found = false;
    
    for (int i = 0; i < rs->numRecords; i++) {
        const char *valStr = obter_valor_campo(rs->records[i], fieldId);
        if (valStr && *valStr) {
            char *endptr;
            long val = strtol(valStr, &endptr, 10);
            // Se conseguiu ler um número válido no início da string
            if (endptr != valStr) { 
                if (!found || val > maxVal) {
                    maxVal = val;
                    found = true;
                }
            }
        }
    }
    snprintf(buffer, maxLen, "%ld", found ? maxVal + 1 : 1);
}

bool cadastrar_registro_interativo(Form *form, RecordSet *recordset) {
    if (!form || !recordset) return false;
    
    limpar_tela();
    
    printf(BOLD_CYAN "╔════════════════════════════════════════════════════╗\n" RESET);
    printf(BOLD_CYAN "║          CADASTRAR NOVO REGISTRO                   ║\n" RESET);
    printf(BOLD_CYAN "╚════════════════════════════════════════════════════╝\n" RESET);
    
    printf(GREEN "\nFormulário: %s\n" RESET, form->displayName);
    printf(DIM "Preencha os campos abaixo. Use ENTER vazio para valor padrão.\n" RESET);
    printf(RED "* = Campo obrigatório\n" RESET);
    
    // Cria novo registro
    Record *record = criar_registro(form, form->nextRecordId);
    if (!record) {
        printf(RED "\nErro ao criar registro!\n" RESET);
        return false;
    }
    
    char errorMsg[200];
    bool sucesso = true;
    
    // Para cada campo, solicita entrada
    for (int i = 0; i < form->numFields; i++) {
        Field *field = form->fields[i];
        
        // Pula campos calculados
        if (field->calculated) {
            continue;
        }
        
        // Se for auto-incremento, gera o valor automaticamente e pula a digitação
        if (field->validation.autoIncrement) {
            char autoVal[MAX_VALUE_LENGTH];
            calcular_proximo_valor_auto(recordset, field->id, autoVal, sizeof(autoVal));
            
            exibir_campo_info(field);
            printf(BOLD_WHITE "\n→ " RESET GREEN "%s (Gerado automaticamente)\n" RESET, autoVal);
            definir_valor_campo(record, field->id, autoVal);
            continue;
        }
        
        char value[MAX_VALUE_LENGTH];
        bool valorValido = false;
        int tentativas = 0;
        const int MAX_TENTATIVAS = 3;
        
        while (!valorValido && tentativas < MAX_TENTATIVAS) {
            exibir_campo_info(field);
            
            if (!ler_valor_campo(field, value, sizeof(value))) {
                printf(RED "Erro ao ler valor!\n" RESET);
                tentativas++;
                continue;
            }
            
            // Valida valor
            if (validar_valor_campo(field, value, errorMsg)) {
                // Verifica unicidade se necessário
                if (field->validation.unique && !verificar_unicidade(recordset, field->id, value)) {
                    printf(RED "✗ Erro: O valor '%s' já existe e este campo deve ser único.\n" RESET, value);
                    tentativas++;
                    if (tentativas < MAX_TENTATIVAS) {
                        printf(YELLOW "Tente novamente (%d/%d)\n" RESET, tentativas, MAX_TENTATIVAS);
                    }
                } else {
                    definir_valor_campo(record, field->id, value);
                    valorValido = true;
                    printf(GREEN "✓ Valor aceito\n" RESET);
                }
            } else {
                printf(RED "✗ %s\n" RESET, errorMsg);
                tentativas++;
                
                if (tentativas < MAX_TENTATIVAS) {
                    printf(YELLOW "Tente novamente (%d/%d)\n" RESET, 
                           tentativas, MAX_TENTATIVAS);
                }
            }
        }
        
        if (!valorValido) {
            printf(RED "\nLimite de tentativas atingido para campo '%s'\n" RESET, 
                   field->label);
            printf("Deseja continuar mesmo assim? (s/n): ");
            char resp = ler_confirmacao();
            
            if (resp != 's' && resp != 'S') {
                liberar_registro(record);
                return false;
            }
        }
    }
    
    // Validação final do registro completo
    if (validar_registro(record, form, errorMsg)) {
        adicionar_registro(recordset, record);
        form->nextRecordId++;
        
        // Salvar automaticamente no disco para evitar perda de dados
        char filepath[300];
        snprintf(filepath, sizeof(filepath), "data/records/%s.csv", form->name);
        salvar_registros_csv(recordset, filepath);
        
        printf(BOLD_GREEN "\n✓ Registro cadastrado e salvo com sucesso!\n" RESET);
        printf(GREEN "ID do registro: %d\n" RESET, record->id);
        printf(GREEN "Total de registros: %d\n" RESET, recordset->numRecords);
        
        sucesso = true;
    } else {
        printf(RED "\n✗ Erro na validação final: %s\n" RESET, errorMsg);
        liberar_registro(record);
        sucesso = false;
    }
    
    printf("\nPressione ENTER para continuar...");
    limpar_buffer();
    
    return sucesso;
}

void visualizar_registros(RecordSet *recordset) {
    if (!recordset || !recordset->form) return;
    
    Form *form = recordset->form;
    
    while (1) {
        limpar_tela();
        desenhar_cabecalho("REGISTROS CADASTRADOS");
        
        printf(GREEN "\nFormulário: %s\n" RESET, form->displayName);
        printf(GREEN "Total de registros: %d\n" RESET, recordset->numRecords);
        
        if (recordset->numRecords == 0) {
            printf(YELLOW "\nNenhum registro cadastrado ainda.\n" RESET);
            printf("\nPressione ENTER para continuar...");
            if (esperar_enter_check_resize()) continue;
            return;
        }
        
        printf("\n");
        
        // Exibe cada registro
        for (int i = 0; i < recordset->numRecords; i++) {
            Record *record = recordset->records[i];
            
            desenhar_separador();
            printf(BOLD_WHITE "Registro #%d\n" RESET, record->id);
            desenhar_separador();
            
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
            
            printf("\n");
        }
        
        desenhar_separador();
        printf("\nPressione ENTER para continuar...");
        if (esperar_enter_check_resize()) continue;
        break;
    }
}

void menu_cadastro(Form *form, RecordSet *recordset) {
    if (!form || !recordset) return;
    
    int opcao;
    
    do {
        limpar_tela();
        desenhar_cabecalho("MENU DE CADASTRO");
        
        printf(GREEN "\nFormulário: %s\n" RESET, form->displayName);
        printf(GREEN "Registros cadastrados: %d\n" RESET, recordset->numRecords);
        
        printf("\n");
        desenhar_separador();
        printf("1. Cadastrar novo registro\n");
        printf("2. Ver todos os registros\n");
        printf("3. Salvar em CSV\n");
        printf("4. Exportar JSON\n");
        printf("0. Voltar\n");
        desenhar_separador();
        
        printf("\nEscolha: ");
        
        // Lógica de responsividade (igual ao menu principal)
        int result = scanf("%d", &opcao);
        if (result == EOF && errno == EINTR) {
            continue; // Redesenha a tela
        }
        
        if (result != 1) {
            limpar_buffer();
            printf(RED "Opção inválida!\n" RESET);
            continue;
        }
        limpar_buffer();
        
        switch (opcao) {
            case 1:
                cadastrar_registro_interativo(form, recordset);
                break;
                
            case 2:
                visualizar_registros(recordset);
                break;
                
            case 3: {
                char filepath[300];
                snprintf(filepath, sizeof(filepath), "data/records/%s.csv", form->name);
                
                if (salvar_registros_csv(recordset, filepath)) {
                    printf(GREEN "\n✓ Dados salvos em: %s\n" RESET, filepath);
                } else {
                    printf(RED "\n✗ Erro ao salvar!\n" RESET);
                }
                
                printf("Pressione ENTER para continuar...");
                limpar_buffer();
                break;
            }
                
            case 4: {
                char filepath[300];
                snprintf(filepath, sizeof(filepath), "exports/%s.json", form->name);
                
                if (exportar_json(recordset, filepath)) {
                    printf(GREEN "\n✓ Dados exportados para: %s\n" RESET, filepath);
                } else {
                    printf(RED "\n✗ Erro ao exportar!\n" RESET);
                }
                
                printf("Pressione ENTER para continuar...");
                limpar_buffer();
                break;
            }
                
            case 0:
                break;
                
            default:
                printf(RED "Opção inválida!\n" RESET);
                printf("Pressione ENTER para continuar...");
                limpar_buffer();
        }
        
    } while (opcao != 0);
}