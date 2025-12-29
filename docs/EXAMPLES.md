# 📚 FormDB - Exemplos Práticos

## 🎯 Formulários Prontos para Copiar

Use estes exemplos como base para seus próprios formulários!

---

## 📦 Exemplo 1: E-commerce / Loja

### Formulário: Produtos

```
Nome interno: produtos
Nome exibido: Controle de Estoque - Produtos

Campos:
1. codigo (Texto Curto) - Obrigatório, Único
2. nome (Texto Curto) - Obrigatório, 3-100 caracteres
3. descricao (Texto Longo) - Opcional, até 500 caracteres
4. categoria (Lista única) - Eletrônicos, Roupas, Alimentos, Livros
5. preco_custo (Dinheiro) - Obrigatório, >= 0
6. preco_venda (Dinheiro) - Obrigatório, >= 0
7. estoque_atual (Número) - Obrigatório, >= 0, padrão: 0
8. estoque_minimo (Número) - Obrigatório, >= 0, padrão: 10
9. fornecedor (Texto Curto) - Opcional
10. codigo_barras (Texto Curto) - Opcional, 13 caracteres
11. ativo (Sim/Não) - Obrigatório, padrão: Sim
```

**Exemplo de Registro:**
```
Código: PROD001
Nome: Mouse Gamer RGB
Descrição: Mouse óptico com 6 botões programáveis e iluminação RGB
Categoria: Eletrônicos
Preço de Custo: 45.00
Preço de Venda: 89.90
Estoque Atual: 25
Estoque Mínimo: 5
Fornecedor: TechSupply LTDA
Código de Barras: 7891234567890
Ativo: Sim
```

---

## 👥 Exemplo 2: CRM / Clientes

### Formulário: Clientes

```
Nome interno: clientes
Nome exibido: Cadastro de Clientes

Campos:
1. nome_completo (Texto Curto) - Obrigatório, 3-100 caracteres
2. cpf (CPF) - Opcional, Único
3. email (Email) - Obrigatório, Único
4. telefone (Telefone) - Obrigatório
5. telefone_alternativo (Telefone) - Opcional
6. cep (CEP) - Opcional
7. endereco (Texto Curto) - Opcional
8. cidade (Texto Curto) - Opcional
9. estado (Lista única) - SP, RJ, MG, etc
10. categoria (Lista única) - Bronze, Prata, Ouro, VIP
11. data_cadastro (Data) - Obrigatório
12. ativo (Sim/Não) - Obrigatório, padrão: Sim
13. observacoes (Texto Longo) - Opcional
```

**Exemplo de Registro:**
```
Nome Completo: Maria Silva Santos
CPF: 123.456.789-09
E-mail: maria.silva@email.com
Telefone: (11) 98765-4321
Telefone Alternativo: (11) 3456-7890
CEP: 01310-100
Endereço: Av. Paulista, 1578 - Apto 42
Cidade: São Paulo
Estado: SP
Categoria: Ouro
Data de Cadastro: 15/10/2025
Ativo: Sim
Observações: Cliente VIP, preferência de contato por WhatsApp
```

---

## 🏥 Exemplo 3: Clínica / Consultório

### Formulário: Pacientes

```
Nome interno: pacientes
Nome exibido: Cadastro de Pacientes

Campos:
1. prontuario (Texto Curto) - Obrigatório, Único
2. nome_completo (Texto Curto) - Obrigatório
3. cpf (CPF) - Obrigatório, Único
4. data_nascimento (Data) - Obrigatório
5. sexo (Lista única) - Masculino, Feminino, Outro
6. telefone (Telefone) - Obrigatório
7. email (Email) - Opcional
8. endereco (Texto Curto) - Obrigatório
9. responsavel (Texto Curto) - Opcional (para menores)
10. telefone_responsavel (Telefone) - Opcional
11. convenio (Lista única) - Particular, Unimed, Bradesco Saúde, SulAmérica
12. alergi as (Texto Longo) - Opcional
13. observacoes_medicas (Texto Longo) - Opcional
14. ativo (Sim/Não) - Obrigatório, padrão: Sim
```

**Exemplo de Registro:**
```
Prontuário: P2025-0001
Nome Completo: João Carlos Oliveira
CPF: 987.654.321-00
Data de Nascimento: 15/03/1985
Sexo: Masculino
Telefone: (11) 99876-5432
E-mail: joao.oliveira@email.com
Endereço: Rua das Flores, 123
Responsável: (vazio)
Telefone Responsável: (vazio)
Convênio: Unimed
Alergias: Penicilina, Dipirona
Observações Médicas: Hipertensão controlada, faz uso de Losartana 50mg
Ativo: Sim
```

### Formulário: Consultas

```
Nome interno: consultas
Nome exibido: Registro de Consultas

Campos:
1. prontuario_paciente (Texto Curto) - Obrigatório
2. data_consulta (Data) - Obrigatório
3. hora_consulta (Hora) - Obrigatório
4. medico (Texto Curto) - Obrigatório
5. especialidade (Lista única) - Clínico Geral, Pediatria, Cardiologia, etc
6. queixa_principal (Texto Longo) - Obrigatório
7. diagnostico (Texto Longo) - Opcional
8. prescricao (Texto Longo) - Opcional
9. valor_consulta (Dinheiro) - Obrigatório
10. forma_pagamento (Lista única) - Dinheiro, Cartão, PIX, Convênio
11. retorno_necessario (Sim/Não) - Obrigatório
```

---

## 🐾 Exemplo 4: Clínica Veterinária

### Formulário: Animais

```
Nome interno: pets
Nome exibido: Cadastro de Animais

Campos:
1. nome_pet (Texto Curto) - Obrigatório
2. especie (Lista única) - Cachorro, Gato, Pássaro, Réptil, Outro
3. raca (Texto Curto) - Opcional
4. idade (Número) - Opcional, >= 0
5. peso (Decimal) - Opcional, >= 0
6. sexo (Lista única) - Macho, Fêmea
7. nome_tutor (Texto Curto) - Obrigatório
8. telefone_tutor (Telefone) - Obrigatório
9. email_tutor (Email) - Opcional
10. observacoes (Texto Longo) - Opcional
11. castrado (Sim/Não) - Obrigatório
12. vacinado (Sim/Não) - Obrigatório
```

**Exemplo de Registro:**
```
Nome do Pet: Rex
Espécie: Cachorro
Raça: Golden Retriever
Idade: 5
Peso: 32.5
Sexo: Macho
Nome do Tutor: Ana Paula Costa
Telefone do Tutor: (11) 98765-1234
E-mail do Tutor: ana.costa@email.com
Observações: Dócil, sociável com outros animais
Castrado: Sim
Vacinado: Sim
```

---

## 🎓 Exemplo 5: Escola

### Formulário: Alunos

```
Nome interno: alunos
Nome exibido: Cadastro de Alunos

Campos:
1. matricula (Texto Curto) - Obrigatório, Único
2. nome_completo (Texto Curto) - Obrigatório
3. data_nascimento (Data) - Obrigatório
4. cpf (CPF) - Opcional
5. serie (Lista única) - 1º Ano, 2º Ano, 3º Ano, etc
6. turma (Lista única) - A, B, C, D
7. nome_responsavel (Texto Curto) - Obrigatório
8. telefone_responsavel (Telefone) - Obrigatório
9. email_responsavel (Email) - Obrigatório
10. endereco (Texto Longo) - Obrigatório
11. observacoes_medicas (Texto Longo) - Opcional
12. transporte_escolar (Sim/Não) - Obrigatório
13. ativo (Sim/Não) - Obrigatório, padrão: Sim
```

---

## 🍕 Exemplo 6: Restaurante

### Formulário: Cardápio

```
Nome interno: cardapio
Nome exibido: Cardápio do Restaurante

Campos:
1. codigo (Texto Curto) - Obrigatório, Único
2. nome_prato (Texto Curto) - Obrigatório
3. descricao (Texto Longo) - Obrigatório
4. categoria (Lista única) - Entrada, Prato Principal, Sobremesa, Bebida
5. preco (Dinheiro) - Obrigatório, >= 0
6. tempo_preparo (Número) - Em minutos, >= 0
7. ingredientes (Texto Longo) - Obrigatório
8. calorias (Número) - Opcional
9. vegetariano (Sim/Não) - Obrigatório
10. sem_gluten (Sim/Não) - Obrigatório
11. disponivel (Sim/Não) - Obrigatório, padrão: Sim
12. avaliacao (Avaliação) - 1-5 estrelas
```

**Exemplo de Registro:**
```
Código: PRATO-001
Nome do Prato: Feijoada Completa
Descrição: Feijoada tradicional com arroz, couve, farofa e laranja
Categoria: Prato Principal
Preço: 45.90
Tempo de Preparo: 30
Ingredientes: Feijão preto, linguiça, bacon, carne seca, paio
Calorias: 850
Vegetariano: Não
Sem Glúten: Não
Disponível: Sim
Avaliação: 5
```

---

## 💼 Exemplo 7: RH / Funcionários

### Formulário: Funcionários

```
Nome interno: funcionarios
Nome exibido: Cadastro de Funcionários

Campos:
1. matricula (Texto Curto) - Obrigatório, Único
2. nome_completo (Texto Curto) - Obrigatório
3. cpf (CPF) - Obrigatório, Único
4. data_nascimento (Data) - Obrigatório
5. email_corporativo (Email) - Obrigatório, Único
6. telefone (Telefone) - Obrigatório
7. cargo (Texto Curto) - Obrigatório
8. departamento (Lista única) - TI, Vendas, Marketing, RH, Financeiro
9. data_admissao (Data) - Obrigatório
10. salario (Dinheiro) - Obrigatório, >= 0
11. tipo_contrato (Lista única) - CLT, PJ, Estágio, Temporário
12. endereco (Texto Longo) - Obrigatório
13. ativo (Sim/Não) - Obrigatório, padrão: Sim
```

---

## 🎯 Dicas de Uso

### Relacionamentos entre Formulários

Você pode criar formulários relacionados! Exemplo:

**Clientes** → **Pedidos** → **Itens do Pedido**

No formulário "Pedidos", adicione um campo:
```
nome_cliente (Texto Curto) - Digite o nome exato do cliente
```

Depois faça a busca manual ou use o ID!

### Campos Calculados (Futuro)

Você pode planejar campos assim:
```
lucro = preco_venda - preco_custo
margem_percentual = (lucro / preco_custo) * 100
```

### Validações Customizadas

Para CEP com auto-completar, use:
```
Campo: cep (CEP)
→ Futuramente integrará com API ViaCEP
```

---

## 🚀 Comece Agora!

1. Escolha um exemplo acima
2. Execute: `make run`
3. Crie o formulário
4. Cadastre dados
5. Exporte para Excel!

**Boa sorte!** 🎉