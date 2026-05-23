# Teste 0003 — Validação elétrica controlada do MOSFET com LED simples

## Objetivo

Registrar a validação elétrica controlada do acionamento por PWM no pino D9,
usando MOSFET IRLZ44N com LED comum e resistor de 1 kΩ.

Este teste valida apenas o caminho Arduino, PWM, gate do MOSFET e LED simples.
Ele não valida fita LED 12 V, MT3608, TP4056, baterias 18650, fonte externa de
12 V ou carga de potência.

## Pré-requisitos físicos

- Arduino Pro Mini 5V / 16 MHz.
- Conversor USB-serial 5 V.
- MOSFET IRLZ44N.
- Resistor de gate entre 100 Ω e 220 Ω.
- Resistor pull-down de 10 kΩ no gate do MOSFET.
- LED comum.
- Resistor de 1 kΩ em série com o LED.
- LDR.
- Botões de modo e brilho.
- GND comum.

## Conexões utilizadas

- A0: leitura do LDR.
- D2: botão de modo com `INPUT_PULLUP`.
- D3: botão de brilho com `INPUT_PULLUP`.
- D9: PWM para o gate do MOSFET por resistor entre 100 Ω e 220 Ω.
- Gate do MOSFET com pull-down de 10 kΩ para GND.
- LED comum com resistor de 1 kΩ como carga simples de baixa potência.
- GND comum entre Arduino, conversor USB-serial, LDR, botões e MOSFET.

## Conexões proibidas nesta etapa

- Fita LED 12 V.
- MT3608.
- TP4056.
- Baterias 18650.
- Fonte externa de 12 V.
- Qualquer carga de potência.

## Ordem segura

1. Conferir que não há fita LED 12 V, MT3608, TP4056, baterias 18650 ou fonte
   externa de 12 V conectados.
2. Conferir o resistor de gate e o resistor pull-down do MOSFET.
3. Conferir o LED comum com resistor de 1 kΩ.
4. Conferir GND comum.
5. Carregar o firmware pelo Arduino IDE.
6. Abrir o Monitor Serial em 9600 baud.
7. Testar os modos automático, manual e respiração.
8. Observar o LED simples e os valores de brilho lógico e PWM físico no Serial.

## Ligação usada no teste

A ligação usou o Arduino Pro Mini controlando o gate do IRLZ44N pelo pino D9,
com resistor de gate entre 100 Ω e 220 Ω e resistor pull-down de 10 kΩ no gate.

A carga usada foi apenas um LED comum com resistor de 1 kΩ, em montagem de baixa
potência. O teste manteve GND comum entre os módulos envolvidos.

## Resultado esperado

- O MOSFET deve responder ao PWM no D9.
- No modo automático, LDR claro/acima do limiar deve apagar o LED.
- No modo automático, LDR escuro/abaixo do limiar deve acender o LED.
- No modo manual, os níveis de brilho devem alterar a intensidade do LED.
- No modo respiração, o LED deve variar conforme o PWM.
- A saída PWM normal deve funcionar com `LED_PWM_OUTPUT_INVERTED = false`.

## Resultado real obtido

Resultado: aprovado após correção e diagnóstico.

- O teste ficou funcional.
- O MOSFET com LED simples respondeu ao PWM do D9.
- No modo automático, o LED acendeu no escuro e apagou no claro.
- A saída PWM normal foi validada.
- A configuração final correta para esta montagem ficou
  `LED_PWM_OUTPUT_INVERTED = false`.
- O firmware diferenciou corretamente brilho lógico e PWM físico aplicado no D9.
- Com saída normal, os valores ficaram equivalentes:
  - brilho lógico 0 aplica PWM físico 0;
  - brilho lógico 70 aplica PWM físico 70;
  - brilho lógico 140 aplica PWM físico 140;
  - brilho lógico 180 aplica PWM físico 180;
  - brilho lógico 220 aplica PWM físico 220.
- O LDR controla apenas o modo automático.
- Os modos manual e respiração continuaram independentes do LDR.
- O LDR ainda poderá precisar de calibração mais fina em etapa futura.

## Observação complementar após correção do LDR

Durante a validação, foi identificado um erro de montagem no divisor do LDR: o
pino de 5 V estava ligado em um lado do LDR e o pino A0 acabou ficando na linha
de 5 V. Isso causava leituras e comportamentos incorretos durante o diagnóstico.

Após corrigir a montagem do divisor do LDR/A0, o teste final do modo automático
ficou correto.

Com luz:

- LDR A0: 854;
- limiar LDR: 450;
- ambiente: claro;
- brilho lógico D9: 0;
- PWM físico D9: 0;
- saída PWM: normal;
- LED apagado.

Sem luz:

- LDR A0: 164;
- limiar LDR: 450;
- ambiente: escuro;
- brilho lógico D9: 180;
- PWM físico D9: 180;
- saída PWM: normal;
- LED aceso.

Conclusão: o problema anterior não era baixa corrente do Arduino nem necessidade
de inversão de PWM. O problema estava na ligação do divisor do LDR/A0. A saída
correta permanece `LED_PWM_OUTPUT_INVERTED = false`.

Esta observação continua limitada à validação de baixa potência com MOSFET e
LED simples. Fita LED 12 V, MT3608, TP4056, baterias 18650 e fonte externa de
12 V continuam fora desta etapa.

## Tentativa de saída invertida

Foi feita uma tentativa experimental com `LED_PWM_OUTPUT_INVERTED = true`.

Essa inversão não resolveu o problema da montagem de teste. Com a saída
invertida, o brilho lógico 0 virou PWM físico 255, o LED ficou aceso direto e,
ao escurecer o LDR, o brilho ficou apenas levemente menor.

Por isso, a inversão foi revertida e a configuração final validada ficou:

```cpp
LED_PWM_OUTPUT_INVERTED = false
```

## Escopo validado

Este teste validou apenas:

- Arduino Pro Mini;
- saída PWM D9;
- MOSFET IRLZ44N;
- resistor de gate;
- resistor pull-down do gate;
- LED comum com resistor de 1 kΩ;
- leitura do LDR no modo automático;
- modos manual e respiração com LED simples.

Este teste não validou fita LED 12 V.

## Checklist de segurança

- [x] Fita LED 12 V não conectada.
- [x] MT3608 não conectado.
- [x] TP4056 não conectado.
- [x] Baterias 18650 não conectadas.
- [x] Fonte externa de 12 V não conectada.
- [x] Nenhuma carga de potência conectada.
- [x] Usado apenas LED comum com resistor de 1 kΩ.
- [x] Resistor de gate usado.
- [x] Resistor pull-down no gate usado.
- [x] GND comum conferido.

## Próximos passos recomendados

- Registrar a etapa como validação elétrica de baixa potência concluída.
- Manter `LED_PWM_OUTPUT_INVERTED = false` para a montagem atual.
- Calibrar o limiar do LDR futuramente com base nas leituras reais de claro e
  escuro.
- Planejar separadamente a validação com fita LED 12 V, MT3608, TP4056,
  baterias e fonte externa, sem misturar essa etapa com o teste de LED simples.
