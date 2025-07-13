# Jogo Stock Car

Um jogo de corrida estilo arcade implementado para microcontrolador 8051 com display LCD gráfico.

## Descrição

Jogo simples onde o jogador controla um carro em uma estrada com curvas, desviando de obstáculos para conseguir a maior pontuação possível. O jogo possui 3 vidas e sistema de pontuação.

## Hardware Necessário

- **Microcontrolador**: 8051 (REGX52)
- **Display**: LCD gráfico 128x64 pixels (controlador KS0108)
- **Controles**: 2 botões push-button
- **Conexões**:
  - Barramento de dados LCD: P1
  - Sinais de controle LCD: P2
  - Botões: P0.0 (esquerda) e P0.1 (direita)

## Pinagem

### LCD (P2)
- `P2.0` - RS
- `P2.1` - RW  
- `P2.2` - EN
- `P2.3` - CS1
- `P2.4` - CS2
- `P2.5` - RST

### Controles (P0)
- `P0.0` - Botão esquerda
- `P0.1` - Botão direita

## Como Jogar

1. **Iniciar**: Pressione qualquer botão para começar
2. **Mover**: Use os botões para mover o carro para esquerda/direita
3. **Objetivo**: Desvie dos obstáculos e faça pontos
4. **Vidas**: Você tem 3 vidas, perde uma a cada colisão
5. **Fim de jogo**: Quando acabam as vidas, mostra a pontuação final

## Arquivos do Projeto

```
stock-car/
├── main.c          # Loop principal do jogo
├── game_logic.c    # Lógica e mecânicas do jogo
├── game_logic.h    # Constantes e declarações
├── glcd.c          # Driver do LCD
├── glcd.h          # Interface do LCD
├── bitmaps.c       # Bitmaps dos sprites
├── bitmaps.h       # Declarações dos bitmaps
├── utils.c         # Funções auxiliares
├── utils.h         # Declarações das funções auxiliares
└── font_header.h   # Dados da fonte
```

## Sobre

Projeto desenvolvido para a disciplina "Sistemas Computacionais Embarcados" do Departamento de Computação - UFSCar, ministrada pelo Prof. Emerson Carlos Pedrino.