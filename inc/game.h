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
#include "minigame.h"

#ifndef AGAME_H
#define AGAME_H


void fullGame(struct player_t *p, int32_t startBallSpeed, int8_t deltaStrikerStart, int8_t deltaGamingSpeed, int8_t *scoreMultiplier);

#endif //AGAME_H
