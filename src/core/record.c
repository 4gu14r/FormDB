#include "record.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// --- Funções de Registro ---

Record* criar_registro(Form *form, int recordId) {
    if (!form) return NULL;
    
    Record *record = (Record*) malloc(sizeof(Record));
    if (!record) return NULL;
    
    record->id = recordId;
    record->formId = form->id;
    record->createdAt = time(NULL);
    record->modifiedAt = time(NULL);
    record->numValues = 0;
    record->values = NULL;
    
    // Alocar espaço para valores baseado nos campos do formulário
    if (form->numFields > 0) {
        record->values = (FieldValue*) malloc(sizeof(FieldValue) * form->numFields);
        if (record->values) {
            record->numValues = form->numFields;
            // Inicializar valores
            for (int i = 0; i < form->numFields; i++) {
                record->values[i].fieldId = form->fields[i]->id;
                strcpy(record->values[i].value, "");
                
                // Se tiver valor padrão
                if (strlen(form->fields[i]->defaultValue) > 0) {
                    strncpy(record->values[i].value, form->fields[i]->defaultValue, MAX_VALUE_LENGTH - 1);
                    record->values[i].value[MAX_VALUE_LENGTH - 1] = '\0';
                }
            }
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
    if (!record || !record->values || !value) return false;
    
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
    if (!record || !record->values) return "";
    
    for (int i = 0; i < record->numValues; i++) {
        if (record->values[i].fieldId == fieldId) {
            return record->values[i].value;
        }
    }
    return "";
}

bool validar_registro(Record *record, Form *form, char *errorMsg) {
    if (!record || !form) return false;
    
    for (int i = 0; i < form->numFields; i++) {
        Field *field = form->fields[i];
        const char *val = obter_valor_campo(record, field->id);
        
        // Validação de obrigatório
        if (field->validation.required && strlen(val) == 0) {
            if (errorMsg) sprintf(errorMsg, "Campo '%s' é obrigatório.", field->label);
            return false;
        }
    }
    return true;
}

// --- Funções de RecordSet ---

RecordSet* criar_recordset(Form *form) {
    RecordSet *rs = (RecordSet*) malloc(sizeof(RecordSet));
    if (!rs) return NULL;
    
    rs->form = form;
    rs->numRecords = 0;
    rs->capacity = 10;
    rs->records = (Record**) malloc(sizeof(Record*) * rs->capacity);
    
    if (!rs->records) {
        free(rs);
        return NULL;
    }
    
    return rs;
}

void liberar_recordset(RecordSet *recordset) {
    if (!recordset) return;
    
    if (recordset->records) {
        for (int i = 0; i < recordset->numRecords; i++) {
            liberar_registro(recordset->records[i]);
        }
        free(recordset->records);
    }
    free(recordset);
}

bool adicionar_registro(RecordSet *recordset, Record *record) {
    if (!recordset || !record) return false;
    
    if (recordset->numRecords >= recordset->capacity) {
        int newCapacity = recordset->capacity * 2;
        Record **newRecords = (Record**) realloc(recordset->records, sizeof(Record*) * newCapacity);
        if (!newRecords) return false;
        
        recordset->records = newRecords;
        recordset->capacity = newCapacity;
    }
    
    recordset->records[recordset->numRecords++] = record;
    return true;
}

bool remover_registro(RecordSet *recordset, int recordId) {
    if (!recordset) return false;
    
    for (int i = 0; i < recordset->numRecords; i++) {
        if (recordset->records[i]->id == recordId) {
            liberar_registro(recordset->records[i]);
            
            // Shift left
            for (int j = i; j < recordset->numRecords - 1; j++) {
                recordset->records[j] = recordset->records[j+1];
            }
            recordset->numRecords--;
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

void ordenar_registros(RecordSet *recordset, int fieldId, bool ascending) {
    if (!recordset || recordset->numRecords < 2) return;
    
    for (int i = 0; i < recordset->numRecords - 1; i++) {
        for (int j = 0; j < recordset->numRecords - i - 1; j++) {
            const char *val1 = obter_valor_campo(recordset->records[j], fieldId);
            const char *val2 = obter_valor_campo(recordset->records[j+1], fieldId);
            
            int cmp = strcmp(val1, val2);
            bool swap = ascending ? (cmp > 0) : (cmp < 0);
            
            if (swap) {
                Record *temp = recordset->records[j];
                recordset->records[j] = recordset->records[j+1];
                recordset->records[j+1] = temp;
            }
        }
    }
}