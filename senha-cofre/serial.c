#include <REGX52.H> // Inclui o cabe�alho principal para o microcontrolador 80C52.

// ===================================
// Defini��es de Hardware do LCD
// ===================================
sfr LCD_Port = 0x90; // Mapeia a porta P1 (0x90) para comunica��o com o LCD.
sbit rs = P1^3;      // Pino RS (Register Select): 0=comando, 1=dado.
sbit rw = P1^1;      // Pino RW (Read/Write): 0=escrita, 1=leitura.
sbit en = P1^2;      // Pino EN (Enable): Pulso para enviar dados/comandos.

// ===================================
// Vari�veis Globais
// ===================================
char cursor_pos = 0; // Controla a posi��o atual do cursor no LCD.

// ===================================
// Fun��es de Utilit�rios
// ===================================
void delay(unsigned int t) {
    // Fun��o de atraso b�sica.
    unsigned int i, j;
    for (i = 0; i < t; i++) {
        for (j = 0; j < 120; j++); // Loop para gerar o atraso.
    }
}

// ===================================
// Fun��es da UART
// ===================================
void UART_Init(void) {
    // Configura e inicia a comunica��o serial (UART).
    SCON = 0x50;    // Modo 1 (8 bits, vari�vel baud rate), habilita recep��o.
    TMOD |= 0x20;   // Timer 1 no Modo 2 (auto-recarregamento) para o baud rate.
    TH1 = 0xFD;     // Define o valor para 9600 bps.
    TL1 = 0xFD;     // Define o valor para 9600 bps.
    TR1 = 1;        // Inicia o Timer 1.
    TI = 1;         // Prepara para a primeira transmiss�o.
}

char UART_ReadChar(void) {
    // L� um caractere recebido pela UART.
    while (RI == 0); // Aguarda at� que o caractere seja recebido.
    RI = 0;          // Limpa a flag de recep��o.
    return SBUF;     // Retorna o caractere do buffer.
}

void UART_WriteChar(char c) {
    // Escreve um caractere para ser transmitido pela UART.
    SBUF = c;        // Coloca o caractere no buffer de transmiss�o.
    while (TI == 0); // Aguarda a transmiss�o ser conclu�da.
    TI = 0;          // Limpa a flag de transmiss�o.
}

// ===================================
// Fun��es do LCD
// ===================================
void LCD_Cmd(char cmd) {
    // Envia um comando de 8 bits para o LCD (modo 4 bits).
    LCD_Port = (LCD_Port & 0x0F) | (cmd & 0xF0); // Envia os 4 bits altos.
    rs = 0; rw = 0; en = 1; delay(1); en = 0;    // Pulso EN.

    LCD_Port = (LCD_Port & 0x0F) | (cmd << 4);  // Envia os 4 bits baixos.
    en = 1; delay(1); en = 0;                    // Pulso EN.
    delay(2); // Atraso para processamento.
}

void LCD_SetCursor(char pos) {
    // Posiciona o cursor no display LCD.
    if (pos < 16) {
        LCD_Cmd(0x80 + pos);       // Primeira linha.
    } else {
        LCD_Cmd(0xC0 + (pos - 16)); // Segunda linha.
    }
}

void LCD_Char(char char_data) {
    // Escreve um caractere no LCD na posi��o atual.
    LCD_SetCursor(cursor_pos); // Define a posi��o.

    LCD_Port = (LCD_Port & 0x0F) | (char_data & 0xF0); // Envia os 4 bits altos dos dados.
    rs = 1; rw = 0; en = 1; delay(1); en = 0;    // Pulso EN (modo dado).

    LCD_Port = (LCD_Port & 0x0F) | (char_data << 4); // Envia os 4 bits baixos dos dados.
    en = 1; delay(1); en = 0;                    // Pulso EN.
    delay(2); // Atraso.

    cursor_pos++; // Avan�a o cursor.

    // Reinicia e limpa o display se o final for atingido.
    if (cursor_pos >= 32) {
        cursor_pos = 0;
        LCD_Cmd(0x01); // Limpa o display.
        delay(2);
    }
}

void LCD_Init() {
    // Inicializa o display LCD para opera��o.
    delay(20);      // Atraso de inicializa��o.
    LCD_Cmd(0x02);  // Retorna para o in�cio.
    LCD_Cmd(0x28);  // Configura��o: 4 bits, 2 linhas, 5x8.
    LCD_Cmd(0x0C);  // Display ON, Cursor OFF, Blink OFF.
    LCD_Cmd(0x06);  // Modo de entrada: incrementar, sem shift.
    LCD_Cmd(0x01);  // Limpa o display.
    delay(2);
    cursor_pos = 0; // Reinicia a posi��o do cursor.
}

// ===================================
// Fun��o Principal
// ===================================
void main() {
    char r; // Vari�vel para o caractere recebido.

    UART_Init(); // Inicializa a UART.
    LCD_Init();  // Inicializa o LCD.

    while (1) { // Loop infinito do programa.
        r = UART_ReadChar(); // L� um caractere.

        // Se for um caractere imprim�vel, ecoa e exibe no LCD.
        if (r >= 0x20 && r <= 0x7E) {
            UART_WriteChar(r); // Ecoa o caractere.
            LCD_Char(r);       // Exibe no LCD.
        }
    }
}