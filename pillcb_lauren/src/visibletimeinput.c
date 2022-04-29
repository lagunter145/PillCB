#include "lcd.h";

void drawTimeInput(int time)
{
    char s[10];
    int hrs1 = time / 100000;
    int hrs2 = time / 10000 % 10;
    int mins1 = time / 1000 % 10;
    int mins2 = time / 100 % 10;
    int secs1 = time / 10 % 10;
    int secs2 = time / 1 % 10;

    sprintf(s, "%d", hrs1);
    LCD_DrawString(90,100, WHITE, BLACK, s, 16, 0);
    sprintf(s, "%d", hrs2);
    LCD_DrawString(90,105, WHITE, BLACK, s, 16, 0);
    sprintf(s, "%d", mins1);
    LCD_DrawString(90,110, WHITE, BLACK, s, 16, 0);
    sprintf(s, "%d", mins2);
    LCD_DrawString(90,115, WHITE, BLACK, s, 16, 0);
    sprintf(s, "%d", secs1);
    LCD_DrawString(90,120, WHITE, BLACK, s, 16, 0);
    sprintf(s, "%d", secs2);
    LCD_DrawString(90,125, WHITE, BLACK, s, 16, 0);
}
