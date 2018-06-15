#include "stm32f30x_conf.h"
#include <stdio.h>

#ifndef TIME_H
#define TIME_H

//A flag for updating the LCD
extern struct timer_t mainTimer;
extern uint8_t updateLCD;
extern uint8_t updateGame;
extern uint8_t updateMinigame;
extern uint8_t minigameSpeed;
extern uint8_t updateMenu;
#define updateSpeed 0x000a

struct timer_t
{
    uint16_t hours;
    uint16_t minuts;
    uint16_t seconds;
    uint16_t hseconds;
    uint8_t twothHS;
};

void TIM1_BRK_TIM15_IRQHandler(void);
void setUpTimer15();
void resetTimer(struct timer_t *t);
void drawAWatch(struct timer_t t);
void setUpTimer2();
void startTimer2();
void stopTimer2();
void setGameSpeed(int8_t gameSpeedIn);

#endif //TIME_H
