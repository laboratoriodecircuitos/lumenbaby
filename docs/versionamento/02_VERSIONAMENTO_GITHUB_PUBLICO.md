# 02 — Versionamento Público no GitHub

## 1. Decisão oficial

O Projeto LumenBABY será versionado desde o início com Git e publicado no GitHub em repositório público.

O repositório público deve conter apenas arquivos que possam ser expostos publicamente sem risco técnico, comercial, pessoal ou estratégico.

---

## 2. Objetivo do repositório público

O repositório público terá como objetivos:

- registrar a evolução técnica do projeto;
- manter documentação organizada;
- permitir que o Codex encontre contexto confiável;
- servir como histórico real de desenvolvimento;
- apoiar o futuro uso didático no Projetista 360°;
- demonstrar processo profissional de criação de produto eletrônico.

---

## 3. O que pode ser versionado

Podem ser versionados:

- firmware Arduino;
- documentação técnica;
- decisões de projeto;
- registros de testes sem dados pessoais;
- listas de componentes sem informações comerciais sensíveis;
- instruções de montagem;
- README;
- CHANGELOG;
- arquivos de configuração do projeto.

---

## 4. O que não deve ser versionado

Não devem ser versionados:

- dados pessoais;
- contatos de clientes;
- notas fiscais;
- comprovantes;
- custos internos sensíveis;
- estratégias privadas de precificação;
- credenciais;
- tokens;
- senhas;
- arquivos `.env`;
- documentos privados;
- dados de fornecedores que não devam ser públicos;
- fotos de pessoas sem autorização;
- arquivos temporários ou backups.

---

## 5. Regra importante

Repositório público não significa, automaticamente, projeto livre para uso comercial por terceiros.

Enquanto não houver uma licença aberta formal, o projeto deve ser tratado como:

Todos os direitos reservados ao autor/proprietário do projeto.

---

## 6. Padrão de commits

Padrão sugerido:

- docs: create initial project documentation
- docs: define public repository policy
- firmware: add initial Arduino sketch
- firmware: implement LDR reading
- firmware: add PWM LED control
- firmware: implement breathing mode
- test: document LDR validation
- docs: update assembly notes

---

## 7. Regra para o Codex

Sempre que o Codex alterar o projeto, ele deve considerar que o repositório é público.

Portanto, o Codex deve:

- não criar arquivos com segredos;
- não incluir dados pessoais;
- não gerar credenciais falsas realistas;
- não criar arquivos privados;
- não inserir informações comerciais sensíveis;
- manter README e documentação coerentes quando fizer alterações relevantes;
- sugerir commits claros ao final das etapas.

---

## 8. Checklist antes de cada push

Antes de enviar para o GitHub, conferir:

- git status;
- arquivos modificados;
- ausência de .env;
- ausência de dados pessoais;
- ausência de arquivos privados;
- ausência de credenciais;
- ausência de custos internos sensíveis;
- README atualizado quando necessário;
- documentação coerente com a etapa.
