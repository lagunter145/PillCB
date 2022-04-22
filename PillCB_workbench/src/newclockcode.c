/*
#include "stm32f0xx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

uint8_t col;

//clock
int counter = 0;
int seconds = 0;
int minutes = 0;
int hours = 0;
int county1 = 0;
int county2 = 0;
int county3 = 0;
int fastkeyhit = 0;

void enableports(void){

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

void EXTI2_3_IRQHandler(void){
    int hr1 = 0;
    int min1 = 0;
    int sec1 = 0;

    int hr2 = 0;
    int min2 = 0;
    int sec2 = 0;

    int hr3 = 0;
    int min3 = 0;
    int sec3 = 0;

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

    //LOOP TO TOGGLE *************
    //normal mode
    if (counter == 60){
        seconds++;
        counter = 0;
    }
    if (seconds == 60){
        minutes++;
        seconds = 0;
    }
    if (minutes == 60){
        hours++;
        minutes = 0;
    }
    //IS THERE A POINT WHERE WE WANT TO RESET HOURS?????
    //Yes
    if (hours == 24) {
        hours = 0;
    }
//only do this if fast mode key is hit, how do we do this?
    //demo mode
    if (fastkeyhit != 0) { //I guess this is one way?
        if (counter == 1){
            seconds++;
            counter = 0;
        }
        if (seconds == 1){
            minutes++;
            seconds = 0;
        }
        if (minutes == 1){
            hours++;
            minutes = 0;
        }
        if (hours == 24) {
            hours = 0;
        }
    }

    //calculate display values for Pill 1
    if (county1 != 0) {

        hr1 = county1 / 10000;
        min1 = (county1 / 100) % 100;
        sec1 = county1 % 100;

        if (counter == 60) {
            sec1--;
            counter = 0;
        }
        if (sec1 == -1) {
            min1--;
            sec1 = 59;
        }
        if (min1 == -1) {
            hr1--;
            min1 = 59;
        }
        if (hr1 == 0 && min1 == 0 && sec1 == 0) {
            //trigger message and alarm, not sure how to do this
        }
        char s11[20];
        char s12[20];
        char s13[20];

        sprintf(s11, "%d", hr1);
        sprintf(s12, "%d", min1);
        sprintf(s13, "%d", sec1);

        strcat(s11, s12);
        strcat(s11, s13);

        int c1 = atoi(s11);
    }
    //calculate display values for Pill 2
    if (county2 != 0) {
        hr2 = county2 / 10000;
        min2 = (county2 / 100) % 100;
        sec2 = county2 % 100;

        if (counter == 60) {
            sec2--;
            counter = 0;
        }
        if (sec2 == -1) {
            min2--;
            sec2 = 59;
        }
        if (min2 == -1) {
            hr2--;
            min2 = 59;
        }
        if (hr2 == 0 && min2 == 0 && sec2 == 0) {
            //trigger message and alarm, not sure how to do this
        }
        char s21[20];
        char s22[20];
        char s23[20];

        sprintf(s21, "%d", hr2);
        sprintf(s22, "%d", min2);
        sprintf(s23, "%d", sec2);

        strcat(s21, s22);
        strcat(s21, s23);

        int c2 = atoi(s21);
    }
    //calculate display values for Pill 3
    if (county3 != 0) {
        hr3 = county3 / 10000;
        min3 = (county3 / 100)  % 100;
        sec3 = county3 % 100;

        if (counter == 60) {
            sec3--;
            counter = 0;
        }
        if (sec3 == -1) {
            min3--;
            sec3 = 59;
        }
        if (min3 == -1) {
            hr3--;
            min3 = 59;
        }
        if (hr3 == 0 && min3 == 0 && sec3 == 0) {
            //trigger message and alarm, not sure how to do this
        }
        char s31[20];
        char s32[20];
        char s33[20];

        sprintf(s31, "%d", hr3);
        sprintf(s32, "%d", min3);
        sprintf(s33, "%d", sec3);

        strcat(s31, s32);
        strcat(s31, s33);

        int c3 = atoi(s31);
    }

    //concatenates the values so that one value can
    //be returned and sent to display function
    char s1[20];
    char s2[20];
    char s3[20];

    sprintf(s1, "%d", hr1);
    sprintf(s2, "%d", min1);
    sprintf(s3, "%d", sec1);

    strcat(s1, s2);
    strcat(s1, s3);

    int c = atoi(s1);
}

int countdown1(){
    int rows = read_rows(); //read the user input value from keyp
    update_history(col, rows); //tells us what button they pressed by value (ex. D)
    col = (col + 1) & 3; //updates column
    drive_column(col);

    county1 = 0; //COME BACK TO THIS -- how do we know what they pressed ??

    return county1;

}


int main(void){

    enableports();

}
*/
