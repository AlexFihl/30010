#include "time.h"

struct timer_t mainTimer = {0,0,0,0,0};

uint8_t minigameSpeed;
uint8_t gameSpeedCounter;
uint8_t minigameSpeedCounter;
uint8_t gameSpeed;
uint8_t gameSpeedCounter;
uint8_t updateGame;
uint8_t playSoundFlag;
uint16_t soundCount;

void TIM1_BRK_TIM15_IRQHandler(void)
{
    mainTimer.twothHS++;
    if(mainTimer.twothHS == 2)
    {
        mainTimer.hseconds++;
        mainTimer.twothHS = 0;
    }
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
    /*if(playSoundFlag == 1)
        soundCount++;
    if (soundCount > 100)
    {
        playSoundFlag = 0;
        soundCount = 0;
    }*/


    TIM15->SR &= ~0x0001;
}

void resetTimer(struct timer_t *t)
{
    t->hours = 0;
    t->minuts = 0;
    t->seconds = 0;
    t->hseconds = 0;
    t->twothHS = 0;
}

void drawAWatch(struct timer_t t) //Prints the time for hr:mm:ss.hs
{
    printf("%02d:%02d:%02d.%02d",t.hours,t.minuts,t.seconds,t.hseconds);
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
    playSoundFlag = 0;
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

void setGameSpeed(int8_t gameSpeedIn) {gameSpeed = gameSpeedIn;} //09/06
