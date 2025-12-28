#include "ui_utils.h"
#include "colors.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <sys/ioctl.h>
    #include <unistd.h>
    #include <termios.h>
#endif

// --- Helpers para Raw Mode (Linux/Mac) ---
#ifndef _WIN32
static struct termios orig_termios;
static bool raw_mode_active = false;

static void disable_raw_mode() {
    if (raw_mode_active) {
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
        raw_mode_active = false;
    }
}

static void enable_raw_mode() {
    if (!raw_mode_active) {
        tcgetattr(STDIN_FILENO, &orig_termios);
        struct termios raw = orig_termios;
        raw.c_lflag &= ~(ECHO | ICANON);
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
        raw_mode_active = true;
    }
}
#endif

void limpar_tela() {
    #ifdef _WIN32
        system("cls");
    #else
        printf("\033[H\033[2J\033[3J"); // Move topo, limpa tela e buffer de rolagem
        fflush(stdout);
    #endif
}

void limpar_buffer() {
    int c;
    while (1) {
        c = getchar();
        if (c == EOF && errno == EINTR) continue; // Ignora interrupção de resize
        if (c == '\n' || c == EOF) break;
    }
}

bool ler_string_segura(char *buffer, size_t tamanho) {
    while (1) {
        if (fgets(buffer, tamanho, stdin)) {
            buffer[strcspn(buffer, "\n")] = '\0';
            return true;
        }
        // Se foi interrompido por um sinal (resize), tenta de novo
        if (errno == EINTR) continue;
        return false;
    }
}

bool ler_int_seguro(int *valor) {
    int result;
    while (1) {
        result = scanf("%d", valor);
        if (result == EOF && errno == EINTR) continue;
        return result == 1;
    }
}

char ler_confirmacao() {
    char buffer[100];
    if (!ler_string_segura(buffer, sizeof(buffer))) return '\0';
    if (strlen(buffer) > 0) return buffer[0];
    return '\0'; // Enter vazio
}

int obter_largura_terminal() {
    #ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        int columns;
        GetStdHandle(STD_OUTPUT_HANDLE, &csbi);
        columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        return columns;
    #else
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
        return 80;
    }
    return w.ws_col;
    #endif
}

void desenhar_cabecalho(const char *titulo) {
    int largura = obter_largura_terminal();
    int len_titulo = strlen(titulo);
    
    // Garante que o título cabe (com margem de 2 caracteres para as bordas)
    if (len_titulo > largura - 4) {
        len_titulo = largura - 4;
    }

    int espacos_total = largura - 2 - len_titulo;
    int pad_esq = espacos_total / 2;
    int pad_dir = espacos_total - pad_esq;

    printf(BOLD_CYAN);
    
    // Linha superior
    printf("╔");
    for (int i = 0; i < largura - 2; i++) printf("═");
    printf("╗\n");

    // Linha do meio com título centralizado
    printf("║%*s%s%*s║\n", pad_esq, "", titulo, pad_dir, "");

    // Linha inferior
    printf("╚");
    for (int i = 0; i < largura - 2; i++) printf("═");
    printf("╝\n" RESET);
}

void desenhar_separador() {
    int largura = obter_largura_terminal();
    printf(CYAN);
    for (int i = 0; i < largura - 1; i++) printf("─");
    printf(" \n" RESET);
}

// --- Implementação dos Diálogos ---

bool ler_texto_dialogo(const char *titulo, const char *texto, char *buffer, size_t tamanho) {
#ifdef _WIN32
    while(1) {
        limpar_tela();
        desenhar_cabecalho(titulo);
        if (texto) printf("%s", texto);
        
        if (fgets(buffer, tamanho, stdin)) {
            buffer[strcspn(buffer, "\n")] = '\0';
            return true;
        }
        // Se foi interrompido por resize, o loop roda de novo e redesenha tudo
        if (errno == EINTR) continue;
        return false;
    }
#else
    size_t pos = 0;
    buffer[0] = '\0';
    
    limpar_tela();
    desenhar_cabecalho(titulo);
    if (texto) printf("%s", texto);
    fflush(stdout);

    enable_raw_mode();
    
    while (1) {
        char c;
        ssize_t nread = read(STDIN_FILENO, &c, 1);
        
        if (nread == -1) {
            if (errno == EINTR) {
                // Resize detected: redesenha tudo e o buffer atual
                limpar_tela();
                desenhar_cabecalho(titulo);
                if (texto) printf("%s", texto);
                printf("%s", buffer);
                fflush(stdout);
                continue;
            }
            disable_raw_mode();
            return false;
        }
        
        if (nread == 0) { // EOF
            disable_raw_mode();
            return false;
        }
        
        if (c == '\n' || c == '\r') {
            buffer[pos] = '\0';
            disable_raw_mode();
            printf("\n");
            return true;
        }
        
        if (c == 127 || c == '\b') { // Backspace
            if (pos > 0) {
                pos--;
                buffer[pos] = '\0';
                printf("\b \b");
                fflush(stdout);
            }
        } else if (c >= 32 && c < 127) { // Caracteres imprimíveis
            if (pos < tamanho - 1) {
                buffer[pos++] = c;
                buffer[pos] = '\0';
                putchar(c);
                fflush(stdout);
            }
        }
    }
#endif
}

bool ler_int_dialogo(const char *titulo, const char *texto, int *valor) {
    char buffer[100];
    if (!ler_texto_dialogo(titulo, texto, buffer, sizeof(buffer))) return false;
    
    char *endptr;
    long val = strtol(buffer, &endptr, 10);
    
    if (buffer[0] == '\0' || *endptr != '\0') return false;
    
    *valor = (int)val;
    return true;
}

char ler_confirmacao_dialogo(const char *titulo, const char *texto) {
    char buffer[100];
    // Reutiliza a lógica do texto diálogo
    if (!ler_texto_dialogo(titulo, texto, buffer, sizeof(buffer))) return '\0';
    if (strlen(buffer) > 0) return buffer[0];
    return '\0';
}

bool esperar_enter_check_resize() {
    int c;
    while (1) {
        c = getchar();
        if (c == EOF && errno == EINTR) return true; // Resize detectado
        if (c == '\n' || c == EOF) return false; // Enter pressionado
    }
}