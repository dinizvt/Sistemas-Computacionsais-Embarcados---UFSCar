#include <reg51.h>
#include <stdlib.h>
#include "lcd.h"
#include "keypad.h"
#include "shared.h"

#define DINO_UP 1
#define DINO_DOWN 0 

#define DINO    '$'
#define CACTUS  '@'
#define BIRD    '#'
#define LCD_LEN 16

int DINO_STATUS = DINO_DOWN;

// Estrutura que contem as strings da linha de cima e de baixo do LCD
typedef struct {
    char top[LCD_LEN + 1];     // Linha de cima (+1 para terminador nulo)
    char bottom[LCD_LEN + 1];  // Linha de baixo (+1 para terminador nulo)
} GameString;

// Inicializa o jogo: Dino no canto inferior esquerdo e o resto vazio
void initGame(GameString *g) {
	int i;
    g->top[0] = ' ';
    g->bottom[0] = DINO;
    for (i = 1; i < LCD_LEN; i++) {
        g->top[i] = ' ';
        g->bottom[i] = ' ';
    }
    g->top[LCD_LEN] = '\0';
    g->bottom[LCD_LEN] = '\0';
}

// Atualiza o estado do jogo: desloca para a esquerda e adiciona novo item
// Retorna 1 se houve colisao (fim de jogo), 0 caso contrario
int updateGame(GameString *g, char newItem, int row) {
	int i;
	
    // Verifica colisao
    if ((DINO_STATUS == DINO_DOWN && g->bottom[1] == CACTUS) ||
        (DINO_STATUS == DINO_UP && g->top[1] == BIRD)) {
        return 1; // Fim de jogo
    }

    // Desloca ambas as linhas para a esquerda
    for (i = 0; i < LCD_LEN - 1; i++) {
        g->top[i] = g->top[i + 1];
        g->bottom[i] = g->bottom[i + 1];
    }

    // Posiciona o Dino na nova posicao
    g->top[LCD_LEN - 1] = ' ';
    g->bottom[LCD_LEN - 1] = ' ';
    if (DINO_STATUS == DINO_DOWN)
        g->bottom[0] = DINO;
    else
        g->top[0] = DINO;

    // Insere o novo item na linha correta
    if (row == 1)
        g->top[LCD_LEN - 1] = newItem;
    else
        g->bottom[LCD_LEN - 1] = newItem;

    return 0; // Continua o jogo
}

// Alterna a posicao do Dino (pulo)
int toggleDinoStatus(void) {
    DINO_STATUS = !DINO_STATUS;
    return DINO_STATUS;
}

// Loop principal do jogo
void main(void) {
    char key;
		int d;
		int iter = 0;
    int randRow, randItem;
    int gameOver = 0;
    GameString game;

    // Configuracao do LCD
    initLCD();
    initGame(&game);

    while (!gameOver) {
		// Exibe o estado atual do jogo
        lcdCommand(0x80);
        sendString(game.top);
        goToPosition(1,0);
        sendString(game.bottom);

        // Verifica entrada de pulo
        key = getKey();
        if (key != 0) {
            toggleDinoStatus();
        }

        // Gera obstaculo aleatorio
				if (++iter % 10 == 0)
					gameOver = ((iter/10) % 2) ? updateGame(&game, CACTUS, 0) : updateGame(&game, BIRD, 1);
				else
					gameOver = updateGame(&game, ' ', 0);
			
        for (d = 0; d < 30; d++) {
						delay_ms(1);
						key = getKey();
						if (key != 0) {
								toggleDinoStatus();
						}
				}
    }

    // Tela de fim de jogo
    clearDisplay();
    sendString("Game Over");
    goToPosition(0,1);

    // Aguarda qualquer tecla para sair
    while (1) {
        if (getKey() != 0) {
            break;
        }
    }
}
