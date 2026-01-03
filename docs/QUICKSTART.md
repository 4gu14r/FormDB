# 🚀 FormDB - Guia de Início Rápido

Este guia mostra como ter o FormDB funcionando em menos de 5 minutos. Escolha a opção que melhor se adapta a você.

---

## Opção 1: Baixar a Versão Pronta (Recomendado)

Esta é a maneira mais fácil e rápida de começar a usar o FormDB.

### Passo 1: Acesse a Página de Releases

Vá para a nossa página de releases no GitHub para encontrar a versão mais recente.

➡️ **[Acessar Releases do FormDB](https://github.com/form-db/FormDB/releases)**

### Passo 2: Baixe o Arquivo para o seu Sistema

- **Para Windows:** Baixe o arquivo `formdb-installer.exe`.
- **Para Linux ou macOS:** Baixe o executável com o nome `formdb`.

### Passo 3: Execute

- **Windows:** Dê um duplo clique no instalador e siga as instruções. Após instalar, você terá um ícone para iniciar o FormDB.
- **Linux/macOS:** Abra um terminal, dê permissão de execução ao arquivo (`chmod +x formdb`) e execute-o (`./formdb`).

**Pronto!** O FormDB já está funcionando.

---

## Opção 2: Compilar Manualmente (Para Desenvolvedores)

Se você deseja modificar o código ou contribuir com o projeto, siga estes passos.

### Passo 1: Pré-requisitos

Você vai precisar do `Git` e de um compilador C (`GCC`).

- **Linux (Debian/Ubuntu):**
  ```bash
  sudo apt-get update && sudo apt-get install build-essential
  ```
- **macOS:**
  ```bash
  xcode-select --install
  ```
- **Windows:**
  Recomendamos o uso do [WSL (Subsistema do Windows para Linux)](https://learn.microsoft.com/pt-br/windows/wsl/install) e seguir os passos do Linux.

### Passo 2: Clone o Repositório

```bash
git clone https://github.com/form-db/FormDB.git
cd FormDB
```

### Passo 3: Compile e Execute

```bash
# Para compilar o programa
make

# Para executar o programa
make run
```

**Pronto!** Você está executando a versão mais recente diretamente do código-fonte.

---

## ✨ Seu Primeiro Formulário em 3 Minutos

Agora que o FormDB está rodando, veja como é fácil criar um sistema de cadastro.

#### 1. Crie o Formulário

- No menu principal, escolha a opção `1. Criar Novo Formulário`.
- Dê um nome, como `produtos`.

#### 2. Adicione Campos

Adicione alguns campos simples. O programa irá te guiar:
- `nome` (Texto Curto)
- `preco` (Dinheiro)
- `estoque` (Número)

Quando terminar, digite `s` para salvar.

#### 3. Cadastre um Item

- No menu, vá em `3. Cadastrar Dados` e escolha o formulário `produtos`.
- Preencha as informações do seu primeiro produto:
  - **Nome:** `Camisa Azul`
  - **Preço:** `79.90`
  - **Estoque:** `50`
- Salve o registro.

#### 4. Exporte seus Dados

- Volte ao menu e escolha `6. Exportar Dados`.
- Selecione o formulário `produtos` e o formato `CSV`.
- O arquivo `exports/produtos.csv` foi criado! Você pode abri-lo no Excel, Google Sheets ou qualquer editor de planilhas.

---

## 🤔 E Depois?

Você acabou de criar um formulário, cadastrar e exportar dados!

- Para ver **exemplos mais complexos** (como controle de clientes, agenda, etc.), confira o nosso **[Guia de Exemplos](./EXAMPLES.md)**.
- Para ver a **lista completa de funcionalidades**, veja o **[Status do Projeto](./STATUS.md)**.
