#ifndef DATA_ENTRY_H
#define DATA_ENTRY_H

#include "../core/form.h"
#include "../core/record.h"

// Função principal para cadastrar novo registro interativamente
bool cadastrar_registro_interativo(Form *form, RecordSet *recordset);

// Visualizar todos os registros de forma formatada
void visualizar_registros(RecordSet *recordset);

// Menu completo de cadastro e visualização
void menu_cadastro(Form *form, RecordSet *recordset);

#endif