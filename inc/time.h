#include "stm32f30x_conf.h"
#include "lcd.h"
#include <stdio.h>

#ifndef TIME_H
#define TIME_H

//A flag for updating the LCD
extern uint8_t updateLCD;
extern uint8_t updateGame;
extern uint8_t gameSpeed;
#define updateSpeed 0x000a
uint8_t gameSpeedCounter;

struct timer_t
{
    uint16_t hours;
    uint16_t minuts;
    uint16_t seconds;
    uint16_t hseconds;
};

void TIM2_IRQHandler(void);
void setUpTimer2();
void resetTimer(struct timer_t *t);
void drawAWatch(struct timer_t t);
void startTimer2();
void stopTimer2();


#endif //TIME_H
