#include "time.h"

struct timer_t mainTimer = {0,0,0,0};



uint8_t minigameSpeed;
uint8_t gameSpeedCounter;
uint8_t minigameSpeedCounter;

void TIM2_IRQHandler(void)
{
    mainTimer.hseconds++;
    if (mainTimer.hseconds == 100)
    {
        mainTimer.hseconds = 0;
        mainTimer.seconds++;
    }
    if (mainTimer.seconds == 60)
    {
        mainTimer.seconds = 0;
        mainTimer.minuts++;
    }
    if (mainTimer.minuts == 60)
    {
        mainTimer.hours++;
        mainTimer.minuts = 0;
    }
    if (mainTimer.hseconds == 0x0000 || (mainTimer.hseconds & updateSpeed) == updateSpeed)
        updateLCD = 1;
    gameSpeedCounter++;
    minigameSpeedCounter++;
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
    TIM2->SR &= ~0x0001;
}


void setUpTimer2()
{
    RCC->APB1ENR |= RCC_APB1Periph_TIM2;
    TIM2->CR1 = 0x0000;
    TIM2->ARR = 0x0009C3FF; // Reload Value = 639999. At 100 Hz
    TIM2->PSC = 0x0000; //Prescale = 0
    TIM2->DIER = 0x0001; //
    NVIC_SetPriority(TIM2_IRQn, 0);
    NVIC_EnableIRQ(TIM2_IRQn);

    //Setting the updateLCD to 0
    updateLCD = 0;
    updateGame = 0;
    updateMinigame = 0;
    gameSpeedCounter = 0;
    minigameSpeedCounter = 0;
}

void resetTimer(struct timer_t *t)
{
    t->hours = 0;
    t->minuts = 0;
    t->seconds = 0;
    t->hseconds = 0;
}

void drawAWatch(struct timer_t t) //Prints the time for hr:mm:ss.hs
{
    printf("%02d:%02d:%02d.%02d",t.hours,t.minuts,t.seconds,t.hseconds);
}

//Starts and stop the timer2
void startTimer2()
{
    TIM2->CR1 |= 0x0001;
}
void stopTimer2()
{
    TIM2->CR1 &= ~0x0001;
}
