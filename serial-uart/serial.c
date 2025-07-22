#include <REGX52.H> // Inclui o cabeçalho principal para o microcontrolador 80C52.

// ===================================
// Definições de Hardware do LCD
// ===================================
sfr LCD_Port = 0x90; // Mapeia a porta P1 (0x90) para comunicação com o LCD.
sbit rs = P1^3;      // Pino RS (Register Select): 0=comando, 1=dado.
sbit rw = P1^1;      // Pino RW (Read/Write): 0=escrita, 1=leitura.
sbit en = P1^2;      // Pino EN (Enable): Pulso para enviar dados/comandos.

// ===================================
// Variáveis Globais
// ===================================
char cursor_pos = 0; // Controla a posição atual do cursor no LCD.

// ===================================
// Funções de Utilitários
// ===================================
void delay(unsigned int t) {
    // Função de atraso básica.
    unsigned int i, j;
    for (i = 0; i < t; i++) {
        for (j = 0; j < 120; j++); // Loop para gerar o atraso.
    }
}

// ===================================
// Funções da UART
// ===================================
void UART_Init(void) {
    // Configura e inicia a comunicação serial (UART).
    SCON = 0x50;    // Modo 1 (8 bits, variável baud rate), habilita recepção.
    TMOD |= 0x20;   // Timer 1 no Modo 2 (auto-recarregamento) para o baud rate.
    TH1 = 0xFD;     // Define o valor para 9600 bps.
    TL1 = 0xFD;     // Define o valor para 9600 bps.
    TR1 = 1;        // Inicia o Timer 1.
    TI = 1;         // Prepara para a primeira transmissão.
}

char UART_ReadChar(void) {
    // Lê um caractere recebido pela UART.
    while (RI == 0); // Aguarda até que o caractere seja recebido.
    RI = 0;          // Limpa a flag de recepção.
    return SBUF;     // Retorna o caractere do buffer.
}

void UART_WriteChar(char c) {
    // Escreve um caractere para ser transmitido pela UART.
    SBUF = c;        // Coloca o caractere no buffer de transmissão.
    while (TI == 0); // Aguarda a transmissão ser concluída.
    TI = 0;          // Limpa a flag de transmissão.
}

// ===================================
// Funções do LCD
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
    // Escreve um caractere no LCD na posição atual.
    LCD_SetCursor(cursor_pos); // Define a posição.

    LCD_Port = (LCD_Port & 0x0F) | (char_data & 0xF0); // Envia os 4 bits altos dos dados.
    rs = 1; rw = 0; en = 1; delay(1); en = 0;    // Pulso EN (modo dado).

    LCD_Port = (LCD_Port & 0x0F) | (char_data << 4); // Envia os 4 bits baixos dos dados.
    en = 1; delay(1); en = 0;                    // Pulso EN.
    delay(2); // Atraso.

    cursor_pos++; // Avança o cursor.

    // Reinicia e limpa o display se o final for atingido.
    if (cursor_pos >= 32) {
        cursor_pos = 0;
        LCD_Cmd(0x01); // Limpa o display.
        delay(2);
    }
}

void LCD_Init() {
    // Inicializa o display LCD para operação.
    delay(20);      // Atraso de inicialização.
    LCD_Cmd(0x02);  // Retorna para o início.
    LCD_Cmd(0x28);  // Configuração: 4 bits, 2 linhas, 5x8.
    LCD_Cmd(0x0C);  // Display ON, Cursor OFF, Blink OFF.
    LCD_Cmd(0x06);  // Modo de entrada: incrementar, sem shift.
    LCD_Cmd(0x01);  // Limpa o display.
    delay(2);
    cursor_pos = 0; // Reinicia a posição do cursor.
}

// ===================================
// Função Principal
// ===================================
void main() {
    char r; // Variável para o caractere recebido.

    UART_Init(); // Inicializa a UART.
    LCD_Init();  // Inicializa o LCD.

    while (1) { // Loop infinito do programa.
        r = UART_ReadChar(); // Lê um caractere.

        // Se for um caractere imprimível, ecoa e exibe no LCD.
        if (r >= 0x20 && r <= 0x7E) {
            UART_WriteChar(r); // Ecoa o caractere.
            LCD_Char(r);       // Exibe no LCD.
        }
    }
}
