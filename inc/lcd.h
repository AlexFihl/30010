#include "stm32f30x_conf.h"
#include <stdio.h>
#include <string.h>
#include "charset.h"
#include "30010_io.h"
#include "io.h"

#ifndef LCD_H
#define LCD_H

uint8_t updateLCD;
uint8_t scrolling;
uint8_t adcPrinting;
uint8_t lcdBuffer[512];

void setupLCD();
void lcd_write_string(char * s, uint16_t slice, uint16_t line);
void lcd_update(); //moving the buffer on pixel to the right.
void bufferReset();
void leftScrollingText(uint8_t lineNumber);
void setScrolling(uint8_t x);
void printADC();
void setADCPrinting(uint8_t x);

#endif //LCD_H
