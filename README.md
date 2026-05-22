# LumenBABY

A **LumenBABY** é o primeiro produto da marca **LumenSense**.

Trata-se de uma luminária decorativa infantil em formato de nuvem, com iluminação indireta, bateria interna recarregável, controle inteligente de brilho e funcionamento sem fios externos durante o uso.

Este repositório concentra a documentação pública, decisões técnicas, testes e firmware Arduino do projeto.

---

## Marca e produto

- **Marca:** LumenSense
- **Produto:** LumenBABY
- **Nome descritivo:** Luminária Nuvem Inteligente
- **Plataforma:** Arduino
- **Ambiente de desenvolvimento:** VSCode
- **IA geradora de código:** Codex

---

## Filosofia do projeto

A LumenBABY não está sendo desenvolvida como um produto industrial de ultra eficiência energética.

A filosofia do projeto é:

- simplicidade;
- replicabilidade;
- custo acessível;
- experiência visual agradável;
- estabilidade suficiente para uso real;
- facilidade de reprodução;
- valor didático para o futuro curso Projetista 360°.

O projeto deve ser analisado como uma luminária inteligente prática, bonita, autônoma, replicável e vendável.

---

## Stack técnica prevista

- Arduino Pro Mini 5V / 16 MHz;
- LDR para leitura de luminosidade ambiente;
- 2 botões push-button;
- MOSFET IRLZ44N para controle PWM da fita LED;
- fita LED 3528 de 12 V;
- conversor boost MT3608;
- 2 baterias 18650 em paralelo, formando sistema 1S2P;
- módulo TP4056 com proteção;
- chave liga/desliga;
- estrutura física em MDF 3 mm;
- iluminação indireta difusa.

---

## Documento principal para o Codex

Antes de gerar código, o Codex deve ler:

`docs/00_DOCUMENTO_INICIAL_LUMENBABY.md`

Esse documento contém o contexto oficial, escopo, filosofia técnica, requisitos e limites do projeto.

---

## Versionamento público

Este projeto será versionado desde o início com Git e publicado no GitHub em repositório público.

Por isso, todos os arquivos devem ser tratados como conteúdo público.

Antes de cada commit e push, verificar:

- ausência de dados sensíveis;
- ausência de credenciais;
- ausência de arquivos privados;
- ausência de informações pessoais;
- ausência de custos internos ou dados comerciais privados;
- documentação atualizada;
- coerência com a filosofia do projeto.

---

## Estrutura inicial

LumenBaby/
  README.md
  LICENSE.md
  CONTRIBUTING.md
  CHANGELOG.md
  .gitignore
  .gitattributes
  docs/
    00_DOCUMENTO_INICIAL_LUMENBABY.md
    01_TAP_RESUMIDO.md
    decisoes/
    testes/
    versionamento/
  firmware/
    LumenBaby/
      LumenBaby.ino
      README.md
