#include "stm32f30x_conf.h"
#include <stdio.h>
#include "trigomentri.h"
#include "ansi.h"
#include "fix14.h"
#include "gameWall.h"
#include "blocks.h"
#include "player.h"
#include "striker.h"
#include "ball.h"
#include "io.h"
#include "lcd.h"
#include "time.h"

#ifndef AGAME_H
#define AGAME_H

struct powerUp_t{
    struct vector_t v;
    struct vector_t old;
    uint32_t downSpeed;
    uint8_t catched;
    uint16_t sign;
};

void fullGame(struct player_t *p, uint16_t startBallSpeed);
uint8_t aGame1(struct player_t *p, uint8_t gameCount, uint16_t startBallSpeed);

#endif //AGAME_H
