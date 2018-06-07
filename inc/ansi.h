#include "stm32f30x_conf.h"
#include "30010_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _ANSI_H_
#define _ANSI_H_

void fgcolor(uint8_t foreground);
void bgcolor(uint8_t background);
void color(uint8_t foreground, uint8_t background);
void resetbgcolor();
void showCursor();
void clrsrc();
void clreol();
void gotoxy(uint8_t x, uint8_t y);
void underline(uint8_t on);
void blink(uint8_t on);
void inverse(uint8_t on);
void window(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, char text[], uint8_t style);
void windowFG(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, char text[], uint8_t style, uint8_t colorText, uint8_t colorBond);
void windowBG(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, char text[], uint8_t style, uint8_t colorText, uint8_t colorTextbg, uint8_t colorBond, uint8_t colorBondbg);





#endif
