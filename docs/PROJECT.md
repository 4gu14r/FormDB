# 📋 FormDB - Estrutura do Projeto

Este documento descreve a organização do código-fonte e a arquitetura do FormDB, servindo como um guia para desenvolvedores.

---

## 📂 Estrutura de Diretórios (Simplificada)

A estrutura abaixo foca nos arquivos-chave do projeto, especialmente dentro do diretório `src`.

```
formdb/
├───.gitignore
├───makefile
├───README.md
├───docs/
│   ├───COMPARISON.md
│   ├───EXAMPLES.md
│   ├───PROJECT.md      <-- Você está aqui
│   ├───QUICKSTART.md
│   └───STATUS.md
└───src/
    ├───main.c              # Ponto de entrada da aplicação e menu principal
    ├───core/               # Lógica de negócio principal
    │   ├───field.c/.h
    │   ├───form.c/.h
    │   └───record.c/.h
    ├───reports/            # (Placeholder) Módulo para futuros relatórios
    │   └───report_gen.c/.h
    ├───search/             # Funcionalidades de busca e filtro
    │   └───search.c/.h
    ├───storage/            # Manipuladores de arquivos
    │   ├───csv_handler.c/.h
    │   └───json_handler.c/.h
    ├───ui/                 # Componentes de interface de usuário
    │   ├───data_entry.c/.h
    │   ├───exporter.c/.h
    │   ├───form_browser.c/.h
    │   ├───form_builder.c/.h
    │   ├───form_manager.c/.h
    │   ├───importer.c/.h
    │   └───menu.c/.h
    ├───utils/              # Funções utilitárias
    │   ├───app_context.c/.h
    │   ├───file_utils.c/.h
    │   ├───input_utils.c/.h
    │   ├───record_utils.c/.h
    │   ├───string_utils.c/.h
    │   └───ui_utils.c/.h
    └───validation/         # Rotinas de validação de dados
        └───validator.c/.h
```

---

## 🏛️ Arquitetura e Módulos

O código é organizado em módulos com responsabilidades bem definidas:

- **`main.c`**: Orquestra a aplicação, inicializa os componentes e gerencia o loop do menu principal.

- **`core`**: O coração do FormDB. Define as estruturas de dados fundamentais (`Form`, `Field`, `Record`) e a lógica para manipulá-las, independente da interface ou do formato de armazenamento.

- **`ui`**: Responsável por toda a interação com o usuário. Cada arquivo gerencia uma tela ou um componente específico da interface de linha de comando (CLI), como o construtor de formulários, a tela de entrada de dados, etc.

- **`storage`**: Abstrai as operações de leitura e escrita. Contém os "drivers" para diferentes formatos de arquivo, como `JSON` e `CSV`. Se um novo formato for adicionado (ex: XML), ele será implementado aqui.

- **`search`**: Contém a lógica para realizar buscas e aplicar filtros sobre os registros carregados em memória.

- **`validation`**: Centraliza as funções que validam dados específicos, como CPF, CNPJ, e-mail, etc. É utilizado tanto na entrada de dados quanto na definição dos formulários.

- **`utils`**: Uma coleção de funções de ajuda usadas em várias partes do sistema, como manipulação de strings, leitura de entrada do usuário e formatação da interface.

- **`reports`**: Atualmente, este módulo é um **placeholder**. A estrutura de arquivos existe para facilitar a implementação de funcionalidades de relatórios e estatísticas no futuro.

---

## 🚀 Roadmap de Desenvolvimento

O plano de implementação inicial foi concluído, e o projeto está em uma versão estável.

Para consultar as funcionalidades que estão planejadas para o futuro ou em consideração, veja o documento de status do projeto.

➡️ **[Consultar Status e Roadmap Futuro](./STATUS.md)**