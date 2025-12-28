#ifndef FORM_H
#define FORM_H

#include "field.h"
#include <time.h>

#define MAX_FIELDS 50
#define MAX_FORM_NAME 100

// Estrutura de formulário
typedef struct {
    int id;                          // ID único do formulário
    char name[MAX_FORM_NAME];        // Nome interno (ex: "produtos")
    char displayName[MAX_FORM_NAME]; // Nome exibido (ex: "Cadastro de Produtos")
    char description[300];           // Descrição do formulário
    
    Field *fields[MAX_FIELDS];       // Array de campos
    int numFields;                   // Quantidade de campos
    
    time_t createdAt;                // Data de criação
    time_t modifiedAt;               // Última modificação
    
    int totalRecords;                // Total de registros cadastrados
    int nextRecordId;                // Próximo ID de registro
} Form;

// Funções de formulário
Form* criar_formulario(const char *name, const char *displayName);
void liberar_formulario(Form *form);
bool adicionar_campo_formulario(Form *form, Field *field);
bool remover_campo_formulario(Form *form, int fieldId);
Field* buscar_campo_por_nome(Form *form, const char *name);
Field* buscar_campo_por_id(Form *form, int id);
void exibir_estrutura_formulario(Form *form);
bool salvar_formulario(Form *form, const char *filepath);
Form* carregar_formulario(const char *filepath);

#endif