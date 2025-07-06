#include <reg51.h>
#include "lcd.h"
#include "keypad.h"
#include "shared.h"

// Senha predefinida para o sistema
const char SENHA[4] = "1234";

// Funcao para verificar se a senha inserida esta correta
int checkPasswd (char* passwdInput, int passwdSize) {
    int i;
    for (i = 0; i < passwdSize; i++) {
        if (passwdInput[i] != SENHA[i]) {
            // Se encontrar qualquer caractere diferente, a senha esta incorreta
            return 0;
        }
    }
    return 1; // Se todos os caracteres correspondem, a senha esta correta
}

// Funcao principal do programa
void main(void) {
    char key;                 // Variavel para armazenar a tecla pressionada
    char passwdInput[4];      // Array para armazenar a senha digitada pelo usuario
    int inputCounter = 0;     // Contador de caracteres inseridos

    // Inicializacao do LCD
    functionSet();           // Configura o LCD (modo 4 bits, 2 linhas, etc.)
    entryModeSet(1, 0);      // Incrementa o cursor apos cada caractere, sem shift do display
    displayOnOffControl(1, 1, 1); // Liga o display, cursor e blinking do cursor

    // Exibe a mensagem inicial no LCD
    sendString("Insira a senha:");
    setDdRamAddress(0x40);   // Move o cursor para a segunda linha do LCD (endereco 0x40)

    // Loop para capturar 4 caracteres da senha
    while (inputCounter < 4) {
        key = getKey();      // Le uma tecla do teclado
        if (key == 0) {      // Se nenhuma tecla foi pressionada, continua o loop
            continue;
        }
        passwdInput[inputCounter] = key; // Armazena a tecla no buffer da senha
        inputCounter++;                  // Incrementa o contador
        sendChar('*');                   // Exibe um asterisco no LCD para ocultar a senha
    }

    // Limpa o display apos a entrada da senha
    clearDisplay();

    // Verifica a senha inserida
    if (checkPasswd(passwdInput, 4)) {
        sendString("Ok"); // Exibe "Ok" se a senha estiver correta
    }
    else {
        sendString("Senha Incorreta"); // Exibe "Senha Incorreta" caso contrario
    }

    // Loop infinito para manter o resultado no display
    while(1);
}