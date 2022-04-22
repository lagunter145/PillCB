#include "stm32f0xx.h"
#include "lcd.h"

// This C macro will create an array of Picture elements.
// Really, you'll just use it as a pointer to a single Picture
// element with an internal pix2[] array large enough to hold
// an image of the specified size.
// BE CAREFUL HOW LARGE OF A PICTURE YOU TRY TO CREATE:
// A 100x100 picture uses 20000 bytes.  You have 32768 bytes of SRAM.
#define TempPicturePtr(name,width,height) Picture name[(width)*(height)/6+2] = { {width,height,2} }
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
    int key_press = get_key_value();
    // switch to general menu
    if(key_press != -1) {
        general_menu();
    }
}

void general_menu(void) {
    LCD_Clear(BLACK);
    int key_press = get_key_value();

    LCD_DrawString(20,30, WHITE, WHITE, "A - Pill Menu", 16, 1);
    LCD_DrawString(20,80, WHITE, WHITE, "B - Set Time", 16, 1);
    LCD_DrawString(20,130, WHITE, WHITE, "C - Toggle Speed", 16, 1);
    LCD_DrawString(20,180, WHITE, WHITE, "D - Reset", 16, 1);
    LCD_DrawString(20,230, WHITE, WHITE, "* - Back to Home Screen", 16, 1);
    // A - pill menu
    if(key_press == 'A') {
        pill_menu();
    }
    // B - set time
    if(key_press == 'B') {

    }
    // C - high speed
    if(key_press == 'C') {
        // high speed function
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

    int key_press = get_key_value();
    if(key_press == '1') {

    }
    if(key_press == '2') {

    }
    if(key_press == '3') {

    }
    if(key_press == '*') {
        general_menu();
    }
}

void add_edit_pill(void) {
    // shows a pills curr set time (00 00 00 if not added)
    // A - next in time seq
    // B - back in time seq
    // * - back to pill_menu
    LCD_Clear(BLACK);
    LCD_DrawString(20,190, WHITE, WHITE, "A - Edit Next Digit", 16, 1);
    LCD_DrawString(20,220, WHITE, WHITE, "B - Edit Previous Digit", 16, 1);
    LCD_DrawString(20,250, WHITE, WHITE, "* - Back to Pill Menu", 16, 1);

    int key_press = get_key_value();
    if(key_press == 'A') {

    }
    if(key_press == 'B') {

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

