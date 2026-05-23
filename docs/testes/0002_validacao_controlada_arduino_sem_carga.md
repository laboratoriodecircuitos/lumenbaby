# Teste 0002 — Validação controlada do Arduino sem carga de potência

## Objetivo

Validar o firmware da LumenBABY no Arduino Pro Mini real, sem carga de potência,
verificando:

- upload do firmware;
- inicialização;
- comunicação Serial de diagnóstico;
- leitura do LDR no A0;
- leitura dos botões em D2 e D3;
- alternância entre modos;
- valor de brilho PWM calculado para D9.

Este teste não valida fita LED, MOSFET, MT3608, TP4056, baterias ou alimentação
de potência.

## Pré-requisitos físicos

- Arduino Pro Mini 5V / 16 MHz.
- Conversor USB-serial compatível com 5 V.
- Cabo USB para o conversor USB-serial.
- Arduino IDE instalado.
- LDR e resistor para divisor de tensão, se a leitura do A0 for testada com
  componente real.
- Dois botões push-button, se a leitura dos botões for testada com componentes
  reais.

## Conexões permitidas

- Arduino Pro Mini ao conversor USB-serial.
- Conversor USB-serial ao computador.
- LDR em divisor de tensão ligado ao A0.
- Botão de modo ligado ao D2 e GND, usando `INPUT_PULLUP`.
- Botão de brilho ligado ao D3 e GND, usando `INPUT_PULLUP`.
- GND comum entre Arduino, conversor USB-serial e componentes de entrada usados
  no teste.

## Conexões proibidas nesta etapa

- MOSFET IRLZ44N.
- Fita LED.
- MT3608.
- TP4056.
- Baterias 18650.
- Fonte externa de 12 V.
- Qualquer carga de potência no D9.

## Ordem segura

1. Conferir que não há MOSFET, fita LED, MT3608, TP4056, baterias ou fonte de
   12 V conectados.
2. Conectar apenas o Arduino Pro Mini ao conversor USB-serial.
3. Conferir VCC, GND, TX, RX e, se necessário, DTR/RESET do conversor.
4. Abrir o firmware no Arduino IDE.
5. Selecionar a placa e o processador corretos.
6. Compilar o firmware.
7. Fazer upload para o Arduino Pro Mini.
8. Abrir o Monitor Serial em 9600 baud.
9. Executar os testes de leitura e botões sem conectar carga de potência.

## Procedimento no Arduino IDE

1. Abrir o arquivo `firmware/LumenBaby/LumenBaby.ino`.
2. Selecionar placa compatível com Arduino Pro Mini.
3. Selecionar processador equivalente a ATmega328P 5V / 16 MHz.
4. Selecionar a porta serial do conversor USB-serial.
5. Usar Verificar/Compilar.
6. Se a compilação estiver correta, usar Upload/Carregar para gravar o Arduino.

## Procedimento no Monitor Serial

1. Abrir o Monitor Serial após o upload.
2. Selecionar 9600 baud.
3. Observar as mensagens iniciais de diagnóstico.
4. Observar a linha periódica exibida aproximadamente a cada 1000 ms.
5. Confirmar que a linha mostra modo atual, leitura do LDR, PWM D9, índice de
   brilho manual e orientação dos botões com `INPUT_PULLUP`.

## Testes a realizar

### Inicialização

Verificar se o Arduino inicia e envia mensagens pelo Monitor Serial.

Resultado: aprovado.

Observações: o firmware foi compilado e carregado com sucesso no Arduino Pro
Mini. O Monitor Serial funcionou em 9600 baud e exibiu diagnóstico periódico
aproximadamente a cada 1000 ms.

### Leitura do LDR

Observar o valor de `LDR A0` no Monitor Serial e variar a iluminação sobre o
LDR, se o componente estiver conectado.

Resultado: aprovado após correção da ligação do divisor de tensão.

Observações: inicialmente houve uma ligação incorreta que deixava a leitura
praticamente presa entre 1021 e 1023. Após a correção, o LDR funcionou
corretamente:

- LDR com luz: aproximadamente 850;
- LDR no escuro/luz apagada: aproximadamente 14;
- LDR com luz novamente: aproximadamente 850.

Isso confirma que o divisor de tensão está correto e que a orientação da leitura
combina com a lógica atual do firmware.

### Botão de modo

Pressionar o botão em D2 e verificar se o modo alterna entre automático,
respiração e manual.

Resultado: aprovado.

Observações: a alternância observada foi respiração, manual, automático e
respiração. O Monitor Serial exibiu mensagens como:

```text
Modo alterado para: manual
Modo alterado para: automatico
Modo alterado para: respiracao
```

### Botão de brilho

Entrar no modo manual, pressionar o botão em D3 e verificar se o índice de
brilho manual muda.

Resultado: aprovado.

Observações: o botão de brilho foi validado no modo manual. O índice de brilho
alternou corretamente usando `INPUT_PULLUP`, com pressionado = LOW:

- índice 1: PWM 140;
- índice 2: PWM 220;
- índice 0: PWM 70.

### Observação do valor PWM

Observar o campo `PWM D9` no Monitor Serial em cada modo. Nesta etapa, o valor é
apenas diagnóstico do firmware, sem carga conectada ao pino D9.

Resultado: aprovado pelo diagnóstico Serial.

Observações: no modo automático, com LDR em aproximadamente 845, o PWM D9 ficou
em 0, coerente com ambiente claro e com `LDR_DARK_THRESHOLD = 450`.

No modo respiração, o PWM D9 variou no Monitor Serial. Foram observados valores
como PWM 165 e PWM 131.

Esta validação foi feita apenas pelo diagnóstico Serial, sem LED, MOSFET ou fita
LED.

## Registro do resultado

Data do teste: não registrada nesta validação

Responsável: Rafael

Versão do Arduino IDE: não registrada nesta validação

Placa selecionada: Arduino Pro Mini / ATmega328P 5V 16 MHz

Processador selecionado: Arduino Pro Mini / ATmega328P 5V 16 MHz

Porta serial usada: não registrada nesta validação

Resultado geral: validação controlada concluída com sucesso, sem carga de
potência conectada.

- [x] Upload realizado com sucesso.
- [x] Inicialização confirmada.
- [x] Monitor Serial funcionando em 9600 baud.
- [x] Leitura do LDR observada.
- [x] Botão de modo validado.
- [x] Botão de brilho validado.
- [x] Valor PWM observado no Monitor Serial.
- [x] Nenhuma carga de potência conectada.
- [ ] Teste não concluído.

Mensagens ou observações:

- Compilação concluída com sucesso.
- O aumento de uso de memória em relação à etapa anterior é esperado porque
  `DEBUG_SERIAL_ENABLED` está em `true` e o diagnóstico Serial foi ativado.
- Upload realizado com sucesso para o Arduino Pro Mini.
- Monitor Serial funcionando em 9600 baud.
- Diagnóstico periódico exibido aproximadamente a cada 1000 ms.
- Nenhum MOSFET, fita LED, MT3608, TP4056, bateria 18650, fonte externa de 12 V
  ou carga de potência foi conectado ao D9.

```text
Sketch uses 3704 bytes (11%) of program storage space. Maximum is 32256 bytes.
Global variables use 221 bytes (10%) of dynamic memory, leaving 1827 bytes for local variables. Maximum is 2048 bytes.
```

## Checklist de segurança

- [x] MOSFET IRLZ44N não conectado.
- [x] Fita LED não conectada.
- [x] MT3608 não conectado.
- [x] TP4056 não conectado.
- [x] Baterias 18650 não conectadas.
- [x] Fonte externa de 12 V não conectada.
- [x] D9 sem carga de potência.
- [x] Teste realizado apenas com Arduino, conversor USB-serial e entradas de
      baixa potência necessárias.

## Observação sobre carga de potência

Este teste não realiza validação com carga de potência. Qualquer teste com
MOSFET, fita LED, MT3608, TP4056, baterias ou fonte externa deve ser planejado
em etapa separada.

## Próximos passos

Firmware aprovado para a próxima etapa de validação controlada.

Preparar a próxima etapa de validação elétrica controlada, ainda com atenção à
separação entre firmware, alimentação e carga de potência.
