# Decisão 0003 — Refinamento dos modos de iluminação

## Contexto

Após a validação do Arduino sem carga de potência e a validação do MOSFET
IRLZ44N com LED simples, o firmware da LumenBABY já controla o brilho lógico e
o PWM físico aplicado ao D9.

A próxima necessidade é melhorar a experiência visual dos modos antes de testar
carga de potência real, mantendo o projeto simples, didático e adequado ao
Arduino Pro Mini 5V / 16 MHz.

## Decisão tomada

Refinar os três modos principais de iluminação:

- modo automático com atraso antes da resposta e fade suave;
- modo manual com 10 níveis fixos e ajuste suave ao pressionar e segurar;
- modo respiração com ciclo visual inspirado no ritmo 4-7-8.

## Refinamentos definidos

### Modo automático

O modo automático continua usando o LDR e o limiar inicial
`LDR_DARK_THRESHOLD = 450`.

Quando o ambiente muda entre claro e escuro, o firmware aguarda
aproximadamente 1 segundo. Se a condição permanecer válida, inicia um fade suave
de aproximadamente 1 segundo até o brilho alvo.

### Modo manual

O modo manual passa a usar 10 níveis fixos de brilho lógico:

`25, 50, 75, 100, 125, 150, 175, 200, 225, 255`

Um toque curto no botão de brilho avança para o próximo nível. Ao pressionar e
segurar, o brilho sobe ou desce gradualmente; o sentido alterna a cada novo
pressionamento longo.

### Modo respiração

O modo respiração passa a usar um ciclo visual inspirado em 4-7-8:

- inspiração visual: 4 segundos;
- retenção visual: 7 segundos;
- expiração visual: 8 segundos.

O brilho máximo do modo respiração fica abaixo do máximo absoluto, e o brilho
mínimo permanece baixo, mas diferente de zero.

## Justificativa

Esses refinamentos melhoram a experiência visual sem adicionar bibliotecas,
hardware novo, comandos Serial interativos ou arquitetura complexa.

O uso de `millis()` mantém o firmware responsivo aos botões e evita `delay()`
longo.

## Consequências

- O modo automático fica menos brusco e menos sensível a mudanças momentâneas de
  iluminação.
- O modo manual oferece controle mais fino sem complicar a interface.
- O modo respiração fica mais lento, suave e adequado a uma luminária decorativa
  infantil.
- O firmware ainda precisa ser validado apenas com MOSFET e LED simples antes
  de qualquer teste com fita LED 12 V.

## Observação

O ritmo 4-7-8 é usado apenas como referência visual para o efeito de luz. O
produto não deve ser comunicado como recurso médico, terapêutico, respiratório
ou de segurança.
