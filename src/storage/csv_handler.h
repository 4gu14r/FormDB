#ifndef CSV_HANDLER_H
#define CSV_HANDLER_H

#include "../core/record.h"
#include <stdbool.h>

bool salvar_registros_csv(RecordSet *recordset, const char *filepath);
bool exportar_excel(RecordSet *recordset, const char *filepath);
RecordSet* carregar_registros_csv(Form *form, const char *filepath);

#endif