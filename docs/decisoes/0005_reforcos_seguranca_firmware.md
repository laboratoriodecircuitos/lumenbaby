# Decisão 0005 — Reforços de segurança no firmware

## Contexto

Após a Etapa 06, o firmware da LumenBABY já possui modos de iluminação
validados com MOSFET IRLZ44N, LED simples e resistor de 1 kΩ, além de
persistência dos índices de brilho manual e automático na EEPROM.

A próxima necessidade é reforçar estados defensivos do firmware antes de avançar
para testes elétricos com fita LED 12 V e alimentação de potência. Esta etapa
não deve alterar a experiência visual já aprovada nem adicionar recursos de
usuário.

## Decisão tomada

Implementar pequenos reforços de segurança e previsibilidade no firmware,
mantendo a arquitetura simples, a plataforma Arduino Pro Mini 5V / 16 MHz e o
comportamento já validado dos modos.

## Reforços implementados

- Inicialização com pino PWM configurado como saída e brilho lógico 0 aplicado
  logo no início.
- Função dedicada para forçar LED apagado usando a conversão correta entre
  brilho lógico e PWM físico.
- Sanitização defensiva de brilho lógico para manter valores entre 0 e 255.
- Sanitização defensiva dos índices `manualBrightnessIndex` e
  `automaticBrightnessIndex` antes de acessar a tabela de níveis.
- Reforço no carregamento e salvamento da EEPROM para usar índices válidos ou
  padrões seguros.
- Cancelamento de estados pendentes de hold e transição automática ao trocar de
  modo.
- Comportamento previsível para a histerese do LDR caso o limiar claro seja
  configurado menor ou igual ao limiar escuro.
- Mensagem curta de debug quando a configuração dos limiares do LDR exigir
  fallback defensivo.

## O que não foi implementado

Não foram implementados:

- watchdog;
- modo sleep;
- leitura de tensão de bateria;
- sensor de bateria;
- proteção real de sobrecorrente;
- fusível eletrônico;
- calibração automática do LDR;
- Wi-Fi, Bluetooth, display, aplicativo ou sensor PIR;
- troca de plataforma;
- bibliotecas externas.

## Justificativa

Os reforços reduzem riscos de estados incoerentes no firmware sem aumentar a
complexidade do projeto e sem mudar o uso normal da luminária. A proposta segue
a filosofia do projeto: simplicidade, replicabilidade, custo acessível e
experiência visual.

## Consequências

- A inicialização fica mais explícita e previsível.
- A tabela de brilho fica protegida contra acesso por índice inválido.
- A EEPROM continua com fallback seguro caso os dados estejam vazios,
  corrompidos ou fora da faixa.
- A troca de modos fica menos suscetível a herdar estados pendentes de outro
  modo.
- Os modos automático, manual e respiração mantêm a experiência já validada.

## Limites da segurança por firmware

Esses reforços tornam o firmware mais defensivo dentro dos limites do hardware
atual. Eles não tornam o produto "à prova de falhas" em sentido absoluto e não
devem ser comunicados como item de segurança crítica.

Proteções de firmware não substituem boas práticas elétricas: resistor
pull-down no gate do MOSFET, isolamento das conexões, dimensionamento correto de
fios, validação térmica, fonte adequada e proteção física contra falhas.

## Observação

Esta decisão não substitui testes elétricos reais com fita LED 12 V, MT3608,
TP4056, baterias 18650, fonte externa de 12 V ou carga de potência. Esses testes
continuam pendentes e devem ter checklist próprio antes de qualquer validação de
potência.
