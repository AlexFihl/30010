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

#ifndef AGAME_H
#define AGAME_H

uint8_t updateGame;

void fullGame(struct player_t *p);
uint8_t aGame1(struct player_t *p, uint8_t gameSpeedIn);
void setGameSpeed(int8_t gameSpeedIn);




#endif //AGAME_H
