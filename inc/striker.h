#include "stm32f30x_conf.h"
#include "trigomentri.h"
#include "ansi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fix14.h"
#include "io.h"
#include "gameWall.h"



#ifndef STRIKER_H
#define STRIKER_H

struct striker_t {
    struct vector_t center;
    struct vector_t oldCenter;
    uint32_t length;
    uint32_t oldLength;

};

void intStriker(struct striker_t *s);
void updateStriker(struct striker_t *s, int32_t deltaCenter);
void changeStrikerLength(struct striker_t *s, int32_t deltaLength);
void drawStriker(struct striker_t *s);
void resetStriker(struct striker_t *s);
int8_t updateStrikerPlacment(struct striker_t *s);
int8_t getDeltaX(struct striker_t *s, struct wall_t *w);
#endif //STRIKER_H
