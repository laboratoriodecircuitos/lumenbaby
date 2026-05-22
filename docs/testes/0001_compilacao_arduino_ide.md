# Teste 0001 — Compilação no Arduino IDE

## Objetivo

Validar se o firmware inicial da LumenBABY compila no Arduino IDE usando a
plataforma Arduino Pro Mini 5V / 16 MHz.

Esta validação é apenas de compilação. Nesta etapa, não deve ser feito upload
para a placa e não é necessário conectar nenhum hardware físico.

## Pré-requisitos

- Arduino IDE instalado.
- Suporte a placas Arduino AVR disponível no Arduino IDE.
- Repositório LumenBABY aberto ou acessível no computador.
- Firmware da LumenBABY disponível em `firmware/LumenBaby/LumenBaby.ino`.

## Arquivos envolvidos

- `firmware/LumenBaby/LumenBaby.ino`
- `firmware/LumenBaby/README.md`
- `docs/testes/0001_compilacao_arduino_ide.md`

## Procedimento no Arduino IDE

1. Abrir o Arduino IDE.
2. Abrir diretamente o arquivo `firmware/LumenBaby/LumenBaby.ino`.
3. Selecionar uma placa compatível com Arduino Pro Mini.
4. Selecionar o processador equivalente a ATmega328P 5V / 16 MHz.
5. Usar apenas o comando de Verificar/Compilar.
6. Não usar o comando Upload/Carregar nesta etapa.

## Placa recomendada

Arduino Pro Mini.

## Processador recomendado

ATmega328P 5V / 16 MHz.

## Orientações importantes

- Usar somente Verificar/Compilar.
- Não fazer upload para a placa nesta etapa.
- Não conectar bateria, TP4056, MT3608, MOSFET ou fita LED nesta etapa.
- Não adicionar bibliotecas externas.
- Não transformar o projeto em PlatformIO.
- Não tornar Arduino CLI um requisito obrigatório.

## Resultado esperado

O Arduino IDE deve concluir a verificação/compilação sem erros para a placa
Arduino Pro Mini com processador ATmega328P 5V / 16 MHz.

## Registro do resultado

Data do teste: não registrada nesta validação

Responsável: Rafael

Versão do Arduino IDE: não registrada nesta validação

Placa selecionada: Arduino Pro Mini / ATmega328P 5V 16 MHz

Processador selecionado: Arduino Pro Mini / ATmega328P 5V 16 MHz

Resultado: compilação concluída com sucesso no Arduino IDE.

- [x] Compilou sem erros.
- [ ] Não compilou.

Mensagens ou observações:

- A validação realizada foi apenas Verificar/Compilar.
- Upload/Carregamento não foi realizado nesta etapa.
- Nenhum hardware físico foi conectado.
- Uso de memória de programa: 1620 bytes, 5% do máximo de 32256 bytes.
- Uso de RAM: 39 bytes, 1% do máximo de 2048 bytes.
- O firmware permanece aprovado para a próxima etapa de validação controlada.

```text
Sketch uses 1620 bytes (5%) of program storage space. Maximum is 32256 bytes.
Global variables use 39 bytes (1%) of dynamic memory, leaving 2009 bytes for local variables. Maximum is 2048 bytes.
```

## Próximos passos

Seguir para a próxima etapa de validação controlada, mantendo a separação entre
testes de compilação e testes com hardware físico.

Se algum novo erro de compilação surgir futuramente, registrar a mensagem
exibida pelo Arduino IDE antes de alterar o firmware. A correção deve ser
mínima e limitada ao problema real encontrado.
