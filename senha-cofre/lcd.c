#include <reg51.h>
#include "lcd.h"
#include "shared.h"

// Definicoes dos pinos do LCD conectados a Porta P1
sbit DB7 = P1^7;
sbit DB6 = P1^6;
sbit DB5 = P1^5;
sbit DB4 = P1^4;
sbit RS  = P1^3; // Register Select (0=Command, 1=Data)
sbit E   = P1^2; // Enable

// Funcao auxiliar para enviar um nibble (4 bits) ao LCD
void sendNibble(char nibble, bit rs_val) {
    RS = rs_val; // Define se eh comando ou dado

    // Coloca os bits do nibble nos pinos de dados do LCD
    DB7 = (nibble & 0x08) ? 1 : 0; // Bit 3 do nibble para DB7
    DB6 = (nibble & 0x04) ? 1 : 0; // Bit 2 do nibble para DB6
    DB5 = (nibble & 0x02) ? 1 : 0; // Bit 1 do nibble para DB5
    DB4 = (nibble & 0x01) ? 1 : 0; // Bit 0 do nibble para DB4

    E = 1; // Pulsa o pino Enable para o LCD ler
    delay_ms(1); // Pequeno atraso para estabilizacao
    E = 0;
    delay_ms(1); // Pequeno atraso apos o pulso
}

// Funcao para enviar um comando ao LCD (8 bits, enviado em 2 nibbles)
void sendCommand(char cmd) {
    sendNibble(cmd >> 4, 0); // Envia os 4 bits mais significativos (MSB) como comando
    sendNibble(cmd & 0x0F, 0); // Envia os 4 bits menos significativos (LSB) como comando
    delay_ms(2); // Atraso necessario apos o comando
}

// Funcao para enviar um caractere ao LCD (8 bits, enviado em 2 nibbles)
void sendChar(char c) {
    sendNibble(c >> 4, 1); // Envia os 4 bits mais significativos (MSB) como dado
    sendNibble(c & 0x0F, 1); // Envia os 4 bits menos significativos (LSB) como dado
    delay_ms(1); // Atraso necessario apos o dado
}

// Funcao para enviar uma string ao LCD
void sendString(char* str) {
    while(*str != '\0') { // Percorre a string ate o caractere nulo
        sendChar(*str); // Envia cada caractere
        str++;
    }
}

// Funcao para retornar o cursor para a posicao inicial (0,0)
void returnHome(void) {
    sendCommand(0x02); // Comando Home
}

// Funcao para configurar o modo de entrada (incremento/decremento do cursor, shift do display)
void entryModeSet(bit id, bit s) {
    char cmd = 0x04; // Base do comando Entry Mode Set
    if (id) cmd |= 0x02; // I/D: Incrementa cursor se 1, decrementa se 0
    if (s)  cmd |= 0x01; // S: Shift display se 1
    sendCommand(cmd);
}

// Funcao para controlar ligar/desligar o display, cursor e piscar
void displayOnOffControl(bit display, bit cursor, bit blinking) {
    char cmd = 0x08; // Base do comando Display On/Off Control
    if (display) cmd |= 0x04; // D: Display On/Off
    if (cursor)  cmd |= 0x02; // C: Cursor On/Off
    if (blinking) cmd |= 0x01; // B: Blinking On/Off
    sendCommand(cmd);
}

// Funcao de inicializacao do LCD (modo de 4 bits, numero de linhas, tamanho da fonte)
void functionSet(void) {
    // Sequencia de inicializacao para modo 4-bits
    // Primeira parte (envio de nibble)
    RS = 0; // Modo comando
    E = 0;
    DB7 = 0; DB6 = 0; DB5 = 1; DB4 = 0; // Envia 0x02 (para configurar o modo 4-bits)
    E = 1; // Pulsa Enable
    delay_ms(1);
    E = 0;
    delay_ms(5); // Atraso longo para primeira inicializacao

    // Segunda parte (envio de Function Set completo)
    // DL=0 (4-bit), N=1 (2-lines), F=0 (5x8 dots) -> 0x28
    sendCommand(0x28);
    delay_ms(1); // Pequeno atraso

    // Outros comandos de inicializacao padrao
    displayOnOffControl(1, 0, 0); // Display ON, Cursor OFF, Blinking OFF
    clearDisplay();               // Limpa o display
    entryModeSet(1, 0);           // Incrementa cursor, Sem Shift do display
}

// Funcao para limpar o display
void clearDisplay(void) {
    sendCommand(0x01); // Comando Clear Display
}

// Funcao para definir o endereco da memoria DDRAM (posicao do cursor)
void setDdRamAddress(char address) {
    sendCommand(0x80 | address); // Comando Set DDRAM Address
}

// Funcao auxiliar para obter um bit especifico de um caractere (nao utilizada no LCD driver atual mas util)
bit getBit(char c, char bitNumber) {
    return (c >> bitNumber) & 0x01;
}