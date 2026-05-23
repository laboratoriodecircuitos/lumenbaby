# Teste 0004 — Refinamento dos efeitos de iluminação

## Objetivo

Validar a Etapa 05 do firmware da LumenBABY usando apenas MOSFET IRLZ44N com
LED simples e resistor, conferindo:

- atraso e fade suave no modo automático;
- modo manual com 10 níveis;
- toque curto no botão de brilho;
- pressionar e segurar no botão de brilho;
- modo respiração visual inspirado em 4-7-8.

Este teste não deve validar fita LED 12 V, MT3608, TP4056, baterias 18650,
fonte externa de 12 V ou carga de potência.

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
6. Testar os modos automático, manual e respiração.
7. Registrar os resultados neste documento.

## Testes do modo automático

### Atraso antes da resposta

Escurecer o LDR e observar se o LED aguarda aproximadamente 1 segundo antes de
iniciar a resposta.

Resultado: aprovado.

Observações: o modo automático aguardou cerca de 1 segundo antes de responder à
mudança do LDR.

### Fade-in após escurecer

Após o atraso, verificar se o LED acende suavemente até o brilho automático.

Resultado: aprovado.

Observações: após o atraso, o LED fez fade-in suave.

### Fade-out após clarear

Clarear o LDR e verificar se o LED aguarda aproximadamente 1 segundo antes de
apagar suavemente.

Resultado: aprovado.

Observações: após clarear o LDR, o LED aguardou a resposta e fez fade-out
suave.

### Cancelamento da resposta

Durante o atraso, voltar o LDR ao estado anterior e verificar se a transição
pendente é cancelada.

Resultado: aprovado.

Observações: o comportamento geral do modo automático foi considerado correto.
O ajuste de histerese ajudou a evitar oscilação perto do limiar.

## Testes do modo manual

### Dez níveis de brilho

Entrar no modo manual e usar toques curtos no botão de brilho para percorrer os
10 níveis.

Resultado: aprovado.

Observações: os 10 níveis por toque curto funcionaram corretamente.

### Transição suave entre níveis

Verificar se a troca entre níveis acontece com fade curto, sem salto brusco.

Resultado: aprovado.

Observações: a transição entre níveis ficou suave.

## Teste de pressionar e segurar

No modo manual, pressionar e segurar o botão de brilho:

- na primeira ação longa, verificar se o brilho sobe suavemente;
- soltar e pressionar novamente;
- verificar se o brilho desce suavemente;
- confirmar que o LED não apaga durante o ajuste.

Resultado: aprovado.

Observações: pressionar e segurar subiu o brilho suavemente. Em nova ação longa,
o brilho desceu suavemente. O brilho mínimo não apagou o LED.

## Testes do modo respiração 4-7-8

Entrar no modo respiração e observar:

- inspiração visual por aproximadamente 4 segundos;
- retenção visual por aproximadamente 7 segundos;
- expiração visual por aproximadamente 8 segundos;
- brilho máximo abaixo de 255;
- brilho mínimo baixo, mas diferente de zero.

Resultado: aprovado.

Observações: o modo respiração 4-7-8 funcionou corretamente, com subida suave
em 4 segundos, sustentação por 7 segundos e descida suave em 8 segundos. O LED
não atingiu brilho máximo absoluto e não apagou totalmente.

## Teste complementar da Etapa 05B

Este complemento deve ser testado por Rafael após compilar e carregar o
firmware da Etapa 05B, mantendo a mesma montagem com MOSFET IRLZ44N, LED simples
e resistor de 1 kΩ.

Não conectar fita LED 12 V, MT3608, TP4056, baterias 18650, fonte externa de
12 V ou qualquer carga de potência neste complemento.

### Controle de brilho automático por toque curto

Entrar no modo automático, deixar o ambiente escuro no LDR e aplicar toques
curtos no botão D3.

Resultado: aprovado.

Observações: no modo automático com LDR escuro, toques curtos no botão D3
alteraram corretamente os níveis de brilho automático.

### Controle de brilho automático por pressionar e segurar

No modo automático, manter o ambiente escuro no LDR e pressionar/segurar o botão
D3.

- na primeira ação longa, verificar se o brilho sobe suavemente até o máximo;
- soltar e pressionar novamente;
- verificar se o brilho desce suavemente até o mínimo;
- confirmar que o LED não apaga durante o ajuste.

Resultado: aprovado.

Observações: no modo automático com LDR escuro, pressionar e segurar o botão D3
fez o brilho subir suavemente. Em nova ação de pressionar e segurar, o brilho
desceu suavemente. O LED não apagou no brilho mínimo.

### Ajuste de brilho automático com ambiente claro

Clarear o LDR, confirmar que o LED permanece apagado e ajustar o brilho
automático usando toque curto e pressionar/segurar no botão D3.

Resultado: aprovado.

Observações: com o LDR em condição de ambiente claro, o LED permaneceu apagado.
Mesmo assim, o botão D3 alterou internamente o alvo/índice automático para uso
na próxima condição de ambiente escuro.

### Acendimento até o brilho automático selecionado

Após ajustar o brilho automático com o ambiente claro, escurecer o LDR e
verificar se, após o atraso, o LED acende com fade-in até o brilho automático
selecionado.

Resultado: aprovado.

Observações: após ajustar o brilho automático com o ambiente claro, ao escurecer
o LDR o LED acendeu com fade-in até o nível automático selecionado.

### Fade-in, fade-out e cancelamento continuam funcionando

Repetir os testes de escurecer, clarear e cancelar uma transição pendente,
confirmando que o atraso de aproximadamente 1 segundo, o fade-in, o fade-out e a
histerese continuam funcionando no modo automático.

Resultado: aprovado.

Observações: o atraso de aproximadamente 1 segundo, o fade-in, o fade-out, a
histerese e o cancelamento de transição pendente continuaram funcionando no modo
automático após a inclusão do controle de brilho.

Checklist complementar da Etapa 05B:

- [x] Toque curto no modo automático testado.
- [x] Pressionar/segurar no modo automático testado.
- [x] LED permanece apagado no claro ao ajustar brilho automático.
- [x] LED acende no escuro até o brilho automático selecionado.
- [x] Fade-in e fade-out do modo automático continuam funcionando.
- [x] Cancelamento de transição pendente continua funcionando.
- [x] Teste complementar feito apenas com MOSFET e LED simples.

Registro complementar da Etapa 05B:

Data do teste complementar: não registrada nesta validação

Responsável: Rafael

Versão do Arduino IDE: não registrada nesta validação

Placa selecionada: não registrada nesta validação

Processador selecionado: não registrado nesta validação

Resultado complementar: aprovado. O controle de brilho no modo automático foi
validado como funcionando 100% com MOSFET IRLZ44N, LED simples e resistor de
1 kΩ.

Observações complementares:

- O modo automático agora permite ajuste de brilho pelo botão D3.
- No modo automático com LDR escuro, toque curto em D3 altera os níveis de
  brilho.
- No modo automático com LDR escuro, pressionar e segurar sobe suavemente o
  brilho, e uma nova ação longa desce suavemente.
- O LED não apaga no brilho mínimo durante o ajuste automático.
- No modo automático com LDR claro, o LED permanece apagado, mas o botão D3
  altera o alvo/índice automático.
- Após ajuste no claro, ao escurecer o LDR, o LED acende com fade até o nível
  automático selecionado.
- O atraso de aproximadamente 1 segundo, fade-in, fade-out, histerese e
  cancelamento de transição pendente continuam funcionando.
- O modo manual continua funcionando.
- O modo respiração 4-7-8 continua funcionando.
- `LED_PWM_OUTPUT_INVERTED = false` permanece correto.
- O mapa de pinos não foi alterado.
- O teste continuou limitado a MOSFET IRLZ44N, LED simples e resistor de 1 kΩ.
- Fita LED 12 V, MT3608, TP4056, baterias 18650, fonte externa de 12 V e carga
  de potência ainda não foram validados.

## Registro do resultado

Data do teste: não registrada nesta validação

Responsável: Rafael

Versão do Arduino IDE: não registrada nesta validação

Placa selecionada: Arduino Pro Mini / ATmega328P 5V 16 MHz

Processador selecionado: Arduino Pro Mini / ATmega328P 5V 16 MHz

Resultado geral: aprovado. Os modos foram validados como funcionando 100% com
MOSFET IRLZ44N, LED simples e resistor de 1 kΩ.

- [x] Modo automático validado.
- [x] Atraso antes da resposta validado.
- [x] Fade-in validado.
- [x] Fade-out validado.
- [x] Cancelamento da resposta validado.
- [x] Modo manual com 10 níveis validado.
- [x] Pressionar/segurar validado.
- [x] Modo respiração 4-7-8 validado.
- [x] Teste feito apenas com MOSFET e LED simples.
- [ ] Teste não concluído.

Mensagens ou observações:

- O teste foi realizado com MOSFET IRLZ44N, LED simples e resistor de 1 kΩ.
- O modo automático ficou correto: aguarda cerca de 1 segundo antes de responder
  à mudança do LDR e depois faz fade-in ou fade-out suave.
- O "engasgo" percebido anteriormente durante o fade foi resolvido após pausar
  o debug Serial durante fade e adicionar histerese simples ao LDR.
- O modo manual funcionou com 10 níveis, toque curto, transição suave e ajuste
  por pressionar/segurar.
- O modo respiração visual 4-7-8 funcionou corretamente.
- `LED_PWM_OUTPUT_INVERTED = false` permanece correto.
- O mapa de pinos não foi alterado.
- Fita LED 12 V, MT3608, TP4056, baterias 18650, fonte externa de 12 V e carga
  de potência ainda não foram validados.

## Checklist de segurança

- [x] Fita LED 12 V não conectada.
- [x] MT3608 não conectado.
- [x] TP4056 não conectado.
- [x] Baterias 18650 não conectadas.
- [x] Fonte externa de 12 V não conectada.
- [x] Nenhuma carga de potência conectada.
- [x] Usado apenas LED comum com resistor de 1 kΩ.
- [x] GND comum conferido.

## Próximos passos

Registrar a Etapa 05 como validada com LED simples.

Planejar uma etapa separada para validação elétrica com fita LED 12 V e
alimentação apropriada, mantendo checklist de segurança específico para carga de
potência.
