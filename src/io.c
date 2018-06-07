#include "io.h"

void digitalWrite(uint16_t port, char portName, uint8_t value)
{
    if (portName == 'A')
    {
        if (value == 1) GPIOA->ODR |= (0x0001 << port);
        else GPIOA->ODR &= ~(0x0001 << port);
    }
    else if (portName == 'B')
    {
        if (value == 1) GPIOB->ODR |= (0x0001 << port);
        else GPIOB->ODR &= ~(0x0001 << port);
    }
    else if (portName == 'C')
    {
        if (value == 1) GPIOC->ODR |= (0x0001 << port);
        else GPIOC->ODR &= ~(0x0001 << port);
    }
}

uint8_t digitalRead(uint16_t port, char portName)
{
    uint8_t x;
    if (portName == 'A')
    {
        x = (GPIOA->IDR & (0x0001 << port)) >> port;
    }
    else if (portName == 'B')
    {
        x = (GPIOB->IDR & (0x0001 << port)) >> port;
    }
    else if (portName == 'C')
    {
        x = (GPIOC->IDR & (0x0001 << port)) >> port;
    }
    return x;
}

void startUpABC()
{
    RCC->AHBENR |= RCC_AHBPeriph_GPIOA;
    RCC->AHBENR |= RCC_AHBPeriph_GPIOB;
    RCC->AHBENR |= RCC_AHBPeriph_GPIOC;
}

/*
*** moder:
****** 0x00000000 Input
****** 0x00000000 Output
*** puder:
****** 0x00000000 no-pull
****** 0x00000001 Pull-up
****** 0x00000002 Pull-down
*/
void pinSetup(uint16_t port, char portName, uint32_t moder, uint32_t puder)
{
    if (portName == 'A')
    {
        GPIOA->MODER &= ~(0x00000003 << (port * 2));
        GPIOA->MODER |= (moder << (port * 2));
        GPIOA->PUPDR &= ~(0x00000003 << (port * 2));
        GPIOA->PUPDR |= (puder << (port * 2));
        if (moder == 0x00000001)
        {
            GPIOA->OSPEEDR &= ~(0x00000003 << (port * 2));
            GPIOA->OSPEEDR |=  (0x00000002 << (port * 2));
            GPIOA->OTYPER &= ~(0x0003 << (port * 2));
            GPIOA->OTYPER |=  (0x0000 << (port * 2));
        }
    }
    else if (portName == 'B')
    {
        GPIOB->MODER &= ~(0x00000003 << (port * 2));
        GPIOB->MODER |= (moder << (port * 2));
        GPIOB->PUPDR &= ~(0x00000003 << (port * 2));
        GPIOB->PUPDR |= (puder << (port * 2));
        if (moder == 0x00000001)
        {
            GPIOB->OSPEEDR &= ~(0x00000003 << (port * 2));
            GPIOB->OSPEEDR |=  (0x00000002 << (port * 2));
            GPIOB->OTYPER &= ~(0x0003 << (port * 2));
            GPIOB->OTYPER |=  (0x0000 << (port * 2));
        }
    }
    else if (portName == 'C')
    {
        GPIOB->MODER &= ~(0x00000003 << (port * 2));
        GPIOB->MODER |= (moder << (port * 2));
        GPIOB->PUPDR &= ~(0x00000003 << (port * 2));
        GPIOB->PUPDR |= (puder << (port * 2));
        if (moder == 0x00000001)
        {
            GPIOB->OSPEEDR &= ~(0x00000003 << (port * 2));
            GPIOB->OSPEEDR |=  (0x00000002 << (port * 2));
            GPIOB->OTYPER &= ~(0x0003 << (port * 2));
            GPIOB->OTYPER |=  (0x0000 << (port * 2));
        }
    }
}

//Will return a 8 bit value where:
/*
***** 0 Bit: Up
***** 1 Bit: Down
***** 2 Bit: Left
***** 3 Bit: Right
***** 4 Bit: Center
*/
uint8_t readJoyStick()
{
    uint8_t x = 0x00;
    x |= (GPIOA->IDR & (0x0001 << 4)) >> 4; //Up
    x |= (GPIOB->IDR & (0x0001 << 0)) << 1; //Down
    x |= (GPIOC->IDR & (0x0001 << 1)) << 1; //Left
    x |= (GPIOC->IDR & (0x0001 << 0)) << 3; //Right
    x |= (GPIOB->IDR & (0x0001 << 5)) >> 1; //Center
    return x;
}

//This function sets the joystick pins for the joy stick
void joyStickSetUp()
{

    pinSetup(0,'C', 0x00000000, 0x00000000); //Right
    pinSetup(4,'A', 0x00000000, 0x00000000); //Up
    pinSetup(5,'B', 0x00000000, 0x00000000); //Center
    pinSetup(1,'C', 0x00000000, 0x00000000); //Left
    pinSetup(0,'B', 0x00000000, 0x00000000); //Down
}

//setLed will take the value for r, g and b and set the right value on the LED
void setLed (uint8_t r, uint8_t g, uint8_t b)
{
    r = r > 0 ? 0 : 1;
    g = g > 0 ? 0 : 1;
    b = b > 0 ? 0 : 1;
    digitalWrite(9,'A',b); //Blue LED
    digitalWrite(7,'C',g); //Green LED
    digitalWrite(4,'B',r); //Red LED
}

void ledSetup ()
{
    pinSetup(9, 'A', 0x00000001, 0x00000000); //Blue
    pinSetup(7, 'C', 0x00000001, 0x00000000); //Green
    pinSetup(4, 'B', 0x00000001, 0x00000000); //Red

    setLed(0, 0, 0);
}

//Reading the two potentiometers
uint16_t readADC1()
{
    pinSetup(0, 'A', 0, 0);

    RCC->CFGR2  &=  ~RCC_CFGR2_ADCPRE12;
    RCC->CFGR2  |=  RCC_CFGR2_ADCPRE12_DIV6;
    RCC->AHBENR |=  RCC_AHBPeriph_ADC12;

    ADC1->CR    =   0x00000000;
    ADC1->CFGR  &=  0xFDFFC007;
    ADC1->SQR1  &=  ~ADC_SQR1_L;

    ADC1->CR    |=  0x10000000;
    for(int i = 0; i < 1000; i++);
    ADC1->CR    |=  0x80000000;
    for(int i = 0; i < 100; i++);

    ADC1->CR    |=  0x00000001;
    while(!(ADC1->ISR & 0x00000001));

    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_1Cycles5);
    ADC_StartConversion(ADC1);
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0);
    return ADC_GetConversionValue(ADC1);
}

uint16_t readADC2()
{
    pinSetup(1, 'A', 0, 0);

    RCC->CFGR2  &=  ~RCC_CFGR2_ADCPRE12;
    RCC->CFGR2  |=  RCC_CFGR2_ADCPRE12_DIV6;
    RCC->AHBENR |=  RCC_AHBPeriph_ADC12;

    ADC1->CR    =   0x00000000;
    ADC1->CFGR  &=  0xFDFFC007;
    ADC1->SQR1  &=  ~ADC_SQR1_L;

    ADC1->CR    |=  0x10000000;
    for(int i = 0; i < 1000; i++);
    ADC1->CR    |=  0x80000000;
    for(int i = 0; i < 100; i++);

    ADC1->CR    |=  0x00000001;
    while(!(ADC1->ISR & 0x00000001));

    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_1Cycles5);
    ADC_StartConversion(ADC1);
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0);
    return ADC_GetConversionValue(ADC1);
}

//This function will read from the console and get the input from it.
char * getInput()
{
    char * line = malloc(32 * sizeof(char));
    uint8_t x;
    line[0] = uart_getc();
    x = 1;
    while (line[x - 1] != 0x0D)
    {
        line[x] = uart_getc();
        x++;
    }
    line[x-1] = '\0';
    return line;
}
