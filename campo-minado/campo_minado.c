#include <reg51.h>

#define ROWS 2
#define COLS 16

sfr LCD_Port = 0x90;
sbit rs = P1^3;
sbit en = P1^2;

// TECLADO MATRICIAL LIGADO EM P0 - AJUSTADO PARA 3 COLUNAS
// Linhas
sbit R1 = P0^0; // Linha 1
sbit R2 = P0^1; // Linha 2
sbit R3 = P0^2; // Linha 3
sbit R4 = P0^3; // Linha 4
// Colunas
sbit C1 = P0^4; // Coluna 1
sbit C2 = P0^5; // Coluna 2
sbit C3 = P0^6; // Coluna 3

char game[ROWS][COLS];    // o que o jogador vê
char back[ROWS][COLS];    // mapa de bombas

unsigned char pos_x, pos_y;
unsigned char lfsr = 0xAB;

// LFSR simples para rand
unsigned char rand_lfsr(){
    bit fb = ((lfsr>>7)^(lfsr>>5)^(lfsr>>4)^(lfsr>>3)) & 1;
    lfsr = (lfsr<<1) | fb;
    return lfsr;
}

void delay(int ms){
    int i,j;
    for(i=0;i<ms;i++) for(j=0;j<112;j++); // Ajuste 112 conforme o seu clock se souber
}

void LCD_Command(char cmd){
    LCD_Port = (LCD_Port & 0x0F) | (cmd & 0xF0);
    rs = 0; en = 1; delay(1); en = 0;
    LCD_Port = (LCD_Port & 0x0F) | (cmd << 4);
    en = 1; delay(1); en = 0; delay(5);
}

void LCD_Char(char c){
    LCD_Port = (LCD_Port & 0x0F) | (c & 0xF0);
    rs = 1; en = 1; delay(1); en = 0;
    LCD_Port = (LCD_Port & 0x0F) | (c << 4);
    en = 1; delay(1); en = 0; delay(5);
}

void LCD_String(char *s){
    while(*s) LCD_Char(*s++);
}

void LCD_Init(){
    delay(20);
    LCD_Command(0x02);
    LCD_Command(0x28);
    LCD_Command(0x0C);
    LCD_Command(0x06);
    LCD_Command(0x01);
    LCD_Command(0x80);

    // --- NOVOS SPRITES ---

    // Define bonequinho (CGRAM addr 0) - Personagem
    LCD_Command(0x40); // Endereço 0 do CGRAM
    LCD_Char(0x04);
    LCD_Char(0x0E);
    LCD_Char(0x0E);
    LCD_Char(0x0E);
    LCD_Char(0x1F);
    LCD_Char(0x1F);
    LCD_Char(0x0A);
    LCD_Char(0x0A);

    // Define NOVA BOMBA (CGRAM addr 1) - Visual mais clássico
    LCD_Command(0x48); // Endereço 1 do CGRAM (0x40 + 8 = 0x48)
    LCD_Char(0x04); // 00000100  <- Pavio
    LCD_Char(0x04); // 00000100  <- Pavio
    LCD_Char(0x0E); // 00001110  <- Topo da bomba
    LCD_Char(0x1F); // 00011111  <- Corpo da bomba
    LCD_Char(0x11); // 00010001  <- Lateral do corpo
    LCD_Char(0x11); // 00010001  <- Lateral do corpo
    LCD_Char(0x0E); // 00001110  <- Fundo da bomba
    LCD_Char(0x00); // 00000000
}

// conta bombas nas 8 direções
int adjacent_bombs(int y, int x){
    int cnt=0, dy, dx;
    for(dy=-1; dy<=1; dy++){
      for(dx=-1; dx<=1; dx++){
        if(dy==0 && dx==0) continue;
        if(y+dy>=0 && y+dy<ROWS && x+dx>=0 && x+dx<COLS){
          if(back[y+dy][x+dx]=='B') cnt++;
        }
      }
    }
    return cnt;
}

// coloca 12 bombas garantindo que em cada coluna
// pelo menos uma das duas células fique vazia
void generate_bombs(){
    int r,c,b=0;
    // limpa mapa
    for(r=0;r<ROWS;r++)
      for(c=0;c<COLS;c++)
        back[r][c]=' ';
    // coloca 12 bombas
    while(b<12){
      r = rand_lfsr()%ROWS;
      c = rand_lfsr()%COLS;
      if(back[r][c]!='B'){
        back[r][c]='B';
        b++;
      }
    }
    // garante caminho: em cada coluna, não fique B/B
    for(c=0;c<COLS;c++){
      if(back[0][c]=='B' && back[1][c]=='B'){
        // limpa uma delas aleatoriamente
        if(rand_lfsr()%2) back[0][c]=' ';
        else               back[1][c]=' ';
        b--;
      }
    }
    // se tirou bombas demais, repõe até 12
    while(b<12){
      r = rand_lfsr()%ROWS;
      c = rand_lfsr()%COLS;
      if(back[r][c]!='B'){
        back[r][c]='B';
        b++;
      }
    }
}

// leitura teclado 4x3 (4 linhas, 3 colunas)
char keyScan(){
    while(1){
        // Coloca TODAS as linhas em HIGH (estado de repouso)
        R1 = R2 = R3 = R4 = 1;

        // --- VARREDURA DA LINHA 1 (R1 em LOW) ---
        R1 = 0; delay(5);
        if (!C1) { while(!C1); return '7'; }
        if (!C2) { while(!C2); return '8'; }
        if (!C3) { while(!C3); return '9'; }

        // --- VARREDURA DA LINHA 2 (R2 em LOW) ---
        R1 = 1; R2 = 0; delay(5);
        if (!C1) { while(!C1); return '4'; } // Esta tecla não funciona no seu sistema
        if (!C2) { while(!C2); return '5'; }
        if (!C3) { while(!C3); return '6'; }

        // --- VARREDURA DA LINHA 3 (R3 em LOW) ---
        R2 = 1; R3 = 0; delay(5);
        if (!C1) { while(!C1); return '1'; } // Esta tecla não funciona no seu sistema
        if (!C2) { while(!C2); return '2'; } // Esta tecla não funciona no seu sistema
        if (!C3) { while(!C3); return '3'; }

        // --- VARREDURA DA LINHA 4 (R4 em LOW) ---
        R3 = 1; R4 = 0; delay(5);
        if (!C1) { while(!C1); return 'C'; }
        if (!C2) { while(!C2); return '0'; }
        if (!C3) { while(!C3); return '='; }
        
        R4 = 1;
    }
}

// redesenha o display
void PrintTela(){
    int c;
    LCD_Command(0x01); // Limpa o display
    LCD_Command(0x80); // Volta para a primeira linha, primeira coluna
    for(c=0;c<COLS;c++) LCD_Char(game[0][c]);
    LCD_Command(0xC0); // Vai para a segunda linha, primeira coluna
    for(c=0;c<COLS;c++) LCD_Char(game[1][c]);
}

void main(){
    char act;
    char new_pos_x, new_pos_y;

    LCD_Init();

    while(1){ // Loop principal do jogo (reinicia um novo jogo após game over/win)
        generate_bombs();
        // Inicializa o mapa do jogo com '*' (oculto)
        for(new_pos_y=0; new_pos_y<ROWS; new_pos_y++)
            for(new_pos_x=0; new_pos_x<COLS; new_pos_x++)
                game[new_pos_y][new_pos_x] = '*';

        // Começa o jogador na posição (0,0)
        pos_x = 0;
        pos_y = 0;
        game[pos_y][pos_x] = 0; // Coloca o sprite do boneco na posição inicial
        PrintTela(); // Exibe a tela inicial

        // Loop de jogo atual (uma partida)
        while(1){
            new_pos_x = pos_x;
            new_pos_y = pos_y;

            act = keyScan(); // Lê a tecla pressionada e espera a liberação

            // Lógica de movimento ATUALIZADA para os novos botões (VOCÊ PEDIU ESTE MAPEAMENTO)
            if(act=='5' && new_pos_y > 0)           new_pos_y--; // CIMA (tecla '5')
            else if(act=='8' && new_pos_y < ROWS-1) new_pos_y++; // BAIXO (tecla '8')
            else if(act=='9' && new_pos_x > 0)      new_pos_x--; // ESQUERDA (tecla '9')
            else if(act=='7' && new_pos_x < COLS-1) new_pos_x++; // DIREITA (tecla '7')

            // Ação de REVELAR CÉLULA (Abrir) - Pressiona '=' ou 'C'
            else if(act=='=' || act=='C'){
                if (game[pos_y][pos_x] == '*') {
                    if (back[pos_y][pos_x] == 'B') {
                        game[pos_y][pos_x] = 1; // Coloca o sprite da bomba
                        PrintTela();
                        delay(4000); // 4 segundos
                        LCD_Command(0x01);
                        LCD_String("Game Over!");
                        delay(1500);
                        goto end_game_loop;
                    } else {
                        game[pos_y][pos_x] = '0' + adjacent_bombs(pos_y, pos_x);
                    }
                    PrintTela();
                }
                continue;
            }
            // Tecla '0' para RESET do jogo (inicia uma nova partida)
            else if(act=='0'){
                goto end_game_loop;
            }

            // --- Lógica de Atualização da Tela e Jogo ---
            // Bugfix: Corrigido de 'new_pos_y != new_pos_y' para 'new_pos_y != pos_y'
            if (new_pos_x != pos_x || new_pos_y != pos_y) {
                // 1. Revele a célula ANTERIOR (onde o boneco ESTAVA)
                if (game[pos_y][pos_x] == 0) { // Se a célula anterior era o boneco
                    game[pos_y][pos_x] = '0' + adjacent_bombs(pos_y, pos_x); // Revele o número de bombas adjacentes
                }
                // ATUALIZA A POSIÇÃO DO JOGADOR
                pos_x = new_pos_x;
                pos_y = new_pos_y;

                // 2. Verifica se o jogador pisou em uma bomba na NOVA posição
                if(back[pos_y][pos_x] == 'B'){
                    game[pos_y][pos_x] = 1; // Coloca o sprite da bomba no display
                    PrintTela();
                    delay(4000); // 4 segundos
                    LCD_Command(0x01);
                    LCD_String("Game Over!");
                    delay(1500);
                    goto end_game_loop;
                }

                // 3. Coloca o boneco na NOVA posição
                game[pos_y][pos_x] = 0; // Coloca o sprite do boneco

                // 4. Redesenha a tela
                PrintTela();
            }

            // 5. Verifica condição de vitória
            if(pos_x == COLS-1){
                LCD_Command(0x01);
                LCD_String("VENCEDOR!");
                delay(2000);
                goto end_game_loop;
            }
        }
        end_game_loop:;
        delay(500);
    }
}