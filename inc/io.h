#include "stm32f30x_conf.h"
#include "30010_io.h"
#include <stdio.h>
#include <stdlib.h>

#ifndef io_custom_h
#define io_custom_h

//Using the GPIO's
void startUPABC();
void digitalWrite(uint16_t port, char portName, uint8_t value);
uint8_t digitalRead(uint16_t port, char portName);
void startUpABC();
void pinSetup(uint16_t port, char portName, uint32_t moder, uint32_t puder);

uint16_t readADC1();
uint16_t readADC2();

//Some custom controlls for the onboard RGB LED and joystick.
uint8_t readJoyStick();
void joyStickSetUp();
void setLed (uint8_t r, uint8_t g, uint8_t b);
void ledSetup ();




#endif //io_custom_h
