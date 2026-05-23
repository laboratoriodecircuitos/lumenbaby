# Firmware Arduino — LumenBABY

Esta pasta contém o firmware principal da LumenBABY.

O arquivo principal é:

`LumenBaby.ino`

## Plataforma-alvo

Arduino Pro Mini 5V / 16 MHz.

## Compilação no Arduino IDE

Para validar a compilação, abrir diretamente o arquivo `LumenBaby.ino` no
Arduino IDE.

Selecionar uma placa compatível com Arduino Pro Mini e o processador equivalente
a ATmega328P 5V / 16 MHz.

Nesta etapa, usar apenas Verificar/Compilar. Não usar Upload/Carregar e não
conectar hardware físico.

## Validação controlada sem carga de potência

Na Etapa 03, o firmware pode ser carregado no Arduino Pro Mini apenas para
validar o microcontrolador, a comunicação Serial, o LDR e os botões.

No Arduino IDE, abrir `LumenBaby.ino`, selecionar Arduino Pro Mini e o
processador equivalente a ATmega328P 5V / 16 MHz.

Conectar somente o Arduino Pro Mini e o conversor USB-serial. Com
`DEBUG_SERIAL_ENABLED` em `true`, abrir o Monitor Serial em 9600 baud para
acompanhar o modo atual, a leitura do LDR, o PWM calculado para D9 e o índice
de brilho manual.

Nesta validação, não conectar MOSFET, fita LED, MT3608, TP4056, baterias 18650
ou fonte externa de 12 V. O debug Serial está ativo para bancada inicial e pode
ser desativado em etapa futura.

## Diagnóstico da Etapa 04

Durante a validação de baixa potência com MOSFET e LED comum com resistor de
1 kΩ, o Monitor Serial pode mostrar LDR, limiar, ambiente claro/escuro, PWM alvo
do modo automático e PWM efetivamente aplicado no D9.

Nesta etapa foi adicionada uma configuração para saída PWM invertida:
`LED_PWM_OUTPUT_INVERTED`.

O firmware diferencia brilho lógico de PWM físico. Brilho lógico 0 sempre
significa apagado para a lógica dos modos, mesmo que o PWM físico precise ser
invertido na montagem de teste.

Para o teste atual com MOSFET e LED simples, `LED_PWM_OUTPUT_INVERTED` voltou
para `false`. Assim, o PWM físico segue o brilho lógico: 0 fica apagado, e
valores maiores aumentam o brilho.

A constante `LED_PWM_OUTPUT_INVERTED` deve ser revisada conforme a montagem
final do MOSFET e da iluminação.

O limiar do LDR ainda deve ser calibrado observando leituras reais de ambiente
claro e escuro. Ainda não houve validação com fita LED 12 V.

## Objetivo do firmware

Criar uma base inicial segura, simples e didática para controlar a luminária
decorativa infantil LumenBABY.

Nesta etapa, o firmware implementa:

- leitura do LDR;
- controle PWM da fita LED;
- estrutura inicial de modo automático;
- estrutura inicial de modo respiração;
- estrutura inicial de modo manual;
- leitura de dois botões com debounce simples;
- estado inicial seguro com MOSFET desligado.

## Mapa de pinos inicial

| Pino | Função |
| --- | --- |
| A0 | Leitura analógica do LDR |
| D2 | Botão de modo com `INPUT_PULLUP` |
| D3 | Botão de brilho com `INPUT_PULLUP` |
| D9 | Saída PWM para o gate do MOSFET da fita LED |

## Modos previstos

### Modo automático

Modo inicial do firmware.

Nesta base, o Arduino lê o LDR e liga ou desliga os LEDs conforme um limite
simples de luminosidade. O limite ainda deve ser ajustado em bancada.

### Modo respiração

Varia o brilho dos LEDs lentamente, sem usar `delay()` longo.

### Modo manual

Permite alternar entre três níveis de brilho:

- baixo;
- médio;
- alto.

O botão de brilho só altera o nível quando o firmware está em modo manual.

## Inicialização segura

O firmware configura o pino PWM dos LEDs como saída e aplica brilho lógico 0
logo no início do `setup()`.

Esse brilho lógico é convertido para o PWM físico correto conforme
`LED_PWM_OUTPUT_INVERTED`. O objetivo é manter o MOSFET desligado durante a
inicialização do Arduino. Mesmo assim, o hardware também deve manter o resistor
pull-down de 10 kΩ no gate do MOSFET.

## Debug Serial

O código possui uma flag de depuração:

`DEBUG_SERIAL_ENABLED`

Nesta etapa, ela fica ativada para a validação controlada sem carga de potência.
Se não for necessário acompanhar mensagens pela Serial em etapa futura, altere
a flag para `false`.

## Cuidados de teste em bancada

Os testes físicos ainda devem ser feitos com atenção especial a:

- ligação correta do MOSFET IRLZ44N;
- resistor de gate entre 100 Ω e 220 Ω;
- resistor pull-down de 10 kΩ no gate;
- ajuste futuro do MT3608 para aproximadamente 10 V;
- uso do TP4056 com proteção;
- montagem segura das baterias 18650 em paralelo no sistema 1S2P;
- aquecimento do MT3608, TP4056, baterias e fita LED;
- polaridade e isolamento das conexões da fita LED.

## Observação

Antes de implementar novas funcionalidades, consultar:

`../../docs/00_DOCUMENTO_INICIAL_LUMENBABY.md`
