#include "trigomentri.h"
#include "ansi.h"

#ifndef GAMEWALL_H
#define GAMEWALL_H

struct wall_t{
    struct vector_t v1;
    struct vector_t v2;
};

void intWall(struct wall_t *w, struct vector_t *v1, struct vector_t *v2);
void drawWall(struct wall_t *w);
//To be able to draw a window with a title

void window(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, char text[], uint8_t style);
void windowFG(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, char text[], uint8_t style, uint8_t colorText, uint8_t colorBond);
void windowBG(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, char text[], uint8_t style, uint8_t colorText, uint8_t colorTextbg, uint8_t colorBond, uint8_t colorBondbg);


#endif //GAMEWALL_H
