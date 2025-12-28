#include "form.h"
#include "field.h"
#include "../utils/colors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

static int nextFormId = 1;

Form* criar_formulario(const char *name, const char *displayName) {
    Form *form = malloc(sizeof(Form));
    if (!form) return NULL;
    
    form->id = nextFormId++;
    strncpy(form->name, name, sizeof(form->name) - 1);
    strncpy(form->displayName, displayName, sizeof(form->displayName) - 1);
    form->description[0] = '\0';
    form->numFields = 0;
    form->createdAt = time(NULL);
    form->modifiedAt = time(NULL);
    form->totalRecords = 0;
    form->nextRecordId = 1;
    
    for (int i = 0; i < MAX_FIELDS; i++) {
        form->fields[i] = NULL;
    }
    
    return form;
}

void liberar_formulario(Form *form) {
    if (!form) return;
    
    for (int i = 0; i < form->numFields; i++) {
        liberar_campo(form->fields[i]);
    }
    
    free(form);
}

bool adicionar_campo_formulario(Form *form, Field *field) {
    if (!form || !field) return false;
    if (form->numFields >= MAX_FIELDS) return false;
    
    form->fields[form->numFields++] = field;
    form->modifiedAt = time(NULL);
    return true;
}

bool remover_campo_formulario(Form *form, int fieldId) {
    if (!form) return false;
    
    for (int i = 0; i < form->numFields; i++) {
        if (form->fields[i]->id == fieldId) {
            liberar_campo(form->fields[i]);
            
            // Shift campos restantes
            for (int j = i; j < form->numFields - 1; j++) {
                form->fields[j] = form->fields[j + 1];
            }
            
            form->fields[form->numFields - 1] = NULL;
            form->numFields--;
            form->modifiedAt = time(NULL);
            return true;
        }
    }
    
    return false;
}

Field* buscar_campo_por_nome(Form *form, const char *name) {
    if (!form || !name) return NULL;
    
    for (int i = 0; i < form->numFields; i++) {
        if (strcmp(form->fields[i]->name, name) == 0) {
            return form->fields[i];
        }
    }
    
    return NULL;
}

Field* buscar_campo_por_id(Form *form, int id) {
    if (!form) return NULL;
    
    for (int i = 0; i < form->numFields; i++) {
        if (form->fields[i]->id == id) {
            return form->fields[i];
        }
    }
    
    return NULL;
}

void exibir_estrutura_formulario(Form *form) {
    if (!form) return;
    
    printf(BOLD_CYAN "\n╔════════════════════════════════════════════════════════╗\n" RESET);
    printf(BOLD_CYAN "║           ESTRUTURA DO FORMULÁRIO                      ║\n" RESET);
    printf(BOLD_CYAN "╚════════════════════════════════════════════════════════╝\n" RESET);
    
    printf(GREEN "\nNome: " RESET "%s\n", form->displayName);
    printf(GREEN "Nome interno: " RESET "%s\n", form->name);
    
    if (form->description[0]) {
        printf(GREEN "Descrição: " RESET "%s\n", form->description);
    }
    
    char timeStr[20];
    struct tm *tm_info = localtime(&form->createdAt);
    strftime(timeStr, sizeof(timeStr), "%d/%m/%Y %H:%M", tm_info);
    printf(GREEN "Criado em: " RESET "%s\n", timeStr);
    
    printf(GREEN "\nTotal de campos: " RESET "%d\n", form->numFields);
    printf(GREEN "Total de registros: " RESET "%d\n\n", form->totalRecords);
    
    if (form->numFields == 0) {
        printf(YELLOW "Nenhum campo adicionado ainda.\n" RESET);
        return;
    }
    
    printf(CYAN "────────────────────────────────────────────────────────\n" RESET);
    
    for (int i = 0; i < form->numFields; i++) {
        Field *f = form->fields[i];
        
        printf(BOLD_WHITE "\n%d. %s" RESET " (%s)\n", i + 1, f->label, tipo_campo_string(f->type));
        printf(DIM "   Nome interno: %s\n" RESET, f->name);
        
        if (f->description[0]) {
            printf(DIM "   Descrição: %s\n" RESET, f->description);
        }
        
        // Validações
        if (f->validation.required) {
            printf(YELLOW "   • Obrigatório\n" RESET);
        }
        if (f->validation.unique) {
            printf(YELLOW "   • Valor único\n" RESET);
        }
        if (f->validation.hasMin) {
            printf(YELLOW "   • Mínimo: %d\n" RESET, f->validation.minValue);
        }
        if (f->validation.hasMax) {
            printf(YELLOW "   • Máximo: %d\n" RESET, f->validation.maxValue);
        }
        if (f->validation.minLength > 0) {
            printf(YELLOW "   • Comprimento mínimo: %d\n" RESET, f->validation.minLength);
        }
        if (f->validation.maxLength < 1000) {
            printf(YELLOW "   • Comprimento máximo: %d\n" RESET, f->validation.maxLength);
        }
        
        // Opções de escolha
        if (f->choices && f->choices->numOptions > 0) {
            printf(CYAN "   • Opções (%d):\n" RESET, f->choices->numOptions);
            for (int j = 0; j < f->choices->numOptions; j++) {
                printf(DIM "     - %s\n" RESET, f->choices->options[j]);
            }
        }
        
        // Valor padrão
        if (f->defaultValue[0]) {
            printf(GREEN "   • Valor padrão: %s\n" RESET, f->defaultValue);
        }
        
        // Campo calculado
        if (f->calculated) {
            printf(MAGENTA "   • Campo calculado\n" RESET);
            if (f->formula[0]) {
                printf(DIM "     Fórmula: %s\n" RESET, f->formula);
            }
        }
    }
    
    printf(CYAN "\n────────────────────────────────────────────────────────\n" RESET);
}

bool salvar_formulario(Form *form, const char *filepath) {
    if (!form || !filepath) return false;
    
    FILE *f = fopen(filepath, "wb");
    if (!f) {
        printf(RED "Erro ao abrir arquivo para escrita: %s\n" RESET, filepath);
        return false;
    }
    
    // Escreve header do formulário
    fwrite(&form->id, sizeof(int), 1, f);
    fwrite(form->name, sizeof(char), MAX_FORM_NAME, f);
    fwrite(form->displayName, sizeof(char), MAX_FORM_NAME, f);
    fwrite(form->description, sizeof(char), 300, f);
    fwrite(&form->numFields, sizeof(int), 1, f);
    fwrite(&form->createdAt, sizeof(time_t), 1, f);
    fwrite(&form->modifiedAt, sizeof(time_t), 1, f);
    fwrite(&form->totalRecords, sizeof(int), 1, f);
    fwrite(&form->nextRecordId, sizeof(int), 1, f);
    
    // Escreve cada campo
    for (int i = 0; i < form->numFields; i++) {
        Field *field = form->fields[i];
        
        // Dados básicos do campo
        fwrite(&field->id, sizeof(int), 1, f);
        fwrite(field->name, sizeof(char), 100, f);
        fwrite(field->label, sizeof(char), 100, f);
        fwrite(field->description, sizeof(char), 200, f);
        fwrite(&field->type, sizeof(FieldType), 1, f);
        fwrite(&field->validation, sizeof(FieldValidation), 1, f);
        fwrite(field->defaultValue, sizeof(char), 200, f);
        fwrite(&field->calculated, sizeof(bool), 1, f);
        fwrite(field->formula, sizeof(char), 300, f);
        
        // Choices (se houver)
        bool hasChoices = (field->choices != NULL);
        fwrite(&hasChoices, sizeof(bool), 1, f);
        
        if (hasChoices) {
            fwrite(&field->choices->numOptions, sizeof(int), 1, f);
            for (int j = 0; j < field->choices->numOptions; j++) {
                int len = strlen(field->choices->options[j]) + 1;
                fwrite(&len, sizeof(int), 1, f);
                fwrite(field->choices->options[j], sizeof(char), len, f);
            }
        }
    }
    
    fclose(f);
    return true;
}

Form* carregar_formulario(const char *filepath) {
    FILE *f = fopen(filepath, "rb");
    if (!f) {
        printf(RED "Erro ao abrir arquivo para leitura: %s\n" RESET, filepath);
        return NULL;
    }
    
    Form *form = malloc(sizeof(Form));
    if (!form) {
        fclose(f);
        return NULL;
    }
    
    // Lê header do formulário
    fread(&form->id, sizeof(int), 1, f);
    fread(form->name, sizeof(char), MAX_FORM_NAME, f);
    fread(form->displayName, sizeof(char), MAX_FORM_NAME, f);
    fread(form->description, sizeof(char), 300, f);
    fread(&form->numFields, sizeof(int), 1, f);
    fread(&form->createdAt, sizeof(time_t), 1, f);
    fread(&form->modifiedAt, sizeof(time_t), 1, f);
    fread(&form->totalRecords, sizeof(int), 1, f);
    fread(&form->nextRecordId, sizeof(int), 1, f);
    
    // Lê cada campo
    for (int i = 0; i < form->numFields; i++) {
        Field *field = malloc(sizeof(Field));
        
        fread(&field->id, sizeof(int), 1, f);
        fread(field->name, sizeof(char), 100, f);
        fread(field->label, sizeof(char), 100, f);
        fread(field->description, sizeof(char), 200, f);
        fread(&field->type, sizeof(FieldType), 1, f);
        fread(&field->validation, sizeof(FieldValidation), 1, f);
        fread(field->defaultValue, sizeof(char), 200, f);
        fread(&field->calculated, sizeof(bool), 1, f);
        fread(field->formula, sizeof(char), 300, f);
        
        // Lê choices (se houver)
        bool hasChoices;
        fread(&hasChoices, sizeof(bool), 1, f);
        
        if (hasChoices) {
            field->choices = malloc(sizeof(ChoiceOptions));
            fread(&field->choices->numOptions, sizeof(int), 1, f);
            
            field->choices->options = malloc(field->choices->numOptions * sizeof(char*));
            for (int j = 0; j < field->choices->numOptions; j++) {
                int len;
                fread(&len, sizeof(int), 1, f);
                field->choices->options[j] = malloc(len);
                fread(field->choices->options[j], sizeof(char), len, f);
            }
        } else {
            field->choices = NULL;
        }
        
        form->fields[i] = field;
    }
    
    // Inicializa campos restantes
    for (int i = form->numFields; i < MAX_FIELDS; i++) {
        form->fields[i] = NULL;
    }
    
    fclose(f);
    return form;
}