#include "../core/record.h"
#include "../core/form.h"
#include "../core/field.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

static void escape_json(const char *input, char *output) {
    int j = 0;
    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] == '"') {
            output[j++] = '\\'; output[j++] = '"';
        } else if (input[i] == '\\') {
            output[j++] = '\\'; output[j++] = '\\';
        } else if (input[i] == '\n') {
            output[j++] = '\\'; output[j++] = 'n';
        } else {
            output[j++] = input[i];
        }
    }
    output[j] = '\0';
}

bool exportar_json(RecordSet *recordset, const char *filepath) {
    if (!recordset || !filepath) return false;
    
    FILE *f = fopen(filepath, "w");
    if (!f) return false;
    
    fprintf(f, "[\n");
    
    char buffer[2048];
    char timeStr[20];
    
    for (int i = 0; i < recordset->numRecords; i++) {
        Record *rec = recordset->records[i];
        struct tm *tm_info = localtime(&rec->createdAt);
        strftime(timeStr, 20, "%Y-%m-%d %H:%M:%S", tm_info);
        
        fprintf(f, "  {\n");
        fprintf(f, "    \"id\": %d,\n", rec->id);
        fprintf(f, "    \"created_at\": \"%s\",\n", timeStr);
        
        for (int j = 0; j < recordset->form->numFields; j++) {
            Field *field = recordset->form->fields[j];
            const char *val = obter_valor_campo(rec, field->id);
            
            escape_json(val, buffer);
            
            fprintf(f, "    \"%s\": \"%s\"", field->name, buffer);
            if (j < recordset->form->numFields - 1) {
                fprintf(f, ",");
            }
            fprintf(f, "\n");
        }
        
        fprintf(f, "  }%s\n", (i < recordset->numRecords - 1) ? "," : "");
    }
    
    fprintf(f, "]\n");
    fclose(f);
    return true;
}

// --- IMPORTAÇÃO (JSON PARSER SIMPLIFICADO) ---

typedef struct {
    const char *src;
    size_t pos;
} JParser;

static void skip_ws(JParser *p) {
    while (p->src[p->pos] && isspace((unsigned char)p->src[p->pos])) p->pos++;
}

static char peek(JParser *p) {
    skip_ws(p);
    return p->src[p->pos];
}

static char advance(JParser *p) {
    char c = p->src[p->pos];
    if (c) p->pos++;
    return c;
}

static bool match(JParser *p, char expected) {
    if (peek(p) == expected) {
        advance(p);
        return true;
    }
    return false;
}

static bool parse_string(JParser *p, char *out, size_t max_len) {
    if (!match(p, '"')) return false;
    size_t i = 0;
    while (p->src[p->pos] && p->src[p->pos] != '"') {
        char c = p->src[p->pos++];
        if (c == '\\' && p->src[p->pos]) {
            c = p->src[p->pos++]; // Ignora escape simples
        }
        if (i < max_len - 1) out[i++] = c;
    }
    out[i] = '\0';
    return match(p, '"');
}

static bool parse_primitive(JParser *p, char *out, size_t max_len) {
    skip_ws(p);
    size_t i = 0;
    while (p->src[p->pos] && (isalnum((unsigned char)p->src[p->pos]) || strchr(".-+", p->src[p->pos]))) {
        if (i < max_len - 1) out[i++] = p->src[p->pos];
        p->pos++;
    }
    out[i] = '\0';
    return i > 0;
}

static FieldType string_to_type(const char *str) {
    if (strcmp(str, "text_short") == 0) return FIELD_TEXT_SHORT;
    if (strcmp(str, "text_long") == 0) return FIELD_TEXT_LONG;
    if (strcmp(str, "number") == 0) return FIELD_NUMBER;
    if (strcmp(str, "decimal") == 0) return FIELD_DECIMAL;
    if (strcmp(str, "money") == 0) return FIELD_MONEY;
    if (strcmp(str, "date") == 0) return FIELD_DATE;
    if (strcmp(str, "time") == 0) return FIELD_TIME;
    if (strcmp(str, "datetime") == 0) return FIELD_DATETIME;
    if (strcmp(str, "email") == 0) return FIELD_EMAIL;
    if (strcmp(str, "phone") == 0) return FIELD_PHONE;
    if (strcmp(str, "cpf") == 0) return FIELD_CPF;
    if (strcmp(str, "cnpj") == 0) return FIELD_CNPJ;
    if (strcmp(str, "cep") == 0) return FIELD_CEP;
    if (strcmp(str, "url") == 0) return FIELD_URL;
    if (strcmp(str, "boolean") == 0) return FIELD_BOOLEAN;
    if (strcmp(str, "choice_single") == 0) return FIELD_CHOICE_SINGLE;
    if (strcmp(str, "choice_multiple") == 0) return FIELD_CHOICE_MULTIPLE;
    if (strcmp(str, "rating") == 0) return FIELD_RATING;
    return FIELD_TEXT_SHORT;
}

static void parse_validation(JParser *p, Field *f) {
    if (!match(p, '{')) return;
    while (peek(p) != '}') {
        char key[50];
        if (!parse_string(p, key, sizeof(key))) break;
        match(p, ':');
        
        char val[100];
        if (peek(p) == '"') parse_string(p, val, sizeof(val));
        else parse_primitive(p, val, sizeof(val));
        
        if (strcmp(key, "required") == 0) f->validation.required = (strcmp(val, "true") == 0);
        else if (strcmp(key, "unique") == 0) f->validation.unique = (strcmp(val, "true") == 0);
        else if (strcmp(key, "minLength") == 0) f->validation.minLength = atoi(val);
        else if (strcmp(key, "maxLength") == 0) f->validation.maxLength = atoi(val);
        else if (strcmp(key, "minValue") == 0) { f->validation.minValue = atoi(val); f->validation.hasMin = true; }
        else if (strcmp(key, "maxValue") == 0) { f->validation.maxValue = atoi(val); f->validation.hasMax = true; }
        
        if (peek(p) == ',') advance(p);
    }
    match(p, '}');
}

static void parse_options(JParser *p, Field *f) {
    if (!match(p, '[')) return;
    while (peek(p) != ']') {
        char opt[200];
        if (parse_string(p, opt, sizeof(opt))) {
            adicionar_opcao_escolha(f, opt);
        }
        if (peek(p) == ',') advance(p);
    }
    match(p, ']');
}

static Field* parse_field(JParser *p) {
    if (!match(p, '{')) return NULL;
    
    // Cria campo temporário
    Field *f = criar_campo("temp", "temp", FIELD_TEXT_SHORT);
    
    while (peek(p) != '}') {
        char key[50];
        if (!parse_string(p, key, sizeof(key))) break;
        match(p, ':');
        
        if (strcmp(key, "validation") == 0) {
            parse_validation(p, f);
        } else if (strcmp(key, "options") == 0) {
            parse_options(p, f);
        } else if (peek(p) == '[') { // Pula arrays desconhecidos
            match(p, '['); while(peek(p) != ']') advance(p); match(p, ']');
        } else if (peek(p) == '{') { // Pula objetos desconhecidos
            match(p, '{'); while(peek(p) != '}') advance(p); match(p, '}');
        } else {
            char val[300];
            if (peek(p) == '"') parse_string(p, val, sizeof(val));
            else parse_primitive(p, val, sizeof(val));
            
            if (strcmp(key, "name") == 0) strncpy(f->name, val, sizeof(f->name)-1);
            else if (strcmp(key, "label") == 0) strncpy(f->label, val, sizeof(f->label)-1);
            else if (strcmp(key, "description") == 0) strncpy(f->description, val, sizeof(f->description)-1);
            else if (strcmp(key, "type") == 0) f->type = string_to_type(val);
            else if (strcmp(key, "defaultValue") == 0) strncpy(f->defaultValue, val, sizeof(f->defaultValue)-1);
            else if (strcmp(key, "calculated") == 0) f->calculated = (strcmp(val, "true") == 0);
            else if (strcmp(key, "formula") == 0) strncpy(f->formula, val, sizeof(f->formula)-1);
        }
        
        if (peek(p) == ',') advance(p);
    }
    match(p, '}');
    return f;
}

static char* read_file(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) return NULL;
    fseek(f, 0, SEEK_END);
    long length = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *buffer = malloc(length + 1);
    if (buffer) {
        fread(buffer, 1, length, f);
        buffer[length] = '\0';
    }
    fclose(f);
    return buffer;
}

Form* importar_formulario_json(const char *filepath) {
    char *json = read_file(filepath);
    if (!json) return NULL;
    
    JParser p = {json, 0};
    Form *form = NULL;
    
    skip_ws(&p);
    if (match(&p, '{')) {
        while (peek(&p) != '}') {
            char key[50];
            if (!parse_string(&p, key, sizeof(key))) break;
            match(&p, ':');
            
            if (strcmp(key, "formInfo") == 0) {
                match(&p, '{');
                char name[100] = "", display[100] = "", desc[300] = "";
                while (peek(&p) != '}') {
                    char infoKey[50];
                    parse_string(&p, infoKey, sizeof(infoKey));
                    match(&p, ':');
                    char val[300];
                    parse_string(&p, val, sizeof(val));
                    
                    if (strcmp(infoKey, "name") == 0) strcpy(name, val);
                    else if (strcmp(infoKey, "displayName") == 0) strcpy(display, val);
                    else if (strcmp(infoKey, "description") == 0) strcpy(desc, val);
                    
                    if (peek(&p) == ',') advance(&p);
                }
                match(&p, '}');
                form = criar_formulario(name, display);
                if (form) strncpy(form->description, desc, sizeof(form->description)-1);
            } else if (strcmp(key, "fields") == 0 && form) {
                match(&p, '[');
                while (peek(&p) != ']') {
                    Field *f = parse_field(&p);
                    if (f) adicionar_campo_formulario(form, f);
                    if (peek(&p) == ',') advance(&p);
                }
                match(&p, ']');
            } else {
                // Pula desconhecidos
                if (peek(&p) == '{') { match(&p, '{'); while(peek(&p) != '}') advance(&p); match(&p, '}'); }
                else if (peek(&p) == '[') { match(&p, '['); while(peek(&p) != ']') advance(&p); match(&p, ']'); }
                else { char temp[100]; if(peek(&p)=='"') parse_string(&p,temp,sizeof(temp)); else parse_primitive(&p,temp,sizeof(temp)); }
            }
            
            if (peek(&p) == ',') advance(&p);
        }
    }
    
    free(json);
    return form;
}