#include "field.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <regex.h>

static int nextFieldId = 1;

Field* criar_campo(const char *name, const char *label, FieldType type) {
    Field *field = malloc(sizeof(Field));
    if (!field) return NULL;
    
    field->id = nextFieldId++;
    strncpy(field->name, name, sizeof(field->name) - 1);
    strncpy(field->label, label, sizeof(field->label) - 1);
    field->type = type;
    field->description[0] = '\0';
    field->defaultValue[0] = '\0';
    field->calculated = false;
    field->formula[0] = '\0';
    
    // Inicializar validação padrão
    field->validation.required = false;
    field->validation.unique = false;
    field->validation.autoIncrement = false;
    field->validation.hasMin = false;
    field->validation.hasMax = false;
    field->validation.minValue = 0;
    field->validation.maxValue = 0;
    field->validation.minLength = 0;
    field->validation.maxLength = 1000;
    field->validation.decimalPlaces = 2;
    field->validation.customRegex[0] = '\0';
    field->validation.errorMessage[0] = '\0';
    
    // Inicializar choices se for tipo escolha
    if (type == FIELD_CHOICE_SINGLE || type == FIELD_CHOICE_MULTIPLE) {
        field->choices = malloc(sizeof(ChoiceOptions));
        field->choices->options = NULL;
        field->choices->numOptions = 0;
    } else {
        field->choices = NULL;
    }
    
    return field;
}

void liberar_campo(Field *field) {
    if (!field) return;
    
    if (field->choices) {
        for (int i = 0; i < field->choices->numOptions; i++) {
            free(field->choices->options[i]);
        }
        free(field->choices->options);
        free(field->choices);
    }
    
    free(field);
}

void adicionar_opcao_escolha(Field *field, const char *option) {
    if (!field || !field->choices) return;
    
    int n = field->choices->numOptions;
    field->choices->options = realloc(field->choices->options, (n + 1) * sizeof(char*));
    field->choices->options[n] = malloc(strlen(option) + 1);
    strcpy(field->choices->options[n], option);
    field->choices->numOptions++;
}

void definir_validacao(Field *field, FieldValidation validation) {
    if (!field) return;
    field->validation = validation;
}

const char* tipo_campo_string(FieldType type) {
    switch (type) {
        case FIELD_TEXT_SHORT: return "Texto Curto";
        case FIELD_TEXT_LONG: return "Texto Longo";
        case FIELD_NUMBER: return "Número";
        case FIELD_DECIMAL: return "Decimal";
        case FIELD_DATE: return "Data";
        case FIELD_TIME: return "Hora";
        case FIELD_DATETIME: return "Data/Hora";
        case FIELD_BOOLEAN: return "Sim/Não";
        case FIELD_EMAIL: return "Email";
        case FIELD_PHONE: return "Telefone";
        case FIELD_CPF: return "CPF";
        case FIELD_CNPJ: return "CNPJ";
        case FIELD_CEP: return "CEP";
        case FIELD_MONEY: return "Dinheiro";
        case FIELD_URL: return "URL";
        case FIELD_CHOICE_SINGLE: return "Lista (única)";
        case FIELD_CHOICE_MULTIPLE: return "Lista (múltipla)";
        case FIELD_RATING: return "Avaliação";
        default: return "Desconhecido";
    }
}

// ========== VALIDAÇÕES ESPECÍFICAS ==========

static bool validar_cpf(const char *cpf) {
    // Remove caracteres não numéricos
    char numeros[12] = {0};
    int j = 0;
    for (int i = 0; cpf[i] && j < 11; i++) {
        if (isdigit(cpf[i])) {
            numeros[j++] = cpf[i];
        }
    }
    
    if (j != 11) return false;
    
    // Verifica CPFs inválidos conhecidos
    bool todos_iguais = true;
    for (int i = 1; i < 11; i++) {
        if (numeros[i] != numeros[0]) {
            todos_iguais = false;
            break;
        }
    }
    if (todos_iguais) return false;
    
    // Valida primeiro dígito
    int soma = 0;
    for (int i = 0; i < 9; i++) {
        soma += (numeros[i] - '0') * (10 - i);
    }
    int digito1 = 11 - (soma % 11);
    if (digito1 >= 10) digito1 = 0;
    
    if (digito1 != (numeros[9] - '0')) return false;
    
    // Valida segundo dígito
    soma = 0;
    for (int i = 0; i < 10; i++) {
        soma += (numeros[i] - '0') * (11 - i);
    }
    int digito2 = 11 - (soma % 11);
    if (digito2 >= 10) digito2 = 0;
    
    return digito2 == (numeros[10] - '0');
}

static bool validar_cnpj(const char *cnpj) {
    char numeros[15] = {0};
    int j = 0;
    for (int i = 0; cnpj[i] && j < 14; i++) {
        if (isdigit(cnpj[i])) {
            numeros[j++] = cnpj[i];
        }
    }
    
    if (j != 14) return false;
    
    // Valida primeiro dígito
    int pesos1[] = {5,4,3,2,9,8,7,6,5,4,3,2};
    int soma = 0;
    for (int i = 0; i < 12; i++) {
        soma += (numeros[i] - '0') * pesos1[i];
    }
    int digito1 = soma % 11 < 2 ? 0 : 11 - (soma % 11);
    
    if (digito1 != (numeros[12] - '0')) return false;
    
    // Valida segundo dígito
    int pesos2[] = {6,5,4,3,2,9,8,7,6,5,4,3,2};
    soma = 0;
    for (int i = 0; i < 13; i++) {
        soma += (numeros[i] - '0') * pesos2[i];
    }
    int digito2 = soma % 11 < 2 ? 0 : 11 - (soma % 11);
    
    return digito2 == (numeros[13] - '0');
}

static bool validar_email(const char *email) {
    // Regex simples para email
    const char *pattern = "^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$";
    regex_t regex;
    int ret = regcomp(&regex, pattern, REG_EXTENDED);
    if (ret) return false;
    
    ret = regexec(&regex, email, 0, NULL, 0);
    regfree(&regex);
    
    return ret == 0;
}

static bool validar_telefone(const char *telefone) {
    // Remove não dígitos
    int j = 0;
    for (int i = 0; telefone[i]; i++) {
        if (isdigit(telefone[i])) {
            j++;
        }
    }
    
    // Aceita 10 (fixo) ou 11 (celular) dígitos
    return j == 10 || j == 11;
}

static bool validar_cep(const char *cep) {
    int j = 0;
    for (int i = 0; cep[i]; i++) {
        if (isdigit(cep[i])) {
            j++;
        }
    }
    return j == 8;
}

static bool validar_url(const char *url) {
    return strncmp(url, "http://", 7) == 0 || 
           strncmp(url, "https://", 8) == 0 ||
           strncmp(url, "ftp://", 6) == 0;
}

static bool validar_data(const char *data) {
    // Formato: dd/mm/aaaa
    if (strlen(data) != 10) return false;
    if (data[2] != '/' || data[5] != '/') return false;
    
    for (int i = 0; i < 10; i++) {
        if (i != 2 && i != 5 && !isdigit(data[i])) return false;
    }
    
    int dia, mes, ano;
    if (sscanf(data, "%d/%d/%d", &dia, &mes, &ano) != 3) return false;
    
    if (mes < 1 || mes > 12) return false;
    if (dia < 1 || dia > 31) return false;
    if (ano < 1900 || ano > 2100) return false;
    
    // Validação básica de dias por mês
    int dias_mes[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    
    // Ano bissexto
    if ((ano % 4 == 0 && ano % 100 != 0) || ano % 400 == 0) {
        dias_mes[1] = 29;
    }
    
    return dia <= dias_mes[mes - 1];
}

static bool validar_hora(const char *hora) {
    // Formato: HH:MM
    if (strlen(hora) != 5) return false;
    if (hora[2] != ':') return false;
    
    int h, m;
    if (sscanf(hora, "%d:%d", &h, &m) != 2) return false;
    
    return h >= 0 && h < 24 && m >= 0 && m < 60;
}

static bool validar_numero(const char *value) {
    if (!value || !*value) return false;
    
    // Permite sinal negativo no início
    int i = 0;
    if (value[0] == '-' || value[0] == '+') i++;
    
    for (; value[i]; i++) {
        if (!isdigit(value[i])) return false;
    }
    
    return i > (value[0] == '-' || value[0] == '+' ? 1 : 0);
}

static bool validar_decimal(const char *value) {
    if (!value || !*value) return false;
    
    int i = 0;
    bool tem_ponto = false;
    
    if (value[0] == '-' || value[0] == '+') i++;
    
    for (; value[i]; i++) {
        if (value[i] == '.' || value[i] == ',') {
            if (tem_ponto) return false; // Segundo ponto
            tem_ponto = true;
        } else if (!isdigit(value[i])) {
            return false;
        }
    }
    
    return i > (value[0] == '-' || value[0] == '+' ? 1 : 0);
}

// ========== VALIDAÇÃO PRINCIPAL ==========

bool validar_valor_campo(Field *field, const char *value, char *errorMsg) {
    if (!field) {
        strcpy(errorMsg, "Campo inválido");
        return false;
    }
    
    // Verifica obrigatório
    if (field->validation.required && (!value || strlen(value) == 0)) {
        snprintf(errorMsg, 200, "Campo '%s' é obrigatório", field->label);
        return false;
    }
    
    // Se não obrigatório e vazio, está ok
    if (!value || strlen(value) == 0) {
        return true;
    }
    
    // Validações específicas por tipo
    switch (field->type) {
        case FIELD_CPF:
            if (!validar_cpf(value)) {
                strcpy(errorMsg, "CPF inválido");
                return false;
            }
            break;
            
        case FIELD_CNPJ:
            if (!validar_cnpj(value)) {
                strcpy(errorMsg, "CNPJ inválido");
                return false;
            }
            break;
            
        case FIELD_EMAIL:
            if (!validar_email(value)) {
                strcpy(errorMsg, "Email inválido");
                return false;
            }
            break;
            
        case FIELD_PHONE:
            if (!validar_telefone(value)) {
                strcpy(errorMsg, "Telefone inválido (use 10 ou 11 dígitos)");
                return false;
            }
            break;
            
        case FIELD_CEP:
            if (!validar_cep(value)) {
                strcpy(errorMsg, "CEP inválido (use 8 dígitos)");
                return false;
            }
            break;
            
        case FIELD_URL:
            if (!validar_url(value)) {
                strcpy(errorMsg, "URL inválida (deve começar com http://, https:// ou ftp://)");
                return false;
            }
            break;
            
        case FIELD_DATE:
            if (!validar_data(value)) {
                strcpy(errorMsg, "Data inválida (use dd/mm/aaaa)");
                return false;
            }
            break;
            
        case FIELD_TIME:
            if (!validar_hora(value)) {
                strcpy(errorMsg, "Hora inválida (use HH:MM)");
                return false;
            }
            break;
            
        case FIELD_NUMBER:
            if (!validar_numero(value)) {
                strcpy(errorMsg, "Número inválido");
                return false;
            }
            // Valida min/max
            if (field->validation.hasMin || field->validation.hasMax) {
                int num = atoi(value);
                if (field->validation.hasMin && num < field->validation.minValue) {
                    snprintf(errorMsg, 200, "Valor deve ser >= %d", field->validation.minValue);
                    return false;
                }
                if (field->validation.hasMax && num > field->validation.maxValue) {
                    snprintf(errorMsg, 200, "Valor deve ser <= %d", field->validation.maxValue);
                    return false;
                }
            }
            break;
            
        case FIELD_DECIMAL:
        case FIELD_MONEY:
            if (!validar_decimal(value)) {
                strcpy(errorMsg, "Número decimal inválido");
                return false;
            }
            break;
            
        case FIELD_TEXT_SHORT:
        case FIELD_TEXT_LONG:
            // Valida comprimento
            int len = strlen(value);
            if (field->validation.minLength > 0 && len < field->validation.minLength) {
                snprintf(errorMsg, 200, "Mínimo de %d caracteres", field->validation.minLength);
                return false;
            }
            if (field->validation.maxLength > 0 && len > field->validation.maxLength) {
                snprintf(errorMsg, 200, "Máximo de %d caracteres", field->validation.maxLength);
                return false;
            }
            break;
            
        case FIELD_BOOLEAN:
            if (strcasecmp(value, "sim") != 0 && strcasecmp(value, "não") != 0 &&
                strcasecmp(value, "s") != 0 && strcasecmp(value, "n") != 0 &&
                strcasecmp(value, "true") != 0 && strcasecmp(value, "false") != 0 &&
                strcmp(value, "1") != 0 && strcmp(value, "0") != 0) {
                strcpy(errorMsg, "Use: Sim/Não, S/N, 1/0");
                return false;
            }
            break;
            
        case FIELD_CHOICE_SINGLE:
            if (field->choices) {
                bool encontrado = false;
                for (int i = 0; i < field->choices->numOptions; i++) {
                    if (strcmp(value, field->choices->options[i]) == 0) {
                        encontrado = true;
                        break;
                    }
                }
                if (!encontrado) {
                    strcpy(errorMsg, "Opção inválida");
                    return false;
                }
            }
            break;
            
        case FIELD_RATING:
            if (!validar_numero(value)) {
                strcpy(errorMsg, "Avaliação deve ser um número");
                return false;
            }
            int rating = atoi(value);
            if (rating < 1 || rating > 5) {
                strcpy(errorMsg, "Avaliação deve ser entre 1 e 5");
                return false;
            }
            break;
            
        default:
            break;
    }
    
    return true;
}