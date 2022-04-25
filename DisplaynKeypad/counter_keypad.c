#include "stm32f0xx.h"

// 16 history bytes.  Each byte represents the last 8 samples of a button.
uint8_t hist[16];
volatile char queue[2];  // A two-entry queue of button press/release events.
int qin;        // Which queue entry is next for input
volatile int qout;       // Which queue entry is next for output
extern int counter;
extern int county1;
extern int county2;
extern int county3;

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
char get_key_event(void) {
    for(;;) {
        int c = calc_clock(counter);
        if (noprint == 0){
            print_clock(c, 0);
        }

          if (county1 != 0) {
              int c1 = timer_pill1(counter, 1);

              if (noprint == 0){
                  print_clock(c1, 1);
              }
          }
          if (county2 != 0) {
              int c2 = timer_pill2(counter, 1);

              if (noprint == 0){
                  print_clock(c2, 2);
              }
          }
          if (county3 != 0) {
              int c3 = timer_pill3(counter, 1);

              if (noprint == 0){
                  print_clock(c3, 3);
              }
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

