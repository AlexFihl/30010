#include "stm32f30x_conf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef PLAYER_H
#define PLAYER_H

struct player_t{
    uint32_t score;
    char * name;
    uint8_t life;
};

void intPlayer(struct player_t *p);
void setPlayerName(struct player_t *p, char *name);
void lossLife(struct player_t *p);
void gainLife(struct player_t *p);
void setPlayerLife(struct player_t *p, int8_t life);
void resetPlayer(struct player_t *p);

#endif //PLAYER_H
