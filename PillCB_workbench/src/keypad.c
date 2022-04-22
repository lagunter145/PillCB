#include "stm32f0xx.h"

// 16 history bytes.  Each byte represents the last 8 samples of a button.
uint8_t hist[16];
char queue[2];  // A two-entry queue of button press/release events.
int qin;        // Which queue entry is next for input
int qout;       // Which queue entry is next for output

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

char get_key_event(void) {
    for(;;) {
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

int get_key_value(void)
{
    int get = -1;
    for(;;)
        for(int c=0; c<4; c++) {
            drive_column(c);
            int r = read_rows();
            if (c==3) { // leftmost column
                if (r & 8) { // '1'
                    get = '1';
                }
                if (r & 4) { // '4'
                    get = '4';
                }
                if (r & 2) { // '7'
                    get = '7';
                }
                if (r & 1) { // '*'
                    get = '*';
                }
            } else if (c == 2) { // column 2
                if (r & 8) { // '2'
                    get = '2';
                }
                if (r & 4) { // '5'
                    get = '5';
                }
                if (r & 2) { // '8'
                    get = '8';
                }
                if (r & 1) { // '0'
                    get = '0';
                }
            } else if (c == 1) { // column 3
                if (r & 8) { // '1'
                    get = '3';
                }
                if (r & 4) { // '4'
                    get = '6';
                }
                if (r & 2) { // '7'
                    get = '9';
                }
                if (r & 1) { // '*'
                    get = '#';
                }
            } else if (c == 0) { // column 3
                if (r & 8) { // '1'
                    get = 'A';
                }
                if (r & 4) { // '4'
                    get = 'B';
                }
                if (r & 2) { // '7'
                    get = 'C';
                }
                if (r & 1) { // '*'
                    get = 'D';
                }
            }
        }
    return get;
}

