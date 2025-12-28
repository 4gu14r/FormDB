#include "record.h"
#include "field.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

Record* criar_registro(Form *form, int recordId) {
    if (!form) return NULL;
    
    Record *record = malloc(sizeof(Record));
    if (!record) return NULL;
    
    record->id = recordId;
    record->formId = form->id;
    record->numValues = form->numFields;
    record->createdAt = time(NULL);
    record->modifiedAt = time(NULL);
    record->createdBy[0] = '\0';
    record->modifiedBy[0] = '\0';
    
    // Aloca array de valores
    record->values = malloc(form->numFields * sizeof(FieldValue));
    
    // Inicializa valores com defaults ou vazio
    for (int i = 0; i < form->numFields; i++) {
        record->values[i].fieldId = form->fields[i]->id;
        
        if (form->fields[i]->defaultValue[0]) {
            strncpy(record->values[i].value, form->fields[i]->defaultValue, MAX_VALUE_LENGTH - 1);
        } else {
            record->values[i].value[0] = '\0';
        }
    }
    
    return record;
}

void liberar_registro(Record *record) {
    if (!record) return;
    
    if (record->values) {
        free(record->values);
    }
    
    free(record);
}

bool definir_valor_campo(Record *record, int fieldId, const char *value) {
    if (!record || !value) return false;
    
    for (int i = 0; i < record->numValues; i++) {
        if (record->values[i].fieldId == fieldId) {
            strncpy(record->values[i].value, value, MAX_VALUE_LENGTH - 1);
            record->values[i].value[MAX_VALUE_LENGTH - 1] = '\0';
            record->modifiedAt = time(NULL);
            return true;
        }
    }
    
    return false;
}

const char* obter_valor_campo(Record *record, int fieldId) {
    if (!record) return NULL;
    
    for (int i = 0; i < record->numValues; i++) {
        if (record->values[i].fieldId == fieldId) {
            return record->values[i].value;
        }
    }
    
    return NULL;
}

bool validar_registro(Record *record, Form *form, char *errorMsg) {
    if (!record || !form) {
        strcpy(errorMsg, "Registro ou formulário inválido");
        return false;
    }
    
    // Valida cada campo
    for (int i = 0; i < form->numFields; i++) {
        Field *field = form->fields[i];
        const char *value = obter_valor_campo(record, field->id);
        
        if (!validar_valor_campo(field, value, errorMsg)) {
            return false;
        }
    }
    
    return true;
}

// ========== RECORDSET ==========

RecordSet* criar_recordset(Form *form) {
    if (!form) return NULL;
    
    RecordSet *recordset = malloc(sizeof(RecordSet));
    if (!recordset) return NULL;
    
    recordset->form = form;
    recordset->numRecords = 0;
    recordset->capacity = 10;
    recordset->records = malloc(recordset->capacity * sizeof(Record*));
    
    return recordset;
}

void liberar_recordset(RecordSet *recordset) {
    if (!recordset) return;
    
    for (int i = 0; i < recordset->numRecords; i++) {
        liberar_registro(recordset->records[i]);
    }
    
    free(recordset->records);
    free(recordset);
}

bool adicionar_registro(RecordSet *recordset, Record *record) {
    if (!recordset || !record) return false;
    
    // Expande array se necessário
    if (recordset->numRecords >= recordset->capacity) {
        recordset->capacity *= 2;
        recordset->records = realloc(recordset->records, recordset->capacity * sizeof(Record*));
    }
    
    recordset->records[recordset->numRecords++] = record;
    recordset->form->totalRecords = recordset->numRecords;
    
    return true;
}

bool remover_registro(RecordSet *recordset, int recordId) {
    if (!recordset) return false;
    
    for (int i = 0; i < recordset->numRecords; i++) {
        if (recordset->records[i]->id == recordId) {
            liberar_registro(recordset->records[i]);
            
            // Shift registros restantes
            for (int j = i; j < recordset->numRecords - 1; j++) {
                recordset->records[j] = recordset->records[j + 1];
            }
            
            recordset->numRecords--;
            recordset->form->totalRecords = recordset->numRecords;
            return true;
        }
    }
    
    return false;
}

Record* buscar_registro_por_id(RecordSet *recordset, int recordId) {
    if (!recordset) return NULL;
    
    for (int i = 0; i < recordset->numRecords; i++) {
        if (recordset->records[i]->id == recordId) {
            return recordset->records[i];
        }
    }
    
    return NULL;
}

static int comparar_registros_por_campo(const void *a, const void *b, int fieldId, bool ascending) {
    Record *r1 = *(Record**)a;
    Record *r2 = *(Record**)b;
    
    const char *v1 = obter_valor_campo(r1, fieldId);
    const char *v2 = obter_valor_campo(r2, fieldId);
    
    if (!v1) v1 = "";
    if (!v2) v2 = "";
    
    int result = strcmp(v1, v2);
    return ascending ? result : -result;
}

void ordenar_registros(RecordSet *recordset, int fieldId, bool ascending) {
    if (!recordset || recordset->numRecords < 2) return;
    
    // Bubble sort simples (pode otimizar depois)
    for (int i = 0; i < recordset->numRecords - 1; i++) {
        for (int j = 0; j < recordset->numRecords - i - 1; j++) {
            int cmp = comparar_registros_por_campo(
                &recordset->records[j], 
                &recordset->records[j + 1], 
                fieldId, 
                ascending
            );
            
            if (cmp > 0) {
                // Troca
                Record *temp = recordset->records[j];
                recordset->records[j] = recordset->records[j + 1];
                recordset->records[j + 1] = temp;
            }
        }
    }
}

// ========== PERSISTÊNCIA CSV ==========

bool salvar_registros_csv(RecordSet *recordset, const char *filepath) {
    if (!recordset || !filepath) return false;
    
    FILE *f = fopen(filepath, "w");
    if (!f) return false;
    
    Form *form = recordset->form;
    
    // Escreve cabeçalho
    for (int i = 0; i < form->numFields; i++) {
        fprintf(f, "%s", form->fields[i]->label);
        if (i < form->numFields - 1) {
            fprintf(f, ",");
        }
    }
    fprintf(f, "\n");
    
    // Escreve registros
    for (int i = 0; i < recordset->numRecords; i++) {
        Record *record = recordset->records[i];
        
        for (int j = 0; j < form->numFields; j++) {
            const char *value = obter_valor_campo(record, form->fields[j]->id);
            
            // Escapa vírgulas e aspas
            if (value && (strchr(value, ',') || strchr(value, '"') || strchr(value, '\n'))) {
                fprintf(f, "\"");
                for (const char *p = value; *p; p++) {
                    if (*p == '"') fprintf(f, "\"\"");
                    else fprintf(f, "%c", *p);
                }
                fprintf(f, "\"");
            } else {
                fprintf(f, "%s", value ? value : "");
            }
            
            if (j < form->numFields - 1) {
                fprintf(f, ",");
            }
        }
        fprintf(f, "\n");
    }
    
    fclose(f);
    return true;
}

RecordSet* carregar_registros_csv(Form *form, const char *filepath) {
    if (!form || !filepath) return NULL;
    
    FILE *f = fopen(filepath, "r");
    if (!f) return NULL;
    
    RecordSet *recordset = criar_recordset(form);
    
    char linha[4096];
    
    // Pula cabeçalho
    fgets(linha, sizeof(linha), f);
    
    int recordId = 1;
    
    // Lê registros
    while (fgets(linha, sizeof(linha), f)) {
        linha[strcspn(linha, "\n")] = '\0';
        
        Record *record = criar_registro(form, recordId++);
        
        char *token = strtok(linha, ",");
        int fieldIndex = 0;
        
        while (token && fieldIndex < form->numFields) {
            // Remove aspas se houver
            if (token[0] == '"') {
                int len = strlen(token);
                if (len > 1 && token[len - 1] == '"') {
                    token[len - 1] = '\0';
                    token++;
                }
            }
            
            definir_valor_campo(record, form->fields[fieldIndex]->id, token);
            
            token = strtok(NULL, ",");
            fieldIndex++;
        }
        
        adicionar_registro(recordset, record);
    }
    
    fclose(f);
    return recordset;
}

// ========== EXPORTAÇÃO JSON ==========

bool exportar_json(RecordSet *recordset, const char *filepath) {
    if (!recordset || !filepath) return false;
    
    FILE *f = fopen(filepath, "w");
    if (!f) return false;
    
    Form *form = recordset->form;
    
    fprintf(f, "{\n");
    fprintf(f, "  \"formulario\": \"%s\",\n", form->displayName);
    fprintf(f, "  \"total\": %d,\n", recordset->numRecords);
    fprintf(f, "  \"registros\": [\n");
    
    for (int i = 0; i < recordset->numRecords; i++) {
        Record *record = recordset->records[i];
        
        fprintf(f, "    {\n");
        fprintf(f, "      \"id\": %d,\n", record->id);
        
        for (int j = 0; j < form->numFields; j++) {
            const char *value = obter_valor_campo(record, form->fields[j]->id);
            
            fprintf(f, "      \"%s\": ", form->fields[j]->name);
            
            // Escapa JSON
            if (value) {
                fprintf(f, "\"");
                for (const char *p = value; *p; p++) {
                    if (*p == '"') fprintf(f, "\\\"");
                    else if (*p == '\\') fprintf(f, "\\\\");
                    else if (*p == '\n') fprintf(f, "\\n");
                    else fprintf(f, "%c", *p);
                }
                fprintf(f, "\"");
            } else {
                fprintf(f, "null");
            }
            
            if (j < form->numFields - 1) {
                fprintf(f, ",\n");
            } else {
                fprintf(f, "\n");
            }
        }
        
        fprintf(f, "    }");
        if (i < recordset->numRecords - 1) {
            fprintf(f, ",\n");
        } else {
            fprintf(f, "\n");
        }
    }
    
    fprintf(f, "  ]\n");
    fprintf(f, "}\n");
    
    fclose(f);
    return true;
}

// ========== EXPORTAÇÃO EXCEL (CSV com codificação) ==========

bool exportar_excel(RecordSet *recordset, const char *filepath) {
    // Por enquanto, usa o mesmo formato CSV
    // Futuramente pode adicionar suporte a .xlsx via biblioteca externa
    return salvar_registros_csv(recordset, filepath);
}