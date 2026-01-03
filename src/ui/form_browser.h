/* /workspaces/RegistroPessoas/src/ui/form_browser.h */
#ifndef FORM_BROWSER_H
#define FORM_BROWSER_H

#include "../core/form.h"

// Lista os formulários disponíveis na pasta data/forms
void listar_formularios();

// Interage com o usuário para selecionar e carregar um formulário existente
// Retorna o ponteiro para o Form carregado ou NULL se cancelar/erro
Form* selecionar_formulario_interativo();

#endif
