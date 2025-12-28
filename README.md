# 📋 FormDB - Sistema Universal de Formulários e Banco de Dados

> Crie formulários customizados para **qualquer coisa** e gerencie seus dados de forma rápida e eficiente!

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Language](https://img.shields.io/badge/language-C-orange.svg)
![Status](https://img.shields.io/badge/status-Em%20Desenvolvimento-yellow.svg)

---

## 🎯 O Que É?

FormDB é um sistema **offline** e **gratuito** que permite criar formulários personalizados para cadastrar QUALQUER tipo de dado, sem precisar programar!

**Imagine:** Você tem uma loja, clínica, academia, restaurante... Em vez de usar planilhas confusas ou sistemas caros, você cria seu próprio formulário em minutos!

### ✨ Principais Características

- 📝 **Crie formulários customizados** sem programar
- 🎨 **18 tipos de campos** diferentes (texto, número, data, email, CPF, etc)
- ✅ **Validações automáticas** (CPF válido, email válido, etc)
- 📤 **Exporte em múltiplos formatos** (CSV, JSON, Excel, PDF)
- 📥 **Importe dados** de planilhas existentes
- 🔍 **Busca ultra-rápida** com filtros avançados
- 📊 **Relatórios e estatísticas** automáticos
- 💾 **Backup automático** dos seus dados
- 🚀 **Rápido e leve** (feito em C!)
- 🔒 **Privacidade total** (offline, seus dados não vão pra nuvem)

---

## 🚀 Instalação Rápida

### Requisitos
- GCC (compilador C)
- Make
- Linux, macOS ou Windows (com MinGW)

### Compilar e Executar

```bash
# Clone ou baixe o projeto
cd formdb

# Compile
make

# Execute
make run
```

Pronto! 🎉

---

## 📖 Como Usar?

### 1️⃣ Criar um Formulário

```
Menu → 1. Criar Novo Formulário

Digite:
- Nome: "produtos"
- Nome exibido: "Cadastro de Produtos"

Adicione campos:
1. Código (Texto Curto)
2. Nome (Texto Curto)
3. Preço (Dinheiro)
4. Estoque (Número)
5. Categoria (Lista única)
6. Ativo (Sim/Não)

Salvar! ✓
```

### 2️⃣ Cadastrar Dados

```
Menu → 3. Cadastrar Dados

Código:     [P001]
Nome:       [Camisa Polo]
Preço:      [R$ 89,90]
Estoque:    [150]
Categoria:  [▼ Roupas]
Ativo:      (●) Sim

Salvar! ✓
```

### 3️⃣ Exportar

```
Menu → 6. Exportar Dados

Escolha formato: CSV
Local: exports/produtos.csv

✓ 1.247 registros exportados!
```

---

## 🎨 Tipos de Campos Disponíveis

| Tipo | Descrição | Exemplo |
|------|-----------|---------|
| 📝 Texto Curto | Até 100 caracteres | "João Silva" |
| 📄 Texto Longo | Até 1000 caracteres | Descrições longas |
| 🔢 Número | Apenas números inteiros | 150 |
| 💰 Dinheiro | Valor monetário | R$ 1.234,56 |
| 📅 Data | Formato dd/mm/aaaa | 15/03/2025 |
| ⏰ Hora | Formato HH:MM | 14:30 |
| 📧 Email | Com validação | joao@email.com |
| 📱 Telefone | Formatação automática | (11) 98765-4321 |
| 🆔 CPF | Validação de dígitos | 123.456.789-00 |
| 🏢 CNPJ | Validação de dígitos | 12.345.678/0001-00 |
| 📍 CEP | Auto-completa endereço | 01310-100 |
| 🌐 URL | Links de sites | https://site.com |
| ✅ Sim/Não | Verdadeiro ou falso | Sim |
| 📋 Lista (única) | Escolhe 1 opção | Dropdown |
| ☑️ Lista (múltipla) | Escolhe várias | Checkboxes |
| ⭐ Avaliação | 1 a 5 estrelas | ★★★★☆ |

---

## 💼 Casos de Uso

### 🏪 Comércio
- Controle de estoque/produtos
- Cadastro de clientes
- Pedidos e vendas
- Fornecedores

### 🏥 Saúde
- Pacientes
- Consultas
- Prontuários
- Exames

### 🎓 Educação
- Alunos
- Professores
- Notas e frequência
- Cursos

### 🏢 Empresas
- Funcionários (RH)
- Projetos
- Tarefas
- Contratos

### 👤 Pessoal
- Contatos/Agenda
- Finanças pessoais
- Tarefas (To-Do)
- Coleções (livros, filmes)

---

## 📁 Estrutura de Arquivos

```
formdb/
├── bin/              # Executável compilado
├── src/              # Código fonte
│   ├── core/        # Motor principal
│   ├── ui/          # Interface
│   ├── storage/     # Persistência
│   └── utils/       # Utilitários
├── data/            # Seus dados
│   ├── forms/       # Formulários criados
│   └── records/     # Registros cadastrados
├── templates/       # Templates prontos
├── exports/         # Arquivos exportados
├── backups/         # Backups automáticos
└── makefile         # Build system
```

---

## 🛠️ Comandos Make

```bash
make          # Compila o projeto
make run      # Compila e executa
make clean    # Remove arquivos compilados
make cleanall # Remove tudo (incluindo dados!)
make info     # Informações do projeto
make help     # Lista de comandos
```

---

## 🔥 Diferenciais

| Característica | FormDB | Airtable | Google Forms |
|----------------|--------|----------|--------------|
| **Custo** | 💚 Gratuito | 💰 R$ 200+/mês | 💚 Gratuito |
| **Offline** | ✅ Sim | ❌ Não | ❌ Não |
| **Privacidade** | ✅ Total | ⚠️ Nuvem | ⚠️ Google |
| **Velocidade** | ⚡ C (nativo) | 🐌 Web | 🐌 Web |
| **Customização** | ✅ Total | ⚠️ Limitada | ⚠️ Limitada |
| **Código Aberto** | ✅ Sim | ❌ Não | ❌ Não |

---

## 📊 Roadmap

### ✅ Fase 1 - Core (Atual)
- [x] Criar formulários customizados
- [x] 18 tipos de campos
- [x] Validações básicas
- [ ] Entrada de dados dinâmica
- [ ] Salvamento CSV

### 🚧 Fase 2 - Storage
- [ ] Exportar JSON
- [ ] Importar CSV/JSON
- [ ] Formato binário (performance)
- [ ] Backup automático

### 📅 Fase 3 - Features
- [ ] Sistema de busca avançado
- [ ] Filtros múltiplos
- [ ] Ordenação dinâmica
- [ ] Relacionamentos entre formulários
- [ ] Campos calculados

### 🎨 Fase 4 - Reports
- [ ] Relatórios customizáveis
- [ ] Estatísticas automáticas
- [ ] Gráficos ASCII
- [ ] Exportar PDF

### 🚀 Fase 5 - Avançado
- [ ] Templates prontos (produtos, clientes, etc)
- [ ] Sistema de login (opcional)
- [ ] Auditoria (quem fez o quê)
- [ ] API REST (opcional)

---

## 🤝 Contribuindo

Contribuições são bem-vindas! Sinta-se à vontade para:

1. 🐛 Reportar bugs
2. 💡 Sugerir funcionalidades
3. 🔧 Enviar pull requests
4. 📖 Melhorar documentação

---

## 📝 Licença

MIT License - Você pode usar, modificar e distribuir livremente!

---

## 💬 Contato

Dúvidas ou sugestões? Abra uma issue no GitHub!

---

## 🌟 Por Que FormDB?

> "Cansado de sistemas caros, lentos e que mandam seus dados pra nuvem sem permissão? O FormDB é a solução: rápido, seguro, gratuito e SEU!"

### Comparação Rápida

**Antes (Excel/Planilhas):**
- ❌ Difícil de validar dados
- ❌ Erros de digitação
- ❌ Sem relacionamentos
- ❌ Dificuldade pra gerar relatórios

**Depois (FormDB):**
- ✅ Validações automáticas
- ✅ Interface guiada
- ✅ Relacionamentos entre dados
- ✅ Relatórios com 1 clique

---

<div align="center">

## ⭐ Gostou? Dê uma estrela no GitHub!

**FormDB** - Simplicidade, Velocidade e Privacidade

</div>