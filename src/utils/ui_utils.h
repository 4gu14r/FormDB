#ifndef UI_UTILS_H
#define UI_UTILS_H
#include <stdbool.h>
#include <stddef.h>

// Obtém a largura atual do terminal
int obter_largura_terminal();

// Desenha um cabeçalho que se adapta à largura da tela com título centralizado
void desenhar_cabecalho(const char *titulo);

// Desenha uma linha separadora horizontal que se adapta à largura da tela
void desenhar_separador();

// Limpa a tela de forma multiplataforma
void limpar_tela();

// Limpa o buffer de entrada (stdin)
void limpar_buffer();

// Lê uma string de forma segura (ignora interrupções de resize)
bool ler_string_segura(char *buffer, size_t tamanho);

// Lê um inteiro de forma segura (ignora interrupções de resize)
bool ler_int_seguro(int *valor);

// Lê um caractere de confirmação (primeira letra ou \0 se vazio) de forma segura
char ler_confirmacao();

// --- Funções de Diálogo (Responsivas) ---
// Limpam a tela, desenham cabeçalho, mostram texto e leem input tratando resize

bool ler_texto_dialogo(const char *titulo, const char *texto, char *buffer, size_t tamanho);

bool ler_int_dialogo(const char *titulo, const char *texto, int *valor);

char ler_confirmacao_dialogo(const char *titulo, const char *texto);

// Espera o usuário pressionar ENTER, mas retorna true se houver resize (para redesenhar)
bool esperar_enter_check_resize();

#endif