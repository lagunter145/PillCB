#include "stm32f0xx.h"
#include "lcd.h"
#include "midi.h"
#include "midiplay.h"
// 16 history bytes.  Each byte represents the last 8 samples of a button.
uint8_t hist[16];
volatile char queue[2];  // A two-entry queue of button press/release events.
int qin;        // Which queue entry is next for input
volatile int qout;       // Which queue entry is next for output
extern int counter;
extern int county1;
extern int county2;
extern int county3;
extern int anime;
extern int screenType;
extern int countNJ;
extern Picture nj_la_small, nj_lb_small, nj_lc_small;
extern int userinputclock;
extern int userinputpill1;
extern int userinputpill2;
extern int userinputpill3;
extern int alarmtimecounter = 0;
void push_queue(int);
char pop_queue(void);
void update_history(int, int);
char get_key_event(void);
char get_keypress(void);
const char keymap[] = "DCBA#9630852*741";

void push_queue(int n) {
    queue[qin] = n;
    qin ^= 1;
}

char pop_queue() {
    char tmp = queue[qout];
    queue[qout] = 0;
    qout ^= 1;
    return tmp;
}

void update_history(int c, int rows)
{
    // We used to make students do this in assembly language.
    for(int i = 0; i < 4; i++) {
        hist[4*c+i] = (hist[4*c+i]<<1) + ((rows>>i)&1);
        if (hist[4*c+i] == 0x01)
            push_queue(0x80 | keymap[4*c+i]);
        if (hist[4*c+i] == 0xfe)
            push_queue(keymap[4*c+i]);
    }
}

extern int noprint;
//volatile int doneprint = 0;
char get_key_event(void) {
    for(;;) {

        int c = calc_clock(counter);
        if (noprint == 0 && userinputclock == 1){
            print_clock(c, 0);
            //nano_wait(1000000000);

        }
        else {
            if (screenType == 0)
                write_to_display("Set ya clock!!", 70, 75);
        }

          if (county1 != 0) {
              int c1 = timer_pill1(counter, 1);

              if (noprint == 0 && userinputpill1 == 1){
                  print_clock(c1, 1);
                  //nano_wait(10000);
              }
              else if (noprint == 0 && (userinputclock == 1))
                  write_to_display("Pill 1: NOT SET!", 100, 50);
          }
          if (county2 != 0) {
              int c2 = timer_pill2(counter, 1);

              if (noprint == 0 && userinputpill2 == 1){
                  print_clock(c2, 2);
                  //nano_wait(10000);
              }
              else if (noprint == 0 && (userinputclock == 1))
                  write_to_display("Pill 2: NOT SET!", 130, 50);
          }
          if (county3 != 0) {
              int c3 = timer_pill3(counter, 1);

              if (noprint == 0 && userinputpill3 == 1){
                  print_clock(c3, 3);
                  //nano_wait(10000);
              }
              else if (noprint == 0 && (userinputclock == 1))
                  write_to_display("Pill 3: NOT SET!", 160, 50);
          }

          if (anime == 1 && alarmtimecounter != 0) {
              if(countNJ == 0)
                  LCD_DrawPicture(40,40,&nj_la_small);
              else if(countNJ == 1)
                  LCD_DrawPicture(40,40,&nj_lb_small);
              else if(countNJ == 2)
                  LCD_DrawPicture(40,40,&nj_lc_small);
              else {
                  LCD_DrawPicture(40,40,&nj_lb_small);

              }
              countNJ = (countNJ + 1) % 4;
              alarmtimecounter++;

              if (alarmtimecounter == 100000){
                  alarmtimecounter = 0;
                  general_menu();

              }

              //nano_wait(1000000000000);
              //doneprint = 1;


/*
              for (;;){
                  nano_wait(1000000000000);
                  home_screen();
                  break;
              }*/
          }

        asm volatile ("wfi" : :);   // wait for an interrupt
        if (queue[qout] != 0)
            break;
    }
    return pop_queue();
}

char get_keypress() {
    char event;
    for(;;) {
        // Wait for every button event...
        event = get_key_event();
        // ...but ignore if it's a release.
        if (event & 0x80)
            break;
    }
    return event & 0x7f;
}
