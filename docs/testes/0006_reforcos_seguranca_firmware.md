# Teste 0006 — Reforços de segurança no firmware

## Objetivo

Validar a Etapa 07 do firmware da LumenBABY usando apenas MOSFET IRLZ44N, LED
simples e resistor de 1 kΩ, conferindo:

- inicialização segura com LED apagado;
- troca de modos sem estados pendentes incoerentes;
- botões mantendo o comportamento já validado;
- EEPROM mantendo fallback seguro;
- modos automático, manual e respiração preservados.

Este teste não valida fita LED 12 V, MT3608, TP4056, baterias 18650, fonte
externa de 12 V ou carga de potência.

## Pré-requisitos físicos

- Arduino Pro Mini 5V / 16 MHz.
- Conversor USB-serial 5 V.
- MOSFET IRLZ44N.
- Resistor de gate entre 100 Ω e 220 Ω.
- Resistor pull-down de 10 kΩ no gate do MOSFET.
- LED comum.
- Resistor de 1 kΩ em série com o LED.
- LDR em divisor de tensão ligado ao A0.
- Botão de modo em D2.
- Botão de brilho em D3.
- GND comum.

## Conexões permitidas

- Arduino Pro Mini ao conversor USB-serial.
- LDR no A0.
- Botões em D2 e D3 com `INPUT_PULLUP`.
- D9 no gate do MOSFET pelo resistor de gate.
- Resistor pull-down de 10 kΩ no gate do MOSFET.
- LED comum com resistor de 1 kΩ como carga simples.

## Conexões proibidas

- Fita LED 12 V.
- MT3608.
- TP4056.
- Baterias 18650.
- Fonte externa de 12 V.
- Qualquer carga de potência.

## Procedimento de teste

1. Conferir que somente LED simples com resistor está conectado como carga.
2. Abrir `firmware/LumenBaby/LumenBaby.ino` no Arduino IDE.
3. Selecionar Arduino Pro Mini / ATmega328P 5V 16 MHz.
4. Compilar e carregar o firmware.
5. Abrir o Monitor Serial em 9600 baud.
6. Conferir as mensagens iniciais da Etapa 07.
7. Executar os testes abaixo.
8. Registrar os resultados neste documento.

## Teste de inicialização segura

1. Desligar e religar o Arduino.
2. Observar se o LED permanece apagado no instante inicial.
3. Confirmar no Monitor Serial que o firmware iniciou normalmente.
4. Confirmar que a saída PWM continua indicada como normal.

Resultado: aprovado.

Observações:

- A inicialização segura foi considerada aprovada com a montagem de baixa
  potência.
- O teste continuou limitado a MOSFET IRLZ44N, LED simples e resistor de 1 kΩ.

## Teste de troca de modos

1. Alternar entre automático, respiração e manual usando o botão D2.
2. Durante uma transição do modo automático, trocar para outro modo.
3. Durante um fade manual, trocar para outro modo.
4. Confirmar que não há travamento, salto inesperado ou estado pendente
   incoerente.
5. Confirmar que a experiência visual aprovada foi preservada.

Resultado: aprovado.

Observações:

- A troca entre automático, respiração e manual funcionou a contento.
- Não foi observado comportamento que indicasse herança perigosa de estados
  pendentes entre modos.

## Teste de botões

1. No modo manual, testar toque curto no botão D3.
2. No modo manual, testar pressionar e segurar no botão D3.
3. No modo automático com LDR escuro, testar toque curto no botão D3.
4. No modo automático com LDR escuro, testar pressionar e segurar no botão D3.
5. No modo respiração, confirmar que o botão de brilho não altera EEPROM.

Resultado: aprovado.

Observações:

- Os botões continuaram funcionando corretamente após os reforços.
- O comportamento validado de toque curto e pressionar/segurar foi preservado.

## Teste de EEPROM inválida ou fallback

Este teste só deve ser feito se Rafael considerar viável sem forçar desgaste
desnecessário da EEPROM.

1. Carregar firmware em um Arduino com EEPROM vazia ou dados previamente
   inválidos.
2. Abrir o Monitor Serial em 9600 baud.
3. Confirmar que o firmware informa EEPROM inválida ou vazia.
4. Confirmar que os padrões seguros são usados:
   - automático: índice 6;
   - manual: índice 5.
5. Aguardar a gravação agendada dos padrões.
6. Desligar e religar o Arduino.
7. Confirmar que os padrões passam a ser carregados como dados válidos.

Resultado: não executado nesta validação.

Observações:

- O teste de EEPROM inválida/fallback não foi executado intencionalmente.
- Forçar EEPROM inválida não é necessário neste momento e poderia gerar
  manipulação ou desgaste desnecessário.
- O fallback permanece previsto no firmware para uso futuro, se necessário.

## Teste de confirmação dos modos já validados

1. No modo automático, escurecer e clarear o LDR.
2. Confirmar atraso aproximado de 1 segundo, histerese e fade suave.
3. No modo manual, percorrer os 10 níveis de brilho.
4. Confirmar ajuste por pressionar/segurar.
5. No modo respiração, observar o ciclo visual 4-7-8.
6. Confirmar que `LED_PWM_OUTPUT_INVERTED = false` permanece correto para a
   montagem com MOSFET e LED simples.

Resultado: aprovado.

Observações:

- Os modos já validados foram confirmados sem regressão:
  - modo automático com atraso, histerese e fade;
  - modo manual com 10 níveis e pressionar/segurar;
  - modo respiração visual 4-7-8.
- `LED_PWM_OUTPUT_INVERTED = false` permanece correto.
- O mapa de pinos não foi alterado.

## Registro do resultado

Data do teste: não registrada nesta validação

Responsável: Rafael

Versão do Arduino IDE: não registrada nesta validação

Placa selecionada: Arduino Pro Mini / ATmega328P 5V 16 MHz

Processador selecionado: Arduino Pro Mini / ATmega328P 5V 16 MHz

Resultado geral: aprovado com ressalva. Os reforços de segurança e
previsibilidade do firmware foram validados com MOSFET IRLZ44N, LED simples e
resistor de 1 kΩ. O teste de EEPROM inválida/fallback não foi executado nesta
validação para evitar manipulação ou desgaste desnecessário.

- [x] Inicialização segura validada.
- [x] Troca de modos validada.
- [x] Botões validados após os reforços.
- [ ] EEPROM inválida ou fallback não executado nesta validação.
- [x] Modos já validados confirmados sem regressão.
- [x] `LED_PWM_OUTPUT_INVERTED = false` confirmado.
- [x] Teste feito apenas com MOSFET e LED simples.
- [x] Teste aprovado com ressalva.

## Checklist de segurança

- [x] Fita LED 12 V não conectada.
- [x] MT3608 não conectado.
- [x] TP4056 não conectado.
- [x] Baterias 18650 não conectadas.
- [x] Fonte externa de 12 V não conectada.
- [x] Nenhuma carga de potência conectada.
- [x] Usado apenas LED comum com resistor de 1 kΩ.
- [x] GND comum conferido.

## Mensagens ou observações

- Rafael informou que todo o restante está funcionando a contento.
- A validação foi feita apenas com MOSFET IRLZ44N, LED simples e resistor de
  1 kΩ.
- Fita LED 12 V, MT3608, TP4056, baterias 18650, fonte externa de 12 V e carga
  de potência continuam não validados.
- O firmware ficou mais defensivo e previsível dentro dos limites do hardware
  atual; isso não representa proteção absoluta contra falhas.
