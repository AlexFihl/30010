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

void initPowerUp(struct powerUp_t *p, struct vector_t *v, uint8_t powerUpType);
void drawPowerUp(struct powerUp_t *p, struct block_t * b, uint16_t lowerBond, uint32_t numberOfBlocks);
void applyPowerUp(struct powerUp_t *p, struct striker_t *s, struct wall_t *w, struct ball_t *b, struct player_t *pl, int8_t * ballOnStriker, int8_t *skipLevel, int8_t *strikerShoting, int8_t *multiplyBalls);
void updatePowerUp(struct powerUp_t *p, struct striker_t *s, struct wall_t *w);



#endif //POWERUP_H
