# Decisão 0004 - Persistência dos brilhos na EEPROM

## Contexto

Após a Etapa 05B, a LumenBABY já possui dois índices de brilho separados:

- `automaticBrightnessIndex`, usado pelo modo automático quando o LDR detecta
  ambiente escuro;
- `manualBrightnessIndex`, usado pelo modo manual.

Sem persistência, esses índices voltariam aos valores definidos no firmware a
cada desligamento do Arduino Pro Mini.

## Decisão tomada

Implementar persistência simples na EEPROM interna do ATmega328P, usando a
biblioteca padrão do Arduino:

`#include <EEPROM.h>`

A EEPROM será carregada na inicialização e validada por assinatura, versão dos
dados e faixa dos índices. Se os dados forem válidos, os índices salvos serão
aplicados. Se estiverem vazios, corrompidos ou fora da faixa, o firmware usará
valores padrão seguros e agendará a gravação desses padrões.

## O que será salvo

Serão salvos apenas quatro bytes:

- endereço 0: assinatura fixa;
- endereço 1: versão dos dados;
- endereço 2: índice do brilho automático;
- endereço 3: índice do brilho manual.

Os índices salvos apontam para a tabela de brilho lógico:

`25, 50, 75, 100, 125, 150, 175, 200, 225, 255`

## O que não será salvo

Não serão salvos:

- valores PWM físicos;
- brilho temporário durante fade;
- brilho momentâneo do modo respiração;
- leitura do LDR;
- modo atual;
- estado de transição ou temporizadores internos.

## Justificativa

Salvar somente os índices mantém o recurso simples, legível e compatível com o
Arduino Pro Mini 5V / 16 MHz. Também preserva a separação entre brilho lógico e
PWM físico, evitando que uma futura revisão elétrica obrigue migração de dados
gravados.

## Cuidados com desgaste da EEPROM

A EEPROM não deve ser escrita em cada passo de fade nem a cada incremento
enquanto o botão de brilho estiver pressionado.

O firmware agenda a gravação quando o índice final muda e aguarda
aproximadamente 2 segundos sem novas mudanças antes de gravar. A escrita usa
`EEPROM.update()`, que evita regravar uma célula quando o valor já é o mesmo.

## Consequências

- O último brilho manual volta após desligar e religar o Arduino.
- O último brilho automático volta após desligar e religar o Arduino.
- Os dois brilhos continuam independentes.
- EEPROM inválida não trava o firmware; o sistema volta para padrões seguros.
- O modo respiração continua apenas como efeito visual e não altera a EEPROM.

## Observação

Este recurso depende apenas do Arduino Pro Mini e da EEPROM interna do
ATmega328P. Ele não depende de bateria, fita LED 12 V, MT3608, TP4056, fonte
externa de 12 V ou carga de potência.
