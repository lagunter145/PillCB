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

//===========================================================================
// KEYPAD STUFF <3
//===========================================================================
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

//void basic_drawing(void);
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
    home_screen();
}


















