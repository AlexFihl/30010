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
void removeCursor();
void clrsrc();
void clreol();
void gotoxy(uint8_t x, uint8_t y);
void underline(uint8_t on);
void blink(uint8_t on);
void inverse(uint8_t on);





#endif
