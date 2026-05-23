# Teste 0005 - Persistência dos brilhos na EEPROM

## Objetivo

Validar a Etapa 06 do firmware da LumenBABY, confirmando que:

- o último índice de brilho manual é salvo na EEPROM;
- o último índice de brilho automático é salvo na EEPROM;
- os índices manual e automático são independentes;
- os valores são recuperados após desligar e religar o Arduino;
- o modo respiração não altera a EEPROM.

Este teste ainda não valida fita LED 12 V, MT3608, TP4056, baterias 18650,
fonte externa de 12 V ou carga de potência.

## Pré-requisitos físicos

- Arduino Pro Mini 5V / 16 MHz.
- Conversor USB-serial 5 V.
- MOSFET IRLZ44N.
- Resistor de gate entre 100 ohms e 220 ohms.
- Resistor pull-down de 10 kohms no gate do MOSFET.
- LED comum.
- Resistor de 1 kohm em série com o LED.
- LDR em divisor de tensão ligado ao A0.
- Botão de modo em D2.
- Botão de brilho em D3.
- GND comum.

## Conexões permitidas

- Arduino Pro Mini ao conversor USB-serial.
- LDR no A0.
- Botões em D2 e D3 com `INPUT_PULLUP`.
- D9 no gate do MOSFET pelo resistor de gate.
- Resistor pull-down de 10 kohms no gate do MOSFET.
- LED comum com resistor de 1 kohm como carga simples.

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
6. Observar se o setup informa EEPROM carregada ou padrões seguros.
7. Testar os brilhos manual e automático.
8. Aguardar pelo menos 3 segundos após cada ajuste final antes de desligar.
9. Desligar e religar o Arduino para confirmar recuperação dos índices.
10. Registrar os resultados neste documento.

## Teste de salvar brilho manual

1. Entrar no modo manual usando o botão D2.
2. Alterar o brilho manual com toques curtos no botão D3.
3. Aguardar pelo menos 3 segundos sem pressionar botões.
4. Verificar no Monitor Serial se aparece uma linha curta indicando EEPROM salva.
5. Repetir usando pressionar/segurar no botão D3.
6. Aguardar pelo menos 3 segundos após soltar o botão.

Resultado: aprovado.

Observações:

- A Etapa 06 rodou como planejado e os dados persistidos foram recuperados após
  religar o Arduino.

## Teste de salvar brilho automático

1. Entrar no modo automático usando o botão D2.
2. Manter o LDR em condição de ambiente escuro.
3. Alterar o brilho automático com toques curtos no botão D3.
4. Aguardar pelo menos 3 segundos sem pressionar botões.
5. Verificar no Monitor Serial se aparece uma linha curta indicando EEPROM salva.
6. Repetir usando pressionar/segurar no botão D3.
7. Aguardar pelo menos 3 segundos após soltar o botão.

Resultado: aprovado.

Observações:

- A Etapa 06 rodou como planejado e os dados persistidos foram recuperados após
  religar o Arduino.

## Teste de desligar/religar e recuperar valores

1. Escolher um índice manual diferente do padrão.
2. Escolher um índice automático diferente do padrão e diferente do manual.
3. Aguardar a gravação pendente da EEPROM.
4. Desligar o Arduino pela alimentação.
5. Religar o Arduino.
6. Abrir o Monitor Serial em 9600 baud.
7. Confirmar que os índices carregados são os mesmos ajustados antes de
   desligar.
8. Entrar no modo manual e verificar o brilho recuperado.
9. Entrar no modo automático, escurecer o LDR e verificar o brilho recuperado.

Resultado: aprovado.

Observações:

- O Monitor Serial informou `EEPROM carregada. Auto indice: 9 | Manual indice:
  9`, confirmando a recuperação dos índices após religar.

## Teste de confirmar que respiração não altera EEPROM

1. Registrar os índices manual e automático exibidos no Monitor Serial.
2. Entrar no modo respiração usando o botão D2.
3. Observar pelo menos um ciclo parcial do efeito 4-7-8.
4. Não pressionar o botão de brilho durante o modo respiração.
5. Voltar aos modos manual e automático.
6. Confirmar que os índices manual e automático continuam iguais aos registrados.
7. Desligar e religar o Arduino.
8. Confirmar novamente que os índices recuperados não foram alterados pelo modo
   respiração.

Resultado: aprovado.

Observações:

- Antes e durante o modo respiração, os índices exibidos eram `Manual indice:
  9` e `Auto indice: 8`.
- Após passar pelo modo respiração, desligar/religar o Arduino e abrir o
  Monitor Serial, a EEPROM carregou `Auto indice: 8 | Manual indice: 9`.
- Isso confirma que o modo respiração não alterou os índices persistidos na
  EEPROM.

## Registro do resultado

Data do teste: não registrada nesta validação

Responsável: Rafael

Versão do Arduino IDE: não registrada nesta validação

Placa selecionada: Arduino Pro Mini / ATmega328P 5V 16 MHz

Processador selecionado: Arduino Pro Mini / ATmega328P 5V 16 MHz

Resultado geral: aprovado para persistência dos brilhos na EEPROM com MOSFET
IRLZ44N, LED simples e resistor de 1 kohm.

- [x] Brilho manual salvo na EEPROM.
- [x] Brilho automático salvo na EEPROM.
- [x] Índices manual e automático confirmados como independentes.
- [x] Valores recuperados após desligar e religar.
- [x] Modo respiração confirmado sem alterar EEPROM.
- [x] Teste feito apenas com MOSFET e LED simples.
- [x] Teste aprovado para persistência dos brilhos na EEPROM.

## Log observado no Monitor Serial

### Recuperação dos brilhos manual e automático

```text
Etapa 06: brilho manual e automatico persistidos na EEPROM.
Sem fita LED, MT3608, TP4056, baterias ou fonte externa.
Botoes: INPUT_PULLUP, pressionado = LOW.
Saida PWM D9: normal.
EEPROM carregada. Auto indice: 9 | Manual indice: 9
LumenBABY iniciado em modo automatico.
```

Evidência principal: `Auto indice: 9 | Manual indice: 9`.

### Confirmação de que o modo respiração não altera EEPROM

```text
LumenBABY diagnostico Serial ativo.
Etapa 06: brilho manual e automatico persistidos na EEPROM.
Sem fita LED, MT3608, TP4056, baterias ou fonte externa.
Botoes: INPUT_PULLUP, pressionado = LOW.
Saida PWM D9: normal.
EEPROM carregada. Auto indice: 8 | Manual indice: 9
LumenBABY iniciado em modo automatico.
```

Evidência do teste de respiração: antes/durante o modo respiração os índices
eram `Auto indice: 8 | Manual indice: 9`; após religar, a EEPROM carregou os
mesmos índices.

`LED_PWM_OUTPUT_INVERTED = false` permanece correto para o teste com MOSFET
IRLZ44N, LED simples e resistor de 1 kohm.

## Checklist de segurança

- [x] Fita LED 12 V não conectada.
- [x] MT3608 não conectado.
- [x] TP4056 não conectado.
- [x] Baterias 18650 não conectadas.
- [x] Fonte externa de 12 V não conectada.
- [x] Nenhuma carga de potência conectada.
- [x] Usado apenas LED comum com resistor de 1 kohm.
- [x] GND comum conferido.

## Mensagens ou observações

- A Etapa 06 rodou como planejado.
- O firmware carregou os dados da EEPROM com sucesso.
- Os índices de brilho automático e manual foram recuperados da EEPROM após
  religar.
- O modo respiração não alterou os índices persistidos na EEPROM.
- O teste foi feito apenas com MOSFET IRLZ44N, LED simples e resistor de
  1 kohm.
- Fita LED 12 V, MT3608, TP4056, baterias 18650, fonte externa de 12 V e carga
  de potência continuam não validados.
