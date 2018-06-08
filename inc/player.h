#include "stm32f30x_conf.h"
#include <stdio.h>

#ifndef PLAYER_H
#define PLAYER_H

struct player_t{
    uint32_t score;
    char name;
    uint8_t life;
};

void intPlayer(struct player_t *p);
void lossLife(struct player_t *p);
void gainLife(struct player_t *p);

#endif //PLAYER_H
