# Decisão 0002 — Mapa de pinos e base inicial do firmware

## Contexto

A LumenBABY será desenvolvida com Arduino Pro Mini 5V / 16 MHz, LDR, dois
botões, fita LED 3528 de 12 V e acionamento por MOSFET IRLZ44N via PWM.

Nesta etapa, o projeto precisa de uma base de firmware segura, didática e
simples, alinhada com a filosofia de simplicidade, replicabilidade, custo
acessível e experiência visual.

## Decisão tomada

O firmware inicial passa a usar um mapa de pinos fixo para os primeiros testes
e uma estrutura de modos com:

- `MODE_AUTO`;
- `MODE_BREATHING`;
- `MODE_MANUAL`.

O modo inicial preferencial será o modo automático.

## Mapa de pinos

| Pino | Função |
| --- | --- |
| A0 | Leitura analógica do LDR |
| D2 | Botão de modo com `INPUT_PULLUP` |
| D3 | Botão de brilho com `INPUT_PULLUP` |
| D9 | Saída PWM para o gate do MOSFET da fita LED |

## Justificativa

O pino A0 atende bem à leitura analógica do divisor com LDR.

Os pinos D2 e D3 são adequados para botões simples com `INPUT_PULLUP`, mantendo
a montagem acessível e com poucos componentes.

O pino D9 possui PWM no Arduino Pro Mini 5V / 16 MHz e permite controlar o gate
do MOSFET da fita LED com variação de brilho.

## Consequências

A base do firmware fica pronta para testes iniciais de:

- inicialização com MOSFET desligado;
- leitura do LDR;
- debounce simples dos botões;
- alternância entre modos;
- controle manual de brilho;
- efeito inicial de respiração.

O resistor pull-down de 10 kΩ no gate do MOSFET continua obrigatório no
hardware, mesmo com o firmware escrevendo PWM 0 na inicialização.

## Observação

Esta decisão pode ser revisada futuramente se houver conflito físico na
montagem, limitação mecânica, ruído elétrico ou necessidade de reorganização dos
pinos.
