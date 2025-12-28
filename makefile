CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I./src
BIN_DIR = bin
SRC_DIR = src
OBJ_DIR = $(BIN_DIR)/obj

# Cores para output
RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[0;33m
CYAN = \033[0;36m
NC = \033[0m # No Color

# Arquivos fonte
CORE_SRC = $(SRC_DIR)/core/field.c $(SRC_DIR)/core/form.c $(SRC_DIR)/core/record.c
UI_SRC = $(SRC_DIR)/ui/form_builder.c $(SRC_DIR)/ui/data_entry.c $(SRC_DIR)/ui/form_browser.c
STORAGE_SRC = $(SRC_DIR)/storage/csv_handler.c
UTILS_SRC = $(SRC_DIR)/utils/string_utils.c $(SRC_DIR)/utils/input_utils.c
MAIN_SRC = $(SRC_DIR)/main.c

ALL_SRC = $(CORE_SRC) $(UI_SRC) $(STORAGE_SRC) $(UTILS_SRC) $(MAIN_SRC)

# Arquivos objeto
CORE_OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(CORE_SRC))
UI_OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(UI_SRC))
STORAGE_OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(STORAGE_SRC))
UTILS_OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(UTILS_SRC))
MAIN_OBJ = $(OBJ_DIR)/main.o

ALL_OBJ = $(CORE_OBJ) $(UI_OBJ) $(STORAGE_OBJ) $(UTILS_OBJ) $(MAIN_OBJ)

# Executável
TARGET = $(BIN_DIR)/formdb

.PHONY: all clean run help dirs info

all: dirs $(TARGET)
	@echo "$(GREEN)✓ Compilação concluída com sucesso!$(NC)"
	@echo "$(CYAN)Execute com: make run$(NC)"

# Criar diretórios necessários
dirs:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(OBJ_DIR)/core
	@mkdir -p $(OBJ_DIR)/ui
	@mkdir -p $(OBJ_DIR)/storage
	@mkdir -p $(OBJ_DIR)/utils
	@mkdir -p data/forms
	@mkdir -p data/records
	@mkdir -p templates
	@mkdir -p exports
	@mkdir -p backups

# Linkar executável
$(TARGET): $(ALL_OBJ)
	@echo "$(YELLOW)→ Linkando executável...$(NC)"
	@$(CC) $(CFLAGS) $^ -o $@

# Compilar arquivos core
$(OBJ_DIR)/core/%.o: $(SRC_DIR)/core/%.c
	@echo "$(CYAN)→ Compilando $<$(NC)"
	@$(CC) $(CFLAGS) -c $< -o $@

# Compilar arquivos ui
$(OBJ_DIR)/ui/%.o: $(SRC_DIR)/ui/%.c
	@echo "$(CYAN)→ Compilando $<$(NC)"
	@$(CC) $(CFLAGS) -c $< -o $@

# Compilar arquivos storage
$(OBJ_DIR)/storage/%.o: $(SRC_DIR)/storage/%.c
	@echo "$(CYAN)→ Compilando $<$(NC)"
	@$(CC) $(CFLAGS) -c $< -o $@

# Compilar arquivos utils
$(OBJ_DIR)/utils/%.o: $(SRC_DIR)/utils/%.c
	@echo "$(CYAN)→ Compilando $<$(NC)"
	@$(CC) $(CFLAGS) -c $< -o $@

# Compilar main
$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c
	@echo "$(CYAN)→ Compilando $<$(NC)"
	@$(CC) $(CFLAGS) -c $< -o $@

# Executar programa
run: all
	@echo "$(GREEN)→ Executando FormDB...$(NC)"
	@./$(TARGET)

# Limpar arquivos compilados
clean:
	@echo "$(RED)→ Limpando arquivos compilados...$(NC)"
	@rm -rf $(BIN_DIR)
	@echo "$(GREEN)✓ Limpeza concluída!$(NC)"

# Limpar tudo (incluindo dados)
cleanall: clean
	@echo "$(RED)→ Removendo dados...$(NC)"
	@rm -rf data exports backups
	@echo "$(GREEN)✓ Limpeza total concluída!$(NC)"

# Informações do projeto
info:
	@echo "$(CYAN)╔════════════════════════════════════════╗$(NC)"
	@echo "$(CYAN)║         FormDB - Informações          ║$(NC)"
	@echo "$(CYAN)╚════════════════════════════════════════╝$(NC)"
	@echo ""
	@echo "$(GREEN)Compilador:$(NC) $(CC)"
	@echo "$(GREEN)Flags:$(NC) $(CFLAGS)"
	@echo "$(GREEN)Executável:$(NC) $(TARGET)"
	@echo ""
	@echo "$(YELLOW)Módulos:$(NC)"
	@echo "  • Core (field, form, record)"
	@echo "  • UI (form_builder, data_entry)"
	@echo "  • Storage (csv_handler)"
	@echo "  • Utils (string_utils, input_utils)"
	@echo ""
	@echo "$(YELLOW)Diretórios:$(NC)"
	@echo "  • data/forms/   → Definições de formulários"
	@echo "  • data/records/ → Dados cadastrados"
	@echo "  • templates/    → Templates prontos"
	@echo "  • exports/      → Arquivos exportados"
	@echo "  • backups/      → Backups automáticos"

# Ajuda
help:
	@echo "$(CYAN)╔════════════════════════════════════════╗$(NC)"
	@echo "$(CYAN)║         FormDB - Comandos              ║$(NC)"
	@echo "$(CYAN)╚════════════════════════════════════════╝$(NC)"
	@echo ""
	@echo "$(GREEN)make$(NC)          - Compila o projeto"
	@echo "$(GREEN)make run$(NC)      - Compila e executa"
	@echo "$(GREEN)make clean$(NC)    - Remove arquivos compilados"
	@echo "$(GREEN)make cleanall$(NC) - Remove tudo (incluindo dados)"
	@echo "$(GREEN)make info$(NC)     - Mostra informações do projeto"
	@echo "$(GREEN)make help$(NC)     - Mostra esta ajuda"
	@echo ""
	@echo "$(YELLOW)Primeira execução:$(NC)"
	@echo "  1. make"
	@echo "  2. make run"
	@echo ""