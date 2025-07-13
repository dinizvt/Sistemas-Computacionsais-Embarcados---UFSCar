# Campo Minado

Implementação do clássico jogo Campo Minado para microcontrolador 8051 com display LCD 16x2 e teclado matricial.

## Descrição

Jogo onde o jogador deve encontrar todas as células seguras de um campo 2x16 evitando as bombas escondidas. O jogo utiliza um algoritmo LFSR para gerar posições aleatórias das bombas e interface através de LCD e teclado matricial.

## Hardware Necessário

- **Microcontrolador**: 8051
- **Display**: LCD 16x2 (modo 4 bits)
- **Teclado**: Matricial 4x3
- **Conexões**:
  - LCD: P1 (dados), P1.3 (RS), P1.2 (EN)
  - Teclado: P0 (linhas e colunas)

## Pinagem

### LCD (P1)
- `P1[7:4]` - Barramento de dados (modo 4 bits)
- `P1.3` - RS (Register Select)
- `P1.2` - EN (Enable)

### Teclado Matricial (P0)
- `P0.0` - Linha 1
- `P0.1` - Linha 2
- `P0.2` - Linha 3
- `P0.3` - Linha 4
- `P0.4` - Coluna 1
- `P0.5` - Coluna 2
- `P0.6` - Coluna 3

## Como Jogar

1. **Inicialização**: O jogo gera um campo 2x16 com bombas posicionadas aleatoriamente
2. **Navegação**: Use o teclado para navegar pelo campo
3. **Revelação**: Pressione uma tecla para revelar uma célula
4. **Objetivo**: Encontre todas as células seguras sem acertar uma bomba
5. **Vitória**: Revele todas as células que não contêm bombas
6. **Derrota**: Ao revelar uma bomba, o jogo termina

## Características Técnicas

- **Campo**: 2 linhas x 16 colunas (32 células total)
- **Gerador aleatório**: LFSR (Linear Feedback Shift Register)
- **Interface**: LCD 16x2 para visualização do campo
- **Controle**: Teclado matricial 4x3
- **Símbolos**:
  - `.` - Célula não revelada
  - ` ` - Célula segura revelada
  - `*` - Bomba (quando revelada)
  - Números - Quantidade de bombas adjacentes

## Arquivos do Projeto

```
campo-minado/
└── campo_minado.c    # Implementação completa do jogo
```

## Funcionalidades

- Geração aleatória de bombas usando LFSR
- Interface LCD para visualização do campo
- Controle via teclado matricial
- Sistema de navegação e seleção
- Detecção de vitória/derrota
- Contagem de bombas adjacentes

## Compilação

O projeto foi desenvolvido para ser compilado com Keil C51 ou compiladores compatíveis com 8051.
