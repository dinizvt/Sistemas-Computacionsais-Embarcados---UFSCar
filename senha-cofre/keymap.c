#include <reg51.h>
#include "keypad.h"
#include "shared.h"

// Definicoes dos pinos do teclado conectados a Porta P0
// Certifique-se que estas definicoes correspondem a sua fiação física!
sbit ROW1 = P0^3; // Linha 1 do teclado para P0.3
sbit ROW2 = P0^2; // Linha 2 do teclado para P0.2
sbit ROW3 = P0^1; // Linha 3 do teclado para P0.1
sbit ROW4 = P0^0; // Linha 4 do teclado para P0.0
sbit COL1 = P0^6; // Coluna 1 do teclado para P0.6
sbit COL2 = P0^5; // Coluna 2 do teclado para P0.5
sbit COL3 = P0^4; // Coluna 3 do teclado para P0.4


// Matriz de mapeamento de teclas.
// A LINHA deste array corresponde à ROW do teclado, e a COLUNA corresponde à COL do teclado.
// EX: keymap[0][0] é a tecla na ROW1 (P0.3) e COL1 (P0.6)
// VOCÊ PODE PRECISAR AJUSTAR ESTA MATRIZ COM BASE NO SEU TESTE FÍSICO!
const char keymap[4][3] = {
    {'1', '2', '3'}, // Linha 1 (ROW1) do teclado físico
    {'4', '5', '6'}, // Linha 2 (ROW2) do teclado físico
    {'7', '8', '9'}, // Linha 3 (ROW3) do teclado físico
    {'*', '0', '#'}  // Linha 4 (ROW4) do teclado físico
};

// Funcao para ler uma tecla do teclado matricial
char getKey(void) {
    char row_idx; // Indice da linha (0 a 3)

    for(row_idx = 0; row_idx < 4; row_idx++) {
        // Inicializa todas as linhas como HIGH (saída padrão do 8051 ou pull-up)
        // Isso eh crucial para a varredura
        ROW1 = 1; ROW2 = 1; ROW3 = 1; ROW4 = 1;

        // Puxa a linha atual para LOW para detectar a tecla pressionada
        switch(row_idx) {
            case 0: ROW1 = 0; break;
            case 1: ROW2 = 0; break;
            case 2: ROW3 = 0; break;
            case 3: ROW4 = 0; break;
        }

        // Verifica se alguma coluna foi puxada para LOW (tecla pressionada)
        if(COL1 == 0 || COL2 == 0 || COL3 == 0) {
            delay_ms(40);  // Atraso de debounce para evitar leituras falsas

            // Re-verifica para confirmar a tecla (apos o debounce) e identifica a coluna
            if(COL1 == 0) {
                while(COL1 == 0); // Espera a tecla ser liberada
                return keymap[row_idx][0]; // Retorna o caractere mapeado
            }
            if(COL2 == 0) {
                while(COL2 == 0); // Espera a tecla ser liberada
                return keymap[row_idx][1]; // Retorna o caractere mapeado
            }
            if(COL3 == 0) {
                while(COL3 == 0); // Espera a tecla ser liberada
                return keymap[row_idx][2]; // Retorna o caractere mapeado
            }
        }
    }
    return 0; // Retorna 0 se nenhuma tecla foi pressionada
}