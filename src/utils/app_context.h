#pragma once

typedef struct {
    char base[512];
    char data[512];
    char forms[512];
    char records[512];
    char exports[512];
    char backups[512];
    char templates[512];
} AppContext;

extern AppContext APP;

int inicializar_app_context(void);
