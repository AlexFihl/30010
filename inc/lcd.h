#include "stm32f30x_conf.h"
#include <stdio.h>
#include <string.h>
#include "charset.h"
#include "30010_io.h"
#include "io.h"
#include "time.h"

#ifndef LCD_H
#define LCD_H



void setupLCD();
void lcd_write_string(char * s, uint16_t slice, uint16_t line);
void lcd_update(); //moving the buffer on pixel to the right.
void bufferReset();
void putInBuffer(uint8_t byte, uint16_t slice, uint16_t line);
uint8_t getBuffer(int8_t slice, int8_t line);
void push_Buffer();

#endif //LCD_H
