# Teste 0007 — Varredura e refino seguro do firmware

## Objetivo

Validar a Etapa 08 do firmware da LumenBABY usando apenas MOSFET IRLZ44N, LED
simples e resistor de 1 kΩ, conferindo:

- fluidez do PWM no modo automático;
- fluidez do PWM no modo manual;
- fluidez do PWM no modo respiração;
- EEPROM após os refinamentos;
- troca de modos sem regressão;
- preservação da experiência já validada.

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
6. Conferir as mensagens iniciais.
7. Executar os testes abaixo.
8. Registrar os resultados neste documento.

## Teste de fluidez do PWM no modo automático

1. Entrar no modo automático.
2. Escurecer o LDR e observar o atraso de aproximadamente 1 segundo.
3. Observar o fade-in do LED simples.
4. Clarear o LDR e observar o fade-out.
5. Confirmar que não há engasgo perceptível durante os fades.
6. Confirmar que o Monitor Serial não gera relatório longo durante fade.

Resultado: aprovado.

Observações:

- O modo automático respondeu corretamente.
- A fluidez do PWM foi considerada adequada.
- O atraso, o fade-in e o fade-out funcionaram sem engasgos perceptíveis.

## Teste de fluidez do PWM no modo manual

1. Entrar no modo manual.
2. Usar toques curtos no botão D3 para percorrer os 10 níveis.
3. Confirmar transições suaves entre níveis.
4. Pressionar e segurar o botão D3 para ajustar brilho gradualmente.
5. Confirmar que o ajuste não apresenta engasgo perceptível.

Resultado: aprovado.

Observações:

- O modo manual respondeu corretamente.
- Os 10 níveis, as transições suaves e o ajuste por pressionar/segurar foram
  confirmados sem engasgos perceptíveis.

## Teste de fluidez do PWM no modo respiração

1. Entrar no modo respiração.
2. Observar pelo menos um ciclo parcial do efeito 4-7-8.
3. Confirmar subida, sustentação e descida suaves.
4. Confirmar que não há piscada ou engasgo perceptível no LED simples.

Resultado: aprovado.

Observações:

- O modo respiração 4-7-8 respondeu corretamente.
- A transição visual permaneceu suave, sem piscadas ou travadas perceptíveis.

## Teste de EEPROM após os refinamentos

1. Ajustar o brilho manual.
2. Aguardar pelo menos 3 segundos sem pressionar botões e sem fade ativo.
3. Confirmar no Monitor Serial se a EEPROM foi salva.
4. Ajustar o brilho automático.
5. Aguardar pelo menos 3 segundos sem pressionar botões e sem transição
   automática pendente.
6. Confirmar no Monitor Serial se a EEPROM foi salva.
7. Desligar e religar o Arduino.
8. Confirmar que os índices manual e automático foram recuperados.

Resultado: aprovado.

Observações:

- A EEPROM continuou funcionando após os refinamentos.
- Os ajustes da Etapa 08 não impediram a persistência e recuperação dos índices.

## Teste de troca de modos

1. Alternar entre automático, respiração e manual usando o botão D2.
2. Trocar de modo durante uma transição visual, quando possível.
3. Confirmar que não há travamento ou estado visual incoerente.
4. Confirmar que o comportamento validado dos botões foi preservado.

Resultado: aprovado.

Observações:

- A troca de modos continuou funcionando sem travamentos ou estados incoerentes.
- O comportamento validado dos botões foi preservado.

## Registro do resultado

Data do teste: não registrada nesta validação

Responsável: Rafael

Versão do Arduino IDE: não registrada nesta validação

Placa selecionada: Arduino Pro Mini / ATmega328P 5V 16 MHz

Processador selecionado: Arduino Pro Mini / ATmega328P 5V 16 MHz

Resultado geral: aprovado. Rafael informou que todos os modos estão respondendo
perfeitamente com MOSFET IRLZ44N, LED simples e resistor de 1 kΩ.

- [x] Fluidez do PWM no modo automático validada.
- [x] Fluidez do PWM no modo manual validada.
- [x] Fluidez do PWM no modo respiração validada.
- [x] EEPROM após refinamentos validada.
- [x] Troca de modos validada.
- [x] `LED_PWM_OUTPUT_INVERTED = false` confirmado.
- [x] Teste feito apenas com MOSFET e LED simples.
- [x] Teste aprovado.

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

- Todos os modos estão respondendo perfeitamente.
- A validação foi feita apenas com MOSFET IRLZ44N, LED simples e resistor de
  1 kΩ.
- `LED_PWM_OUTPUT_INVERTED = false` permanece correto.
- O mapa de pinos não foi alterado.
- Fita LED 12 V, MT3608, TP4056, baterias 18650, fonte externa de 12 V e carga
  de potência continuam não validados.
