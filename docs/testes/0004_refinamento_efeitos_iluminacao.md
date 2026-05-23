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
