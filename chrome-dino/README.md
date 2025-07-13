# Chrome Dino Game

Implementação do famoso jogo do dinossauro do Google Chrome para microcontrolador 8051 com display LCD 16x2 e teclado matricial.

## Descrição

Jogo de plataforma onde o jogador controla um dinossauro que deve pular obstáculos em uma corrida infinita. O objetivo é conseguir a maior pontuação possível evitando colidir com os obstáculos que aparecem aleatoriamente.

## Hardware Necessário

- **Microcontrolador**: 8051
- **Display**: LCD 16x2 (modo 4 bits)
- **Teclado**: Matricial para controle
- **Timer**: Timer0 para geração de seed aleatório

## Características do Jogo

- **Personagem**: Dinossauro com animação de corrida e pulo
- **Obstáculos**: Cactos de diferentes tipos e altura
- **Pontuação**: Sistema crescente baseado na distância percorrida
- **Dificuldade**: Aumenta progressivamente com a velocidade
- **Caracteres customizados**: Sprites definidos para LCD

## Como Jogar

1. **Iniciar**: O jogo inicia automaticamente
2. **Pular**: Pressione qualquer tecla para fazer o dinossauro pular
3. **Objetivo**: Evite os obstáculos saltando sobre eles
4. **Pontuação**: Ganha pontos continuamente enquanto corre
5. **Game Over**: Quando colidir com um obstáculo

## Interface de Jogo

- **Linha superior**: Pontuação atual + dinossauro/obstáculos
- **Linha inferior**: Chão + base do dinossauro
- **Caracteres especiais**: Sprites customizados para elementos do jogo

## Sistema de Obstáculos

- **Tipos**: Diferentes formatos de cactos
- **Frequência**: Baseada em cooldown variável
- **Velocidade**: Aumenta conforme a pontuação
- **Detecção de colisão**: Sistema preciso de verificação

## Arquivos do Projeto

```
chrome-dino/
├── main.c      # Loop principal e controle do jogo
├── game.c      # Lógica do jogo e mecânicas
├── game.h      # Definições e estruturas do jogo
├── lcd.c       # Driver do LCD e caracteres customizados
├── lcd.h       # Interface do LCD
├── keypad.c    # Controle do teclado
├── keypad.h    # Interface do teclado
├── shared.c    # Funções utilitárias compartilhadas
└── shared.h    # Definições compartilhadas
```

## Estruturas de Dados

### GameString
Estrutura que mantém o estado visual do jogo:
- `top[LCD_LEN]` - Linha superior do display
- `bottom[LCD_LEN]` - Linha inferior do display

## Características Técnicas

- **Velocidade inicial**: Configurável via `INITIAL_GAME_FREQ`
- **Geração aleatória**: Baseada em Timer0 para seed
- **Animação**: Sistema de frames para movimentação
- **Otimização**: Uso eficiente de memória para 8051

## Mecânicas de Jogo

- **Pulo**: Controle por tecla com debounce
- **Física simples**: Controle de tempo de pulo
- **Progressão**: Aumento gradual de dificuldade
- **Spawn de obstáculos**: Sistema de cooldown inteligente

