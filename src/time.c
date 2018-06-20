#include "time.h"

/*
** These global variables are flags that are been used to been able to update
** various things
*/
uint8_t updateLCD;
uint8_t updateGame;
uint8_t updateMenu;
uint8_t updateMinigame;
/*
** All of these values are counters for the IRQHandler for TIM15.
*/
uint8_t updateLCDCounter;
uint8_t gameSpeedCounter;
uint8_t minigameSpeedCounter;
uint8_t gameSpeedCounter;
uint8_t updateMenuCounter;

/*
** These variables are for setting the speed for game and minigame.
** These can be set by set functions seen longer down in the flie.
** The are what the counters need to count up to.
*/
uint8_t minigameSpeed;
uint8_t gameSpeed;

void TIM1_BRK_TIM15_IRQHandler(void)
{
    gameSpeedCounter++;
    minigameSpeedCounter++;
    updateMenuCounter++;
    updateLCDCounter++;
    if(updateMenuCounter == 10) //Every 5 100 part of a secound
    {
        updateMenu = 1;
        updateMenuCounter = 0;
    }
    if (updateLCDCounter == 10) //Every 5 100 part of a secound
    {
        updateLCD = 1;
        updateLCDCounter = 0;
    }
    if (gameSpeedCounter == gameSpeed)
    {
        updateGame = 1;
        gameSpeedCounter = 0;
    }
    if (minigameSpeedCounter == minigameSpeed)
    {
        updateMinigame = 1;
        minigameSpeedCounter = 0;
    }
    TIM15->SR &= ~0x0001;
}

void setUpTimer15()
{
    RCC->APB2ENR |= RCC_APB2Periph_TIM15;

    TIM15->CR1 = 0x0000;
    TIM15->ARR = 31999; //
    TIM15->PSC = 9; //Prescale = 9
    TIM15->DIER |= 0x0001; //

    NVIC_SetPriority(TIM1_BRK_TIM15_IRQn, 0);
    NVIC_EnableIRQ(TIM1_BRK_TIM15_IRQn);

    TIM15->CR1 |=  0x0001;

    //Setting all the flags to 0.
    updateLCD = 0;
    updateGame = 0;
    updateMinigame = 0;
    gameSpeedCounter = 0;
    minigameSpeedCounter = 0;
    updateMenu = 1;
    updateLCDCounter = 0;
    updateMenuCounter = 0;
}

void setUpTimer2()
{
    RCC->APB1ENR |= 0x00000001; // Enable clock line to timer 2;
    TIM2->CR1 = 0x0000; // Disable timer
    TIM2->ARR = 1000; // Set auto reload value
    TIM2->PSC = 0; // Set pre-scaler value
    TIM2->CR1 |= 0x0001; // Enable timer

    TIM2->CCER &= ~TIM_CCER_CC3P; // Clear CCER register
    TIM2->CCER |= 0x00000001 << 8; // Enable OC3 output
    TIM2->CCMR2 &= ~TIM_CCMR2_OC3M; // Clear CCMR2 register
    TIM2->CCMR2 &= ~TIM_CCMR2_CC3S;
    TIM2->CCMR2 |= TIM_OCMode_PWM1; // Set output mode to PWM1
    TIM2->CCMR2 &= ~TIM_CCMR2_OC3PE;
    TIM2->CCMR2 |= TIM_OCPreload_Enable;
    TIM2->CCR3 = 500; // Set duty cycle to 50 %
}

void startTimer2()
{
    TIM2->CR1 |= 0x0001;
}
void stopTimer2()
{
    TIM2->CR1 &= ~0x0001;
}

void setGameSpeed(int8_t gameSpeedIn) {gameSpeed = gameSpeedIn;}
void setMiniGameSpeed(int8_t gameSpeedIn) {minigameSpeed = gameSpeedIn;}
