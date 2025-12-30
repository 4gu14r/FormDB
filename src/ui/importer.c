#include "importer.h"
#include "../utils/ui_utils.h"
#include "../utils/colors.h"
#include "../utils/string_utils.h"
<<<<<<< HEAD
=======
#include "../utils/app_context.h"
>>>>>>> cd0de64bb0fd98426ae2df168632884f345e13cd
#include "../core/form.h"
#include "../core/record.h"
#include "form_browser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <errno.h>

#ifdef _WIN32
    #include <direct.h>
    #define MKDIR(path) _mkdir(path)
#else
    #define MKDIR(path) mkdir(path, 0777)
#endif

// Protótipos de funções que assumimos existir no core/storage
// Caso não existam, precisarão ser implementadas no record.c/csv_handler.c
static void trim_whitespace(char *str) {
    char *end;
    while(isspace((unsigned char)*str)) str++;
    if(*str == 0) return;
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    *(end+1) = 0;
}

// Função auxiliar para limpar caminhos no estilo Windows (remove aspas e espaços)
static void clean_path(char *path) {
    if (!path) return;
    
    // Remove quebra de linha se houver
    path[strcspn(path, "\n")] = 0;
    
    // Remove espaços em branco nas pontas
    trim_whitespace(path);
    
    // Remove aspas duplas se existirem (comum no Windows "Copiar como caminho")
    size_t len = strlen(path);
    if (len >= 2 && path[0] == '"' && path[len-1] == '"') {
        memmove(path, path + 1, len - 2);
        path[len - 2] = '\0';
    }
}

static void ensure_directory_exists(const char *path) {
    struct stat st = {0};
    if (stat(path, &st) == -1) {
        MKDIR(path);
    }
}

// Parser simples de linha CSV que respeita aspas
// Retorna o próximo token e atualiza o ponteiro da linha
static char* parse_csv_token(char **line_ptr) {
    char *start = *line_ptr;
    if (start == NULL || *start == '\0') return NULL;

    // Pula espaços iniciais se não estiver entre aspas (opcional, CSV padrão não tem espaço)
    while (*start == ' ' || *start == '\t') start++;

    char *current = start;
    char *token = malloc(strlen(start) + 1);
    int token_idx = 0;
    int in_quotes = 0;

    if (*current == '"') {
        in_quotes = 1;
        current++; // Pula aspas de abertura
    }

    while (*current != '\0') {
        if (in_quotes) {
            if (*current == '"') {
                if (*(current + 1) == '"') {
                    // Aspas duplas escapadas ("")
                    token[token_idx++] = '"';
                    current += 2;
                } else {
                    // Fim das aspas
                    in_quotes = 0;
                    current++;
                }
            } else {
                token[token_idx++] = *current++;
            }
        } else {
            if (*current == ',') {
                // Fim do campo
                current++; // Pula a vírgula
                break;
            } else if (*current == '\n' || *current == '\r') {
                // Fim da linha
                *current = '\0'; // Marca fim para a próxima chamada
                break;
            } else {
                token[token_idx++] = *current++;
            }
        }
    }

    token[token_idx] = '\0';
    *line_ptr = (*current == '\0') ? NULL : current;
    
    // Limpa espaços finais se não estava entre aspas
    if (!in_quotes) {
        trim_whitespace(token);
    }

    return token;
}

void processar_importacao(Form *form) {
    char filepath[300];
    char prompt[512];
    
    snprintf(prompt, sizeof(prompt), 
        "\nFormulário selecionado: " BOLD_CYAN "%s" RESET "\n"
        "Digite o caminho do arquivo CSV para importar (ex: dados.csv): ", 
        form->displayName);

    if (!ler_texto_dialogo("IMPORTAR DADOS", prompt, filepath, sizeof(filepath))) return;
    
    // Limpa o caminho (remove aspas do Windows e espaços)
    clean_path(filepath);

    FILE *file = fopen(filepath, "r");
    if (!file) {
        char msg[512];
        snprintf(msg, sizeof(msg), RED "\n✗ Erro ao abrir arquivo: %s\nVerifique se o caminho está correto ou se o arquivo não está aberto no Excel.\n" RESET, filepath);
        printf("%s", msg);
        printf("Pressione ENTER para continuar...");
        esperar_enter_check_resize();
        return;
    }

    // Carrega dados atuais para manter integridade e IDs
<<<<<<< HEAD
    char db_path[300];
    snprintf(db_path, sizeof(db_path), "data/records/%s.csv", form->name);
    RecordSet *rs = carregar_registros_csv(form, db_path);
    if (!rs) {
        // Se não existe, cria um novo
        // Assumindo que existe uma função para criar RecordSet vazio se carregar falhar
        // Como não temos acesso ao record.c, vamos assumir que carregar retorna NULL se não existe
        // e precisaríamos criar um novo. Mas para simplificar, vamos abortar se falhar a criação básica.
        printf(YELLOW "\n⚠ Criando novo conjunto de registros...\n" RESET);
        // rs = criar_recordset(form); // Assumindo existência
        // Se carregar_registros_csv já cria vazio se não achar arquivo, ótimo.
        // Caso contrário, abortamos para evitar crash.
        fclose(file);
        return; 
=======
    char db_path[1024];
    snprintf(db_path, sizeof(db_path),
         "%s/%s.csv", APP.records, form->name);

    RecordSet *rs = carregar_registros_csv(form, db_path);
    if (!rs) {
        // Se não existe arquivo de registros, cria um novo conjunto vazio na memória
        rs = criar_recordset(form);
        if (!rs) {
            printf(RED "\n✗ Erro fatal ao inicializar memória para registros.\n" RESET);
            fclose(file);
            return;
        }
>>>>>>> cd0de64bb0fd98426ae2df168632884f345e13cd
    }

    char line[4096];
    int imported_count = 0;
    int line_num = 0;

    // Pula cabeçalho? Perguntar ao usuário seria ideal, vamos assumir que tem cabeçalho
    char tem_header = ler_confirmacao_dialogo("CONFIGURAÇÃO", "\nO arquivo possui linha de cabeçalho? (s/n): ");
    if ((tem_header == 's' || tem_header == 'S') && fgets(line, sizeof(line), file)) {
        line_num++; // Pulou linha 1
    }

    printf(YELLOW "\nImportando registros...\n" RESET);

    while (fgets(line, sizeof(line), file)) {
        line_num++;
        // Remove newline do final
        line[strcspn(line, "\n")] = 0;
        
        if (strlen(line) == 0) continue;

        Record *rec = criar_registro(form, form->nextRecordId++);
        char *line_ptr = line;
        int field_idx = 0;

        // Itera pelos campos do formulário
        while (field_idx < form->numFields) {
            char *token = parse_csv_token(&line_ptr);
            
            if (token) {
                definir_valor_campo(rec, form->fields[field_idx]->id, token);
                free(token);
            } else {
                // Campo vazio
                definir_valor_campo(rec, form->fields[field_idx]->id, "");
            }
            
            field_idx++;
        }

        adicionar_registro(rs, rec);
        imported_count++;
        
        if (imported_count % 100 == 0) {
            printf("\rProcessados: %d...", imported_count);
            fflush(stdout);
        }
    }

    fclose(file);

    // Garante que a estrutura de pastas existe antes de salvar
<<<<<<< HEAD
    ensure_directory_exists("data");
    ensure_directory_exists("data/records");
=======
    ensure_directory_exists(APP.data);
    ensure_directory_exists(APP.records);
>>>>>>> cd0de64bb0fd98426ae2df168632884f345e13cd

    // Salva o banco de dados atualizado
    if (salvar_registros_csv(rs, db_path)) {
        printf(GREEN "\n\n✓ Importação concluída com sucesso!\n" RESET);
        printf("Total de registros importados: %d\n", imported_count);
        printf("Total no banco de dados: %d\n", rs->numRecords);
    } else {
        printf(RED "\n\n✗ Erro ao salvar banco de dados atualizado!\n" RESET);
    }

    // Liberar memória (assumindo função existente)
<<<<<<< HEAD
    // liberar_recordset(rs); 
=======
    liberar_recordset(rs); 
>>>>>>> cd0de64bb0fd98426ae2df168632884f345e13cd
    
    printf("\nPressione ENTER para continuar...");
    esperar_enter_check_resize();
}

void menu_importar() {
    limpar_tela();
    desenhar_cabecalho("IMPORTAR DADOS (CSV)");
    
    printf("Selecione o formulário de destino para os dados:\n");
    
    Form *form = selecionar_formulario_interativo();
    if (!form) return;

    processar_importacao(form);
    
    // liberar_formulario(form); // Assumindo existência
}