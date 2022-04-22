#include "stm32f0xx.h"
#include "lcd.h"

struct Pill{
  int pill_number;
  int secs1;
  int mins1;
  int hrs1;
  int secs2;
  int mins2;
  int hrs2;
};

struct Pill pill1 = { .pill_number =1, .secs1 = 5, .mins1 =3, .hrs1 =1, .secs2 = 6, .mins2 =4, .hrs2 =2};
struct Pill pill2 = { .pill_number =2, .secs1 = 5, .mins1 =3, .hrs1 =1, .secs2 = 6, .mins2 =4, .hrs2 =2};
struct Pill pill3 = { .pill_number =3, .secs1 = 5, .mins1 =3, .hrs1 =1, .secs2 = 6, .mins2 =4, .hrs2 =2};

// time is given as 000000 - split into hrs, mins, secs
int time;
int pill1_time;
int pill2_time;
int pill3_time;

void home_screen(void) {
    // default screen
    LCD_Clear(BLACK);
    // display curr time
    // display countdown to next pill remaining
    // stay on this screen until key_press != -1
    char key_press = get_keypress();
    // switch to general menu
    if(key_press != -1) {
        general_menu();
    }
}

void general_menu(void) {
    LCD_Clear(BLACK);
    LCD_DrawString(20,30, WHITE, WHITE, "A - Pill Menu", 16, 1);
    LCD_DrawString(20,80, WHITE, WHITE, "B - Set Time", 16, 1);
    LCD_DrawString(20,130, WHITE, WHITE, "C - Toggle Speed", 16, 1);
    LCD_DrawString(20,180, WHITE, WHITE, "D - Reset", 16, 1);
    LCD_DrawString(20,230, WHITE, WHITE, "* - Back to Home Screen", 16, 1);
    char key_press = get_keypress();
    // A - pill menu
    if(key_press == 'A') {
        pill_menu();
    }
    // B - set time
    if(key_press == 'B') {
        set_time();
    }
    // C - high speed
    if(key_press == 'C') {
        // high speed function
        /*
        if (fastkeyhit == 0){
            fastkeyhit = 1;
        }
        else {
            fastkeyhit = 0;
        }*/
    }
    // D - reset time
    if(key_press == 'D') {

    }
    // * back to always_on
    if(key_press == '*') {
        home_screen();
    }
}

void pill_menu(void) {
    // pick a pill to view/ edit/ add
    // 1 - pill 1
    // 2 - pill 2
    // 3 - pill 3
    // * - back to gen_menu
    LCD_Clear(BLACK);
    LCD_DrawString(20,30, WHITE, WHITE, "1 - Pill 1", 16, 1);
    LCD_DrawString(20,80, WHITE, WHITE, "2 - Pill 2", 16, 1);
    LCD_DrawString(20,130, WHITE, WHITE, "3 - Pill 3", 16, 1);
    LCD_DrawString(20,180, WHITE, WHITE, "* - Back to General Menu", 16, 1);

    char key_press = get_keypress();
    if(key_press == '1') {
        add_edit_pill(pill1);
    }
    if(key_press == '2') {
        add_edit_pill(pill2);
    }
    if(key_press == '3') {
        add_edit_pill(pill1);
    }
    if(key_press == '*') {
        general_menu();
    }
}
int get_time(void) {
    int time = 0;

    for(;;) {
    char key_press = get_keypress();
    if(key_press == '1') {
        time += 1;
        time *= 10;
    }
    if(key_press == '2') {
        time += 2;
        time *= 10;
    }
    if(key_press == '3') {
        time += 3;
        time *= 10;
    }
    if(key_press == '4') {
        time += 4;
        time *= 10;
    }
    if(key_press == '5') {
        time += 5;
        time *= 10;
    }
    if(key_press == '6') {
        time += 6;
        time *= 10;
    }
    if(key_press == '7') {
        time += 7;
        time *= 10;
    }
    if(key_press == '8') {
        time += 8;
        time *= 10;
    }
    if(key_press == '9') {
        time += 9;
        time *= 10;
    }
    if(key_press == '0') {
        time += 0;
        time *= 10;
    }
    if(key_press == '*') {
        pill_menu();
    }
    if(key_press == 'B')
        break;
    }
    return time;
}
void add_edit_pill(struct Pill p) {
    // shows a pills curr set time (00 00 00 if not added)
    // A - next in time seq
    // B - back in time seq
    // * - back to pill_menu
    LCD_Clear(BLACK);

    char s1[10];

    char s2[10];

    char s3[10];

    char s4[10];

    char s5[10];

    char s6[10];

    sprintf(s1, "%d", p.hrs1);

    sprintf(s2, "%d:", p.hrs2);

    sprintf(s3, "%d", p.mins1);
    sprintf(s4, "%d:", p.mins2);
    sprintf(s5, "%d", p.secs1);
    sprintf(s6, "%d", p.secs2);

    strcat(s1, s2);

    strcat(s1, s3);
    strcat(s1, s4);

    strcat(s1, s5);
    strcat(s1, s6);
    LCD_DrawString(90,100, WHITE, WHITE, s1, 16, 1);

    LCD_DrawString(20,190, WHITE, WHITE, "A - Enter New Pill Reminder", 16, 1);
    //LCD_DrawString(20, 200, WHITE, WHITE, "Enter in format Hours + Minutes + Seconds", 16, 1);
   // LCD_DrawString(20, 210, WHITE, WHITE, "With 0's as Necessary", 16, 1);
    LCD_DrawString(50, 210, WHITE, WHITE, "i.e.; enter 5:30:00 ", 16, 1);
    LCD_DrawString(100, 230, WHITE, WHITE, "as 053000", 16, 1);
    LCD_DrawString(20, 250, WHITE, WHITE, "B - Stop Entering Digits", 16, 1);
    LCD_DrawString(20,280, WHITE, WHITE, "* - Back to Pill Menu", 16, 1);


    char key_press = get_keypress();
    if(key_press == 'A') {
        int newtime = get_time();
        p.hrs1 = newtime / 1000000;
        p.hrs2 = newtime / 100000 % 10;
        p.mins1 = newtime / 10000 % 10;
        p.mins2 = newtime / 1000 % 10;
        p.secs1 = newtime / 100 % 10;
        p.secs2 = newtime / 10 % 10;
        char s1[10];

        char s2[10];

        char s3[10];

        char s4[10];

        char s5[10];

        char s6[10];

        sprintf(s1, "%d", p.hrs1);

        sprintf(s2, "%d:", p.hrs2);

        sprintf(s3, "%d", p.mins1);
        sprintf(s4, "%d:", p.mins2);
        sprintf(s5, "%d", p.secs1);
        sprintf(s6, "%d", p.secs2);
        strcat(s1, s2);

        strcat(s1, s3);
        strcat(s1, s4);

        strcat(s1, s5);
        strcat(s1, s6);
        LCD_DrawFillRectangle(80, 100, 180, 120, BLACK);
        LCD_DrawString(90,100, WHITE, WHITE, s1, 16, 1);
    }
    if(key_press == '*') {
        pill_menu();
    }
}


void set_time(void) {
    // pressing reset will reset time (as will unplugging)
    // display 00 00 00
    // A - next in time seq
    // B - back in time seq
    LCD_Clear(BLACK);

    LCD_DrawString(20,190, WHITE, WHITE, "A - Edit Next Digit", 16, 1);
    LCD_DrawString(20,220, WHITE, WHITE, "B - Edit Previous Digit", 16, 1);
    LCD_DrawString(20,250, WHITE, WHITE, "* - Back to General Menu", 16, 1);
}

