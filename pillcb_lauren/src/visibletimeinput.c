/*
 * visibletimeinput.c

 *
 *  Created on: Apr 29, 2022
 *      Author: gunter3
 */
#include "stm32f0xx.h"
#include "lcd.h";

void drawTimeInput(int time)
{
    char s[10];
    int secs2 = time / 100000;
    int secs1 = time / 10000 % 10;
    int mins2 = time / 1000 % 10;
    int mins1 = time / 100 % 10;
    int hrs2 = time / 10 % 10;
    int hrs1 = time / 1 % 10;

    sprintf(s, "%d", secs2);
    LCD_DrawString(95,100, WHITE, BLACK, s, 16, 0);
    sprintf(s, "%d", secs1);
    LCD_DrawString(105,100, WHITE, BLACK, s, 16, 0);
    sprintf(s, "%d", mins2);
    LCD_DrawString(115,100, WHITE, BLACK, s, 16, 0);
    sprintf(s, "%d", mins1);
    LCD_DrawString(125,100, WHITE, BLACK, s, 16, 0);
    sprintf(s, "%d", hrs2);
    LCD_DrawString(135,100, WHITE, BLACK, s, 16, 0);
    sprintf(s, "%d", hrs1);
    LCD_DrawString(145,100, WHITE, BLACK, s, 16, 0);
}

