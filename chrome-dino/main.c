#include <reg51.h>
#include <stdlib.h>
#include <string.h>
#include "lcd.h"
#include "keypad.h"
#include "game.h"
#include "shared.h"

// Higher = slower
#define INITIAL_GAME_FREQ 200

void main(void) {
   char key;
   unsigned int updateDelayCounter;
   unsigned char gameOver = 0;
   unsigned short gameUpdateFreq = INITIAL_GAME_FREQ;
   int score = 0;
   int jumpCounter = 0;
   char scoreStr[6];
   int obstacleCooldown = 0;
   short scorePos;
   unsigned char obstacleType;
   char obstacle;
   char row;
   GameString game;
   unsigned int seed;

   // Generate random seed
   TMOD |= 0x01;      
   TR0 = 1;           
   delay_ms(10);      
   seed = (TH0 << 8) | TL0;
   srand(seed);       
   TR0 = 0;           

   lcdInit();
   initGame(&game);
   defineCustomChars();
   
   while (!gameOver) {
      intToStr(score, scoreStr);
      scorePos = LCD_LEN - strlen(scoreStr);
      sendStringXY(0, scorePos, scoreStr);
      sendStringXY(0, 0, game.top);
      sendStringXY(0, scorePos, scoreStr);
      sendStringXY(1, 0, game.bottom);
      
      for (updateDelayCounter = 0; updateDelayCounter < gameUpdateFreq; updateDelayCounter++) {
	 key = getKey();
	 if ((jumpCounter == 0) && (key != 0)) {
	    gameOver = setDinoStatus(&game, DINO_UP);
	    sendStringXY(0, 0, DINO);
	    sendStringXY(1, 0, " ");
	    jumpCounter = gameUpdateFreq*3;
	 }
	 else if (jumpCounter==1) {
	    gameOver = setDinoStatus(&game, DINO_DOWN);
	    sendStringXY(0,0," ");
	    sendStringXY(1,0,DINO);
	    jumpCounter = 0;       
	 }
	 else if (jumpCounter > 1) {
	    jumpCounter--;
	 }
      }
      if (!gameOver) {
	 if (obstacleCooldown <= 0) {
	    obstacleType = rand() % 2;
	    if (obstacleType == 0 || score <= 100) {
		obstacle = CACTUS; // Cactus char code
		row = 1;       // Bottom row
	    } else {
		obstacle = BIRD; // Bird char code
		row = 0;      // Top row
	    }
            gameOver = updateGame(&game, obstacle, row);
            obstacleCooldown = (rand() % 11) + 5; // 5-15 updates cooldown
	 }
	 else {
	    obstacleCooldown--;
	    gameOver = updateGame(&game, ' ', 0);
	 }
	 if (score % 50 == 0) gameUpdateFreq = (gameUpdateFreq*70)/100;
	 score++;
      }
   }
   
   clearDisplay();
   sendString("Game Over");

   while (1) {
      if (getKey() != 0) break;
   }
}