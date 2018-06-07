#include "stm32f30x_conf.h"
#include <stdio.h>
#include "trigomentri.h"
#include "ansi.h"

#define FIX14_SHIFT 14
#define FIX14_MULT(a, b) ( (a)*(b) >> FIX14_SHIFT )
#define FIX14_DIV(a, b) ( (a) << FIX14_SHIFT / (b) )

#ifndef BALL_H
#define BALL_H

//Thee speed factor which it will be 0400 = 0.0625
#define speed 0x00000200

struct ball_t
{
    struct vector_t position;
    struct vector_t oldPos;
    struct vector_t velocity;
    uint16_t hitCount;
};

void intBall(struct ball_t *b, int32_t x, int32_t y, int32_t vx, int32_t vy);
void drawBall(struct ball_t *b);
void updatePosition(struct ball_t *b, uint8_t wallx1, uint8_t wally1, uint8_t wallx2, uint8_t wally2);


#endif	// TRIGOMENTRI_H
