#include "../core/record.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Função auxiliar para escapar strings no CSV (trata aspas e separadores)
static void escape_csv(const char *input, char *output) {
    int j = 0;
    output[j++] = '"';
    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] == '"') {
            output[j++] = '"'; // Aspas duplas para escapar aspas
        }
        output[j++] = input[i];
    }
    output[j++] = '"';
    output[j] = '\0';
}

bool salvar_registros_csv(RecordSet *recordset, const char *filepath) {
    if (!recordset || !filepath) return false;
    
    FILE *f = fopen(filepath, "w");
    if (!f) return false;
    
    // BOM para Excel reconhecer acentos (UTF-8)
    fprintf(f, "\xEF\xBB\xBF");
    
    // Cabeçalho
    fprintf(f, "ID;Data Criacao");
    for (int i = 0; i < recordset->form->numFields; i++) {
        fprintf(f, ";\"%s\"", recordset->form->fields[i]->label);
    }
    fprintf(f, "\n");
    
    // Registros
    char buffer[2048];
    char timeStr[20];
    
    for (int i = 0; i < recordset->numRecords; i++) {
        Record *rec = recordset->records[i];
        
        // Formatar data
        struct tm *tm_info = localtime(&rec->createdAt);
        strftime(timeStr, 20, "%Y-%m-%d %H:%M:%S", tm_info);
        
        fprintf(f, "%d;%s", rec->id, timeStr);
        
        for (int j = 0; j < recordset->form->numFields; j++) {
            int fieldId = recordset->form->fields[j]->id;
            const char *val = obter_valor_campo(rec, fieldId);
            
            // Se tiver caracteres especiais, escapa
            if (strpbrk(val, ";\"\n")) {
                escape_csv(val, buffer);
                fprintf(f, ";%s", buffer);
            } else {
                fprintf(f, ";%s", val);
            }
        }
        fprintf(f, "\n");
    }
    
    fclose(f);
    return true;
}

// Wrapper para Excel (que abre CSV)
bool exportar_excel(RecordSet *recordset, const char *filepath) {
    return salvar_registros_csv(recordset, filepath);
}

RecordSet* carregar_registros_csv(Form *form, const char *filepath) {
    if (!form || !filepath) return NULL;
    
    FILE *f = fopen(filepath, "r");
    if (!f) return NULL;
    
    RecordSet *rs = criar_recordset(form);
    if (!rs) { fclose(f); return NULL; }
    
    char line[4096];
    
    // Ignorar cabeçalho (primeira linha)
    if (!fgets(line, sizeof(line), f)) {
        fclose(f);
        return rs;
    }
    
    while (fgets(line, sizeof(line), f)) {
        // Remover newline
        line[strcspn(line, "\r\n")] = '\0';
        if (strlen(line) == 0) continue;
        
        // Parse ID
        char *token = strtok(line, ";");
        if (!token) continue;
        int id = atoi(token);
        
        Record *rec = criar_registro(form, id);
        
        // Parse Data (ignorar por enquanto)
        token = strtok(NULL, ";");
        
        // Parse Campos
        for (int i = 0; i < form->numFields; i++) {
            token = strtok(NULL, ";");
            if (token) {
                // Remover aspas se necessário
                if (token[0] == '"') {
                    size_t len = strlen(token);
                    if (len > 1 && token[len-1] == '"') {
                        token[len-1] = '\0';
                        token++;
                    }
                }
                definir_valor_campo(rec, form->fields[i]->id, token);
            }
        }
        
        adicionar_registro(rs, rec);
        
        if (id >= form->nextRecordId) {
            form->nextRecordId = id + 1;
        }
    }
    
    fclose(f);
    return rs;
}