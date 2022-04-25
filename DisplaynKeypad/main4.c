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
volatile int county1 = 20;
volatile int hr1 = 0;
volatile int min1 = 0;
volatile int sec1 = 0;
volatile int county2 = 30;
volatile int hr2 = 0;
volatile int min2 = 0;
volatile int sec2 = 0;
volatile int county3 = 709;
volatile int hr3 = 0;
volatile int min3 = 0;
volatile int sec3 = 0;
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
extern int qout;
extern char queue[2];
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
    if(county3 != 0&& reset3 == 1) {
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
        char key_press = get_keypress();
        if (key_press == 'A'){
            noprint = 1;
            general_menu();
        }


}

int main(void)
{
    // TFT DISPLAY
    enable_ports();
    LCD_Setup(); // this will call init_lcd_spi()
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
