#ifndef RECORD_H
#define RECORD_H

#include "form.h"
#include <time.h>

#define MAX_VALUE_LENGTH 1000

// Estrutura de valor de campo
typedef struct {
    int fieldId;                     // ID do campo
    char value[MAX_VALUE_LENGTH];    // Valor armazenado como string
} FieldValue;

// Estrutura de registro (uma linha de dados)
typedef struct {
    int id;                          // ID único do registro
    int formId;                      // ID do formulário pai
    
    FieldValue *values;              // Array de valores
    int numValues;                   // Quantidade de valores
    
    time_t createdAt;                // Data de criação
    time_t modifiedAt;               // Última modificação
    char createdBy[50];              // Criado por (se houver auth)
    char modifiedBy[50];             // Modificado por
} Record;

// Estrutura para gerenciar múltiplos registros
typedef struct {
    Record **records;                // Array de ponteiros para registros
    int numRecords;                  // Quantidade de registros
    int capacity;                    // Capacidade alocada
    Form *form;                      // Formulário associado
} RecordSet;

// Funções de registro
Record* criar_registro(Form *form, int recordId);
void liberar_registro(Record *record);
bool definir_valor_campo(Record *record, int fieldId, const char *value);
const char* obter_valor_campo(Record *record, int fieldId);
bool validar_registro(Record *record, Form *form, char *errorMsg);

// Funções de conjunto de registros
RecordSet* criar_recordset(Form *form);
void liberar_recordset(RecordSet *recordset);
bool adicionar_registro(RecordSet *recordset, Record *record);
bool remover_registro(RecordSet *recordset, int recordId);
Record* buscar_registro_por_id(RecordSet *recordset, int recordId);
void ordenar_registros(RecordSet *recordset, int fieldId, bool ascending);

// Persistência
bool salvar_registros_csv(RecordSet *recordset, const char *filepath);
RecordSet* carregar_registros_csv(Form *form, const char *filepath);
bool exportar_json(RecordSet *recordset, const char *filepath);
bool exportar_excel(RecordSet *recordset, const char *filepath);

#endif