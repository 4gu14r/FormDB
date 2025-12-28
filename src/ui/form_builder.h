#ifndef FORM_BUILDER_H
#define FORM_BUILDER_H

#include "../core/form.h"
#include "../core/field.h"

// Constrói um formulário completo de forma interativa
Form* construir_formulario_interativo();

// Constrói um campo interativamente
Field* construir_campo_interativo();

// Seleciona o tipo de campo
FieldType selecionar_tipo_campo();

// Configura validações de um campo
void configurar_validacao(Field *field);

// Adiciona opções para campos de escolha
void adicionar_opcoes_escolha(Field *field);

#endif