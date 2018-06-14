#include "stm32f30x_conf.h"
#include <stdio.h>
#include "trigomentri.h"
#include "ansi.h"
#include "fix14.h"
#include "gameWall.h"
#include "blocks.h"
#include "player.h"
#include "striker.h"

#ifndef BALL_H
#define BALL_H

struct ball_t
{
    struct vector_t position;
    struct vector_t oldPos;
    uint32_t velocity;
    int32_t angle;
    uint16_t hitCount;
};

void intBall(struct ball_t *b, int32_t x, int32_t y, int32_t vx, int32_t vy);
void setBallSpeedFactor(int32_t speedFactor);
void addToBallSpeedFactor(int32_t deltaSpeed);
void moveBall(struct ball_t *b, int32_t deltaX, int32_t deltaY);
void moveBallToPosition(struct ball_t *b);
void updatePosition(struct ball_t *b, struct wall_t *w, struct block_t ** blocks, uint16_t numberOfBlocks, struct player_t *p, struct striker_t *s);
void resetBall(struct ball_t *b);

//To draw the ball, from an ball struct
void drawBall(struct ball_t *b);


#endif	// TRIGOMENTRI_H
