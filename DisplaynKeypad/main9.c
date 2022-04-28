
/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include "stm32f0xx.h"
#include "lcd.h"
#include "midiplay.h"
#include "midi.h"
//#include "wavetable.c"
volatile int county1 = 555555;
volatile int hr1 = 0;
volatile int min1 = 0;
volatile int sec1 = 0;
volatile int county2 = 555555;
volatile int hr2 = 0;
volatile int min2 = 0;
volatile int sec2 = 0;
volatile int county3 = 555555;
volatile int hr3 = 0;
volatile int min3 = 0;
volatile int sec3 = 0;
//volatile int countNJ = 0;
extern int countNJ;
volatile int anime = 0;
//extern short int wavetable[N];

//MIDI STUFF
// The number of simultaneous voices to support.
#define VOICES 15

// An array of "voices".  Each voice can be used to play a different note.
// Each voice can be associated with a channel (explained later).
// Each voice has a step size and an offset into the wave table.
struct {
    uint8_t in_use;
    uint8_t note;
    uint8_t chan;
    uint8_t volume;
    int     step;
    int     offset;
} voice[VOICES];


// We'll use the Timer 6 IRQ to recompute samples and feed those
// samples into the DAC.
void TIM6_DAC_IRQHandler(void)
{
    // TODO: Remember to acknowledge the interrupt right here.
    TIM6->SR &= ~(TIM_SR_UIF);
    int sample = 0;
    for(int x=0; x < sizeof voice / sizeof voice[0]; x++) {
        if (voice[x].in_use) {
            voice[x].offset += voice[x].step;
            if (voice[x].offset >= N<<16)
                voice[x].offset -= N<<16;
            sample += (wavetable[voice[x].offset>>16] * voice[x].volume) >> 4;
        }
    }
    sample = (sample >> 10) + 2048;
    if (sample > 4095)
        sample = 4095;
    else if (sample < 0)
        sample = 0;
    DAC->DHR12R1 = sample;
}

// Initialize the DAC so that it can output analog samples
// on PA4.  Configure it to be triggered by TIM6 TRGO.
void init_dac(void)
{
    // TODO: you fill this in.
    RCC->APB1ENR |= RCC_APB1ENR_DACEN;
    DAC->CR &= ~(0x38);     // ensures TSEL is 000
    DAC->CR |= DAC_CR_TEN1;        //enables DAC_channel1 trigger
    DAC->CR |= DAC_CR_EN1;
}

// Initialize Timer 6 so that it calls TIM6_DAC_IRQHandler
// at exactly RATE times per second.  You'll need to select
// a PSC value and then do some math on the system clock rate
// to determine the value to set for ARR.  Set it to trigger
// the DAC by enabling the Update Tigger in the CR2 MMS field.
void init_tim6(void)
{
    // TODO: you fill this in.
    RCC->APB1ENR |= 1<<4;
    TIM6->PSC = (2400000/RATE) - 1;
    TIM6->ARR = 20 - 1;
    TIM6->DIER |= 1<<0;
    TIM6->CR2 |= 0x20;
    TIM6->CR1 |= 1<<0;
    NVIC->ISER[0] = 1<<17;
}

void stop_song(void){
    for (int i = 0; i < 15; i++){
        voice[i].in_use = 0;
        voice[i].note = 0;
        voice[i].chan = 0;
        voice[i].volume = 0;
        voice[i].step = 0;
        voice[i].offset = 0;
    }
    note_off(0,0,0,0);
  //  TIM2->CR1 &= ~(TIM_CR1_CEN);

  //  DAC->CR &= ~DAC_CR_EN1;
}

// Find the voice current playing a note, and turn it off.
void note_off(int time, int chan, int key, int velo)
{
    int n;
    for(n=0; n<sizeof voice / sizeof voice[0]; n++) {
        if (voice[n].in_use && voice[n].note == key) {
            voice[n].in_use = 0; // disable it first...
            voice[n].chan = 0;   // ...then clear its values
            voice[n].note = key;
            voice[n].step = step[key];
            return;
        }
    }
}

// Find an unused voice, and use it to play a note.
void note_on(int time, int chan, int key, int velo)
{
    if (velo == 0) {
        note_off(time, chan, key, velo);
        return;
    }
    int n;
    for(n=0; n<sizeof voice / sizeof voice[0]; n++) {
        if (voice[n].in_use == 0) {
            voice[n].note = key;
            voice[n].step = step[key];
            voice[n].offset = 0;
            voice[n].volume = velo;
            voice[n].chan = chan;
            voice[n].in_use = 1;
            return;
        }
    }
}

void set_tempo(int time, int value, const MIDI_Header *hdr)
{
    // This assumes that the TIM2 prescaler divides by 48.
    // It sets the timer to produce an interrupt every N
    // microseconds, where N is the new tempo (value) divided by
    // the number of divisions per beat specified in the MIDI file header.
    TIM2->ARR = value/hdr->divisions - 1;
}

const float pitch_array[] = {
0.943874, 0.945580, 0.947288, 0.948999, 0.950714, 0.952432, 0.954152, 0.955876,
0.957603, 0.959333, 0.961067, 0.962803, 0.964542, 0.966285, 0.968031, 0.969780,
0.971532, 0.973287, 0.975046, 0.976807, 0.978572, 0.980340, 0.982111, 0.983886,
0.985663, 0.987444, 0.989228, 0.991015, 0.992806, 0.994599, 0.996396, 0.998197,
1.000000, 1.001807, 1.003617, 1.005430, 1.007246, 1.009066, 1.010889, 1.012716,
1.014545, 1.016378, 1.018215, 1.020054, 1.021897, 1.023743, 1.025593, 1.027446,
1.029302, 1.031162, 1.033025, 1.034891, 1.036761, 1.038634, 1.040511, 1.042390,
1.044274, 1.046160, 1.048051, 1.049944, 1.051841, 1.053741, 1.055645, 1.057552,
};

void pitch_wheel_change(int time, int chan, int value)
{
    //float multiplier = pow(STEP1, (value - 8192.0) / 8192.0);
    float multiplier = pitch_array[value >> 8];
    for(int n=0; n<sizeof voice / sizeof voice[0]; n++) {
        if (voice[n].in_use && voice[n].chan == chan) {
            voice[n].step = step[voice[n].note] * multiplier;
        }
    }
}

// An array of structures that say when to turn notes on or off.
struct {
    int when;
    uint8_t note;
    uint8_t volume;
};

int time = 0;
int n = 0;
void TIM2_IRQHandler(void)
{
    // TODO: Remember to acknowledge the interrupt here!
    TIM2->SR &= ~(TIM_SR_UIF);
    midi_play();
#if defined(RICK_DID_THIS)
    note_on(0,0,70,128);
    TIM2->CR1 &= ~TIM_CR1_CEN;
    NVIC->ICER[0] = -1;
    NVIC->ISER[0] = 1<<TIM6_DAC_IRQn;
#endif
}
// Configure timer 2 so that it invokes the Update interrupt
// every n microseconds.  To do so, set the prescaler to divide
// by 48.  Then the CNT will count microseconds up to the ARR value.
// Basically ARR = n-1
// Set the ARPE bit in the CR1 so that the timer waits until the next
// update before changing the effective ARR value.
// Call NVIC_SetPriority() to set a low priority for Timer 2 interrupt.
// See the lab 6 text to understand how to do so.
void init_tim2(int n) {
    // TODO: you fill this in.
    RCC->APB1ENR |= 1<<0;
    TIM2->PSC = 48 - 1;
    TIM2->ARR = n - 1;
    TIM6->CR1 |= TIM_CR1_ARPE;
    TIM2->DIER |= 1<<0;
    TIM2->CR1 |= 1<<0;
    NVIC->ISER[0] = 1<<15;
    NVIC_SetPriority(TIM2_IRQn, 1);
  //  TIM2->CR1 |= (TIM_CR1_CEN);
}
//===========================================================================
// KEYPAD STUFF <3
//===========================================================================
void nano_wait(unsigned int n) {
    asm(    "        mov r0,%0\n"
            "repeat: sub r0,#83\n"
            "        bgt repeat\n" : : "r"(n) : "r0", "cc");
}
uint8_t col; // the column being scanned

void init_tim7() {
    RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;
    TIM7->PSC = 24000-1;
    TIM7-> ARR = 2-1;
    TIM7->DIER |= TIM_DIER_UIE;
    NVIC->ISER[0] = 1<<TIM7_IRQn;
    TIM7->CR1 |= TIM_CR1_CEN;
    NVIC_SetPriority(TIM7_IRQn, 2);
}

void TIM7_IRQHandler()
{
    // acknowledge interrupt
    TIM7->SR &= ~TIM_SR_UIF;
    int rows = read_rows();
    update_history(col, rows);
    col = (col + 1) & 3;
    drive_column(col);
}

/*
void keypad_ports(void){
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    GPIOA->MODER &= ~0xc00;
    GPIOA->PUPDR &= ~0xc00;
    GPIOA->PUPDR |= 0x400;
    RCC->APB2ENR|= RCC_APB2ENR_SYSCFGEN;
    SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI5_PA;
    EXTI->RTSR |= EXTI_RTSR_TR5;
    EXTI->IMR &= ~0xffffffff;
    EXTI->IMR |= EXTI_IMR_MR5;
    NVIC->ISER[0] = (1<<EXTI4_15_IRQn);
}
void EXTI4_15_IRQHandler(void){
    EXTI->PR |= EXTI_PR_PR5;
    int c = calc_clock(counter);
    print_clock(c, 0);
    if (county1 != 0) {
        int c1 = timer_pill1(counter, 1);
        print_clock(c1, 1);
    }
    if (county2 != 0) {
        int c2 = timer_pill2(counter, 1);
        print_clock(c2, 2);
    }
    if (county3 != 0) {
        int c3 = timer_pill3(counter, 1);
        print_clock(c3, 3);
    }
}*/

//===========================================================================
// TFT STUFF <3
//===========================================================================

void init_lcd_spi(void)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    // configure pb 8, 11, 14 as outputs
    GPIOB->MODER &= ~(0x30c30000);
    GPIOB->MODER |= (0x10410880);
    // set odr vals for pb 8, 11, 14 high
    GPIOB->ODR |= 0x4900;
    // configure pb 3 and 5 as alt function 0
    GPIOB->AFR[0] &= ~0x00f0f000;

    // set up spi1 peripheral

    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    // clear cr1 spe
    SPI1->CR1 &= ~(SPI_CR1_SPE);

    // set baud rate to as high as possible (max divisor?)
    SPI1->CR1 &= ~ SPI_CR1_BR;

    // config spi channel in mast mode
    SPI1->CR1 |= SPI_CR1_MSTR;

    // config intergace for 8 bit word size (0111)
    SPI1->CR2 = SPI_CR2_DS_0 | SPI_CR2_DS_1 | SPI_CR2_DS_2;

    // set ssm and ssi bits
    SPI1->CR1 |= SPI_CR1_SSM;
    SPI1->CR1 |= SPI_CR1_SSI;

    // enable spi channel
    SPI1->CR1 |= SPI_CR1_SPE;
}
/*
void enable_keypadports(void) {
    // KEYPAD PORTS
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

    // port c output pins 7 -4
    // port c input pins 3 - 0
    GPIOC->MODER &= ~(0xffff);
    GPIOC->MODER |= 0x55 << (4*2);
    // port c open drain pins 7 - 4
    GPIOC->OTYPER &= ~(0xff);
    GPIOC->OTYPER |= (0xf0);
    // port c pull high pins 3 - 0
    GPIOC->PUPDR &= ~(0xff);
    GPIOC->PUPDR |= (0x55);
}
*/
void enable_ports(void)
{
    // KEYPAD PORTS

    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

    // port c output pins 7 -4
    // port c input pins 3 - 0
    GPIOC->MODER &= ~(0xffff);
    GPIOC->MODER |= 0x55 << (4*2);
    // port c open drain pins 7 - 4
    GPIOC->OTYPER &= ~(0xff);
    GPIOC->OTYPER |= (0xf0);
    // port c pull high pins 3 - 0
    GPIOC->PUPDR &= ~(0xff);
    GPIOC->PUPDR |= (0x55);


    // CLOCK PORTS
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    GPIOB->MODER &= ~0x30;
    GPIOB->MODER |= 0x0;
    GPIOB->PUPDR &= ~0x30;
    GPIOB->PUPDR |= 0x20;
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI2_PB;
    EXTI->RTSR |= EXTI_RTSR_TR2;
    EXTI->IMR &= ~0xffffffff;
    EXTI->IMR |= EXTI_IMR_MR2;
    NVIC->ISER[0] = (1<<6);
}
void general_menu(void);
volatile int counter = 0;

//===========================================================================
// CLOCK STUFF <3
//===========================================================================
void EXTI2_3_IRQHandler(void){

    EXTI->PR |= EXTI_PR_PR2;
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    GPIOC->MODER &= ~0x30000;
    GPIOC->MODER |= 0x10000;
    //toggling PC8 to make sure RTSR is getting triggered
    if(GPIOC->ODR & 1<<8) {
        GPIOC->ODR &= ~0x0100;
    }
    else {
        GPIOC->ODR |= 0x0100;
    }
    counter++;
    calc_clock(counter);
    timer_pill1(counter, 0);
    timer_pill2(counter, 0);
    timer_pill3(counter, 0);

    //pill2(counter);
    //pill3(counter);
    if(counter == 60) {
        counter = 0;
    }
}

//===========================================================================
// MAIN FUNCTION <3
//===========================================================================
//extern int qout;
//extern char queue[2];
volatile int noprint = 0;

void home(int reset1, int reset2, int reset3) {
    if(county1 != 0 && reset1 == 1) {
         hr1 = county1 / 10000;
         min1 = (county1 / 100) % 100;
         sec1 = county1 % 100;
     }
    if(county2 != 0 && reset2 == 1) {
        hr2 = county2 / 10000;
        min2 = (county2 / 100) % 100;
        sec2 = county2 % 100;
    }
    if(county3 != 0 && reset3 == 1) {
        hr3 = county3 / 10000;
        min3 = (county3 / 100) % 100;
        sec3 = county3 % 100;
    }
    int pillnum1 = 1;
    int pillnum2 = 2;
    int pillnum3 = 3;
    LCD_Clear(BLACK);
/*
    for(;;) {
        int c = calc_clock(counter);
        print_clock(c, 0);
        if (county1 != 0) {
            int c1 = timer_pill1(counter, 1);
            print_clock(c1, pillnum1);
        }
        if (county2 != 0) {
            int c2 = timer_pill2(counter, 1);
            print_clock(c2, pillnum2);
        }
        if (county3 != 0) {
            int c3 = timer_pill3(counter, 1);
            print_clock(c3, pillnum3);
        }
      */
    int check = 0;
    do {
        char key_press = get_keypress();
        if (key_press != -1){
            noprint = 1;
            general_menu();
        }
        else {
            check = 1;
        }
    }while(check == 1);


}

int main(void)
{
#if defined(RICK_DID_THIS_TOO)
    init_dac();
    init_tim6();
    init_wavetable_hybrid2();
    //TIM2_IRQHandler();
    MIDI_Player *mp = midi_init(midifile);
    init_tim2(10519);
    return 0;
#endif
    // TFT DISPLAY
    //enable_keypadports();
    enable_ports();
    LCD_Setup();
    //general_menu();

    //LCD_Setup(); // this will call init_lcd_spi()
    //LCD_Clear(BLACK);
    //LCD_DrawString(20,141, WHITE, WHITE, ":)", 16, 1); // opaque background
    //add_edit_pill();

    // KEYPAD
    // setup keyboard
    init_tim7();
    //general_menu();
/*
    for(;;) {
    int c = calc_clock(counter);
    print_clock(c);
    }
*/
    home(1,1,1);

}


