#include "gameWall.h"
#include "trigomentri.h"
#include "blocks.h"
#include "ansi.h"
#include "striker.h"
#include "stm32f30x_conf.h"
#include "player.h"
#include "ball.h"
#include <stdio.h>


#ifndef POWERUP_H
#define POWERUP_H

struct powerUp_t{
    struct vector_t v;
    struct vector_t old;
    uint32_t downSpeed;
    uint8_t catched;
    uint8_t dead;
    uint8_t type;
    uint16_t sign;
};

struct strikerShooting_t
{
    uint8_t isAlive[10];
    struct vector_t position[10];
    struct vector_t oldPosition[10];
    uint32_t speed;
    uint8_t shootSpeed;
    uint8_t amountOfBalls;
};

void initPowerUp(struct powerUp_t *p, struct vector_t *v, uint8_t powerUpType);
void drawPowerUp(struct powerUp_t *p, struct block_t * b, uint16_t lowerBond, uint32_t numberOfBlocks);
void applyPowerUp(struct powerUp_t *p, struct striker_t *s, struct wall_t *w, struct player_t *pl, int8_t * ballOnStriker, int8_t *skipLevel, int8_t *strikerShoting, int8_t *multiplyBalls, int8_t *telePortBallFlag);
void updatePowerUp(struct powerUp_t *p, struct striker_t *s, struct wall_t *w);
void initStrikerShooting (struct strikerShooting_t *s, struct striker_t *st);
void shotCaller(struct strikerShooting_t *s, struct striker_t *st, uint8_t shotNumber);
void drawShot(struct strikerShooting_t *u, uint8_t shotNumber);
void updateShot(struct wall_t *w, struct block_t ** blocks, uint16_t numberOfBlocks, struct player_t *p, struct striker_t *s, struct strikerShooting_t *u, uint8_t shotNumber);
void printPowerupHelp();


#endif //POWERUP_H
