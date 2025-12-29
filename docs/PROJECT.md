# 📋 FormDB - Estrutura do Projeto

```
formdb/
├── src/
│   ├── main.c                    # Ponto de entrada
│   ├── core/
│   │   ├── form.h               # Estruturas de formulário
│   │   ├── form.c               # Gerenciador de formulários
│   │   ├── field.h              # Estruturas de campos
│   │   ├── field.c              # Tipos de campos
│   │   ├── record.h             # Estrutura de registro
│   │   └── record.c             # Gerenciador de registros
│   ├── ui/
│   │   ├── menu.h               # Menus do sistema
│   │   ├── menu.c
│   │   ├── form_builder.h       # Construtor visual de forms
│   │   ├── form_builder.c
│   │   ├── data_entry.h         # Entrada de dados
│   │   └── data_entry.c
│   ├── storage/
│   │   ├── csv_handler.h        # Exportar/Importar CSV
│   │   ├── csv_handler.c
│   │   ├── json_handler.h       # Exportar/Importar JSON
│   │   ├── json_handler.c
│   │   └── binary_handler.h     # Formato binário nativo
│   │       binary_handler.c
│   ├── search/
│   │   ├── search.h             # Motor de busca
│   │   └── search.c
│   ├── validation/
│   │   ├── validators.h         # Validadores de campos
│   │   └── validators.c
│   ├── reports/
│   │   ├── report_gen.h         # Gerador de relatórios
│   │   └── report_gen.c
│   └── utils/
│       ├── colors.h             # Cores ANSI
│       ├── string_utils.h       # Utilidades string
│       ├── string_utils.c
│       └── input_utils.h        # Utilidades input
│           input_utils.c
├── data/
│   ├── forms/                   # Definições de formulários
│   └── records/                 # Dados cadastrados
├── templates/                   # Templates prontos
│   ├── produtos.json
│   ├── clientes.json
│   ├── vendas.json
│   └── ...
├── exports/                     # Arquivos exportados
├── backups/                     # Backups automáticos
├── makefile
└── README.md
```

## 🎯 Ordem de Implementação

### Fase 1 - Core (Agora!)
1. ✅ Estruturas de dados (form.h, field.h, record.h)
2. ✅ Criador de formulários (form_builder)
3. ✅ Entrada de dados dinâmica (data_entry)
4. ✅ Salvamento CSV básico

### Fase 2 - Storage
5. JSON handler
6. Importação inteligente
7. Binary format (performance)

### Fase 3 - Features
8. Sistema de busca
9. Validações avançadas
10. Relatórios

### Fase 4 - Polish
11. Interface melhorada
12. Templates prontos
13. Backup automático
