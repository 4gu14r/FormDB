#ifndef JSON_HANDLER_H
#define JSON_HANDLER_H

#include "../core/record.h"
#include "../core/form.h"

bool exportar_json(RecordSet *recordset, const char *filepath);
Form* importar_formulario_json(const char *filepath);

#endif