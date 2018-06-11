#include "stm32f30x_conf.h"
#include "lcd.h"
#include <stdio.h>

#ifndef TIME_H
#define TIME_H

//A flag for updating the LCD
extern uint8_t updateLCD;
extern uint8_t updateGame;
<<<<<<< HEAD
extern uint8_t updateMinigame;
extern uint8_t gameSpeed;
extern uint8_t minigameSpeed;

=======
>>>>>>> master
#define updateSpeed 0x000a

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
void setGameSpeed(int8_t gameSpeedIn);

#endif //TIME_H
