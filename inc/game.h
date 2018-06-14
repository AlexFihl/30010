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
#include "powerUp.h"

#ifndef AGAME_H
#define AGAME_H

void fullGame(struct player_t *p, uint16_t startBallSpeed);
uint8_t aGame1(struct player_t *p, uint8_t gameCount, uint16_t startBallSpeed);

#endif //AGAME_H
