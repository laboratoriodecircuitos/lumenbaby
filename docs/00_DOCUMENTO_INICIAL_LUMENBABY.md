# 00 — Documento Inicial do Projeto LumenBABY

## 1. Identidade do projeto

**Marca:** LumenSense  
**Produto:** LumenBABY  
**Nome descritivo:** Luminária Nuvem Inteligente  
**Categoria:** luminária decorativa infantil de parede com bateria interna e controle inteligente de iluminação  
**Central do projeto:** ChatGPT — Central do Projeto LumenBABY  
**Ambiente de desenvolvimento:** VSCode  
**IA geradora de código:** Codex  
**Plataforma de firmware:** Arduino  
**Repositório:** público no GitHub

---

## 2. Conceito do produto

A LumenBABY é uma luminária decorativa infantil em formato de nuvem, desenvolvida para uso em quarto de bebê.

O produto oferece iluminação indireta, suave e acolhedora, com funcionamento sem fios externos durante o uso, graças à bateria interna recarregável.

A luminária deve auxiliar pais e responsáveis durante a rotina noturna, como amamentação, troca de fralda, circulação no quarto e momentos em que é necessário iluminar o ambiente sem acender a luz principal.

O produto não deve ser comunicado como brinquedo, produto médico, item de segurança crítica ou dispositivo de monitoramento infantil.

---

## 3. Filosofia técnica oficial

Este projeto deve seguir a seguinte filosofia:

simplicidade + replicabilidade + custo acessível + experiência visual

A LumenBABY não será tratada como produto industrial de ultra eficiência energética.

O objetivo é criar uma luminária inteligente prática, bonita, autônoma, replicável e vendável, com arquitetura simples e didática.

As decisões técnicas já aceitas dentro dessa filosofia incluem:

- uso de Arduino Pro Mini 5V / 16 MHz;
- uso de TP4056 com proteção para recarga das baterias;
- uso de 2 células 18650 em paralelo, formando sistema 1S2P;
- uso de MT3608 para elevar a tensão para a fita LED;
- uso de fita LED 3528 de 12 V;
- controle por PWM via MOSFET IRLZ44N;
- ausência inicial de modo sleep obrigatório;
- foco em autonomia suficiente, e não em autonomia máxima.

O Codex não deve transformar o projeto em uma arquitetura complexa, hiperotimizada ou distante da proposta didática e replicável.

---

## 4. Objetivo do projeto

Desenvolver o firmware Arduino e a documentação técnica inicial da LumenBABY, permitindo validar o funcionamento da luminária com:

- modo automático por luminosidade;
- transições suaves de fade in e fade out;
- modo respiração;
- controle manual de brilho;
- operação por bateria interna;
- controle PWM da fita LED;
- uso simples por meio de dois botões e uma chave liga/desliga.

---

## 5. Hardware previsto

### Controle

- Arduino Pro Mini 5V / 16 MHz.

### Entradas

- 1 LDR;
- resistor para divisor de tensão do LDR;
- 2 botões push-button.

### Saída de iluminação

- fita LED 3528 de 12 V, aproximadamente 1,2 m;
- MOSFET IRLZ44N;
- resistor de gate entre 100 Ω e 220 Ω;
- resistor pull-down de 10 kΩ.

### Alimentação

- 2 baterias 18650 em paralelo, formando sistema 1S2P;
- TP4056 com proteção;
- MT3608 ajustado para aproximadamente 10 V;
- chave liga/desliga;
- capacitor eletrolítico de 100 µF;
- capacitor cerâmico de 100 nF.

---

## 6. Modos de funcionamento

### Modo automático

O sistema deve ler o LDR e identificar quando o ambiente escureceu.

Quando o ambiente estiver abaixo do limite definido, a luminária deve acender com fade in suave.

Quando o ambiente voltar a ficar claro, a luminária deve apagar com fade out suave.

### Modo respiração

A intensidade da luz deve variar lentamente em ciclos suaves, simulando um efeito de respiração.

### Modo manual

O usuário deve poder selecionar níveis de brilho pré-definidos.

Sugestão inicial:

- brilho baixo;
- brilho médio;
- brilho alto.

---

## 7. Interface de usuário

A versão inicial deve utilizar:

- 1 chave física liga/desliga geral;
- 1 botão para alternar modos;
- 1 botão para alterar níveis de brilho.

Sugestão inicial de modos:

1. Automático;
2. Respiração;
3. Manual.

O firmware deve usar debounce simples para evitar comandos acidentais.

---

## 8. Requisitos funcionais

O firmware deve:

1. Inicializar o sistema em estado seguro.
2. Ler o sensor LDR.
3. Controlar a fita LED por PWM.
4. Alternar entre automático, respiração e manual.
5. Aplicar fade in e fade out suaves.
6. Permitir ajuste de brilho manual.
7. Evitar travamentos usando lógica simples.
8. Ser escrito em Arduino C/C++.
9. Ter comentários claros e didáticos.
10. Evitar bibliotecas desnecessárias.

---

## 9. Fora de escopo nesta fase

Não fazem parte desta fase:

- Wi-Fi;
- Bluetooth;
- aplicativo móvel;
- controle remoto;
- display;
- sensor PIR;
- integração com assistentes virtuais;
- monitoramento de bebê;
- conexão com internet;
- PCB própria;
- modo sleep obrigatório;
- ESP32;
- Raspberry Pi Pico;
- qualquer plataforma diferente de Arduino.

---

## 10. Cuidados técnicos mínimos

Mesmo seguindo a filosofia de simplicidade, o projeto deve respeitar cuidados básicos:

- evitar curto-circuito;
- manter MOSFET desligado durante inicialização;
- usar pull-down no gate do MOSFET;
- evitar flicker perceptível nos LEDs;
- testar aquecimento do MT3608;
- testar aquecimento do TP4056 durante recarga;
- testar aquecimento da fita LED;
- testar autonomia real;
- testar comportamento durante carga;
- manter baterias bem fixadas;
- isolar soldas expostas;
- documentar decisões relevantes.

---

## 11. Repositório público

O projeto será publicado em repositório público no GitHub.

Por isso, o Codex deve evitar:

- dados pessoais;
- credenciais;
- tokens;
- senhas;
- custos internos sensíveis;
- informações de clientes;
- notas fiscais;
- documentos privados;
- arquivos temporários;
- backups desnecessários.

---

## 12. Orientação obrigatória para o Codex

Antes de gerar qualquer código, o Codex deve respeitar este documento como fonte principal de contexto.

O Codex deve implementar o projeto de forma incremental, começando por um firmware simples, legível e funcional.

O Codex não deve propor troca de plataforma, uso de Wi-Fi, Bluetooth, aplicativo, PCB própria ou arquitetura complexa nesta fase.

Sempre que alterar algo relevante, o Codex deve manter a documentação coerente.
