#include "stm32f30x_conf.h"
#include <stdio.h>

#ifndef TIME_H
#define TIME_H

//A flag for updating the LCD
extern uint8_t updateLCD;
extern uint8_t updateGame;
extern uint8_t updateMinigame;
extern uint8_t updateMenu;
#define updateSpeed 0x000a


void TIM1_BRK_TIM15_IRQHandler(void);
void setUpTimer15();
void setUpTimer2();
void startTimer2();
void stopTimer2();
void setGameSpeed(int8_t gameSpeedIn);
void setMiniGameSpeed(int8_t gameSpeedIn);

#endif //TIME_H
