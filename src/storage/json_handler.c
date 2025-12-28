#include "../core/record.h"
#include <stdio.h>
#include <string.h>
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