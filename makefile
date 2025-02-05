CC = gcc
CFLAGS = -Wall -I./src/lib
BIN_DIR = bin
SRC_DIR = src

all: criar_diretorios programa

criar_diretorios:
	mkdir -p $(BIN_DIR)

programa: $(BIN_DIR)/main.o $(BIN_DIR)/pessoas.o $(BIN_DIR)/cadastro.o $(BIN_DIR)/leitura.o
	$(CC) $^ -o $(BIN_DIR)/programa

$(BIN_DIR)/main.o: $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/pessoas.o: $(SRC_DIR)/lib/pessoas.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/cadastro.o: $(SRC_DIR)/cadastro/cadastro.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/leitura.o: $(SRC_DIR)/leitura/leitura.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BIN_DIR)

.PHONY: all clean criar_diretorios