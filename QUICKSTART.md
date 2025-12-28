# 🚀 FormDB - Guia de Início Rápido

## ⚡ 5 Minutos para Começar!

### 📦 Passo 1: Instalação

```bash
# Clone ou baixe o projeto
cd formdb

# Compile
make

# Pronto! ✓
```

---

## 🎯 Cenários Práticos

### 📦 Cenário 1: Controle de Estoque (Loja)

#### 1️⃣ Execute o programa
```bash
make run
```

#### 2️⃣ No menu, escolha: `1. Criar Novo Formulário`

#### 3️⃣ Preencha:
- **Nome interno:** `produtos`
- **Nome exibido:** `Controle de Estoque`
- **Descrição:** `Cadastro de produtos da loja`

#### 4️⃣ Adicione os campos:

**Campo 1 - Código:**
```
Nome interno: codigo
Nome exibido: Código do Produto
Tipo: 1 (Texto Curto)
Obrigatório: Sim
Valor único: Sim
```

**Campo 2 - Nome:**
```
Nome interno: nome
Nome exibido: Nome do Produto
Tipo: 1 (Texto Curto)
Obrigatório: Sim
```

**Campo 3 - Preço:**
```
Nome interno: preco
Nome exibido: Preço de Venda
Tipo: 7 (Dinheiro)
Obrigatório: Sim
Valor mínimo: 0
```

**Campo 4 - Estoque:**
```
Nome interno: estoque
Nome exibido: Quantidade em Estoque
Tipo: 5 (Número Inteiro)
Obrigatório: Sim
Valor mínimo: 0
```

**Campo 5 - Categoria:**
```
Nome interno: categoria
Nome exibido: Categoria
Tipo: 16 (Lista única)
Opções:
  - Eletrônicos
  - Roupas
  - Alimentos
  - Outros
Obrigatório: Sim
```

**Campo 6 - Ativo:**
```
Nome interno: ativo
Nome exibido: Produto Ativo
Tipo: 15 (Sim/Não)
Obrigatório: Sim
Valor padrão: Sim
```

#### 5️⃣ Salve o formulário
- Digite `s` para salvar
- Arquivo salvo em: `data/forms/produtos.form`

#### 6️⃣ Cadastre produtos!
- Menu → `3. Cadastrar Dados`
- Preencha os campos
- Repita para vários produtos

#### 7️⃣ Exporte para Excel
- Menu → `6. Exportar Dados`
- Escolha formato: CSV
- Abra no Excel! ✓

---

### 👥 Cenário 2: Cadastro de Clientes (CRM)

#### Opção Rápida: Use o Template!

```bash
# No menu principal
Escolha: 10. Templates Prontos

# Selecione
→ Cadastro de Clientes (completo)

# Template carregado! ✓
```

O template já vem com:
- ✅ Nome completo
- ✅ CPF/CNPJ
- ✅ Email
- ✅ Telefones
- ✅ Endereço completo
- ✅ Categorização
- ✅ Preferências
- ✅ Observações

---

### 🏥 Cenário 3: Clínica Veterinária

#### Criar formulário "Consultas"

**Campos sugeridos:**
1. **Nome do Pet** (Texto)
2. **Espécie** (Lista: Cachorro, Gato, Pássaro, Outro)
3. **Raça** (Texto)
4. **Idade** (Número)
5. **Nome do Dono** (Texto)
6. **Telefone do Dono** (Telefone)
7. **Data da Consulta** (Data)
8. **Sintomas** (Texto Longo)
9. **Diagnóstico** (Texto Longo)
10. **Medicamentos** (Texto)
11. **Valor da Consulta** (Dinheiro)
12. **Retorno Necessário** (Sim/Não)

---

### 🎓 Cenário 4: Escola (Cadastro de Alunos)

**Campos sugeridos:**
1. **Matrícula** (Texto) - único
2. **Nome Completo** (Texto)
3. **Data de Nascimento** (Data)
4. **Série/Ano** (Lista)
5. **Turma** (Lista)
6. **Nome do Responsável** (Texto)
7. **Telefone do Responsável** (Telefone)
8. **Email do Responsável** (Email)
9. **Endereço** (Texto)
10. **Observações Médicas** (Texto Longo)
11. **Ativo** (Sim/Não)

---

## 💡 Dicas Rápidas

### ✅ Validações Úteis

**Para CPF/CNPJ:**
- Marque como "Valor único" para evitar duplicatas
- A validação de dígitos é automática!

**Para Email:**
- Marque como "Valor único"
- Validação de formato é automática

**Para Números:**
- Defina valor mínimo para evitar negativos
- Defina valor máximo se houver limite

**Para Textos:**
- Defina tamanho mínimo para evitar dados vazios
- Use "Texto Longo" para descrições/observações

### 📊 Exportação

**CSV (Excel):**
```
Menu → 6. Exportar → CSV
Arquivo em: exports/nome_formulario.csv
Abra no Excel, Google Sheets, LibreOffice
```

**JSON (Desenvolvedores):**
```
Menu → 6. Exportar → JSON
Arquivo em: exports/nome_formulario.json
Use em APIs, apps, outros sistemas
```

### 🔍 Busca Rápida

```
Menu → 5. Buscar/Filtrar

Exemplos:
- "João" → Busca em todos os campos
- Filtros: Categoria = Eletrônicos
- Filtros: Preço > 100
- Ordenar: Por nome (A-Z)
```

### 💾 Backup Automático

O sistema faz backup automaticamente:
```
backups/
├── produtos_2025-10-02_14-30.bak
├── produtos_2025-10-02_15-00.bak
└── produtos_2025-10-02_15-30.bak
```

Para restaurar:
```
Menu → 9. Gerenciar Formulários → Restaurar Backup
```

---

## 🎨 Personalizações

### Criar Campos Calculados

Exemplo: **Margem de Lucro**
```
Campo: margem_lucro
Tipo: Decimal
Calculado: Sim
Fórmula: ((preco_venda - preco_custo) / preco_custo) * 100
```

Exemplo: **Total (Quantidade × Preço)**
```
Campo: total
Tipo: Dinheiro
Calculado: Sim
Fórmula: quantidade * preco_unitario
```

### Listas com Muitas Opções

Para Estados (UF):
```
AC, AL, AP, AM, BA, CE, DF, ES, GO, MA, MT, MS, MG,
PA, PB, PR, PE, PI, RJ, RN, RS, RO, RR, SC, SP, SE, TO
```

Para Categorias:
```
Organize por hierarquia:
- Eletrônicos > Celulares
- Eletrônicos > Computadores
- Roupas > Masculino
- Roupas > Feminino
```

---

## ❓ Problemas Comuns

### "Erro ao compilar"
```bash
# Instale o GCC
# Ubuntu/Debian:
sudo apt-get install build-essential

# macOS:
xcode-select --install

# Windows:
# Instale MinGW ou use WSL
```

### "Arquivo não encontrado"
```bash
# Certifique-se de estar no diretório correto
cd formdb

# Compile novamente
make clean
make
```

### "Dados não salvam"
```bash
# Verifique permissões da pasta data/
ls -la data/

# Se necessário:
chmod -R 755 data/
```

---

## 🎯 Próximos Passos

Depois de dominar o básico:

1. **Relacionamentos** - Ligue formulários (Cliente → Pedidos)
2. **Relatórios** - Gere estatísticas automáticas
3. **Importação** - Importe planilhas existentes
4. **Templates** - Use templates prontos para economizar tempo
5. **Avançado** - Explore campos calculados e validações complexas

---

## 🆘 Precisa de Ajuda?

```bash
# No terminal
make help

# No programa
Menu → ? ou F1 (Ajuda)
```

---

## ✨ Você Consegue!

**Em 10 minutos você já terá seu sistema rodando!** 🚀

Comece simples, adicione complexidade conforme necessário.

**Boa sorte!** 🎉