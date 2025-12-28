#ifndef FIELD_H
#define FIELD_H

#include <stdbool.h>

// Tipos de campos suportados
typedef enum {
    FIELD_TEXT_SHORT,      // Texto curto (até 100 chars)
    FIELD_TEXT_LONG,       // Texto longo (até 1000 chars)
    FIELD_NUMBER,          // Número inteiro
    FIELD_DECIMAL,         // Número decimal
    FIELD_DATE,            // Data (dd/mm/aaaa)
    FIELD_TIME,            // Hora (HH:MM)
    FIELD_DATETIME,        // Data e hora
    FIELD_BOOLEAN,         // Sim/Não
    FIELD_EMAIL,           // Email (com validação)
    FIELD_PHONE,           // Telefone (com formatação)
    FIELD_CPF,             // CPF (com validação)
    FIELD_CNPJ,            // CNPJ (com validação)
    FIELD_CEP,             // CEP (com auto-complete)
    FIELD_MONEY,           // Valor monetário (R$)
    FIELD_URL,             // URL/Link
    FIELD_CHOICE_SINGLE,   // Lista (escolhe 1)
    FIELD_CHOICE_MULTIPLE, // Lista (escolhe vários)
    FIELD_RATING           // Avaliação (1-5 estrelas)
} FieldType;

// Estrutura de opções para campos de escolha
typedef struct {
    char **options;        // Array de opções
    int numOptions;        // Quantidade de opções
} ChoiceOptions;

// Estrutura de validação
typedef struct {
    bool required;         // Campo obrigatório?
    bool unique;           // Valor deve ser único?
    bool autoIncrement;    // Sugerir próximo valor automaticamente?
    
    // Para números
    bool hasMin;
    int minValue;
    bool hasMax;
    int maxValue;
    
    // Para textos
    int minLength;
    int maxLength;
    
    // Para decimais
    int decimalPlaces;
    
    // Validação customizada
    char customRegex[200];
    char errorMessage[200];
} FieldValidation;

// Estrutura de campo
typedef struct {
    int id;                      // ID único do campo
    char name[100];              // Nome do campo (ex: "nome_produto")
    char label[100];             // Label exibido (ex: "Nome do Produto")
    char description[200];       // Descrição/ajuda
    FieldType type;              // Tipo do campo
    FieldValidation validation;  // Regras de validação
    ChoiceOptions *choices;      // Opções (se for CHOICE)
    char defaultValue[200];      // Valor padrão
    bool calculated;             // Campo calculado?
    char formula[300];           // Fórmula (se calculado)
} Field;

// Funções de campo
Field* criar_campo(const char *name, const char *label, FieldType type);
void liberar_campo(Field *field);
void adicionar_opcao_escolha(Field *field, const char *option);
void definir_validacao(Field *field, FieldValidation validation);
const char* tipo_campo_string(FieldType type);
bool validar_valor_campo(Field *field, const char *value, char *errorMsg);

#endif