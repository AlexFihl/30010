#include "stm32f30x_conf.h"
#include "trigomentri.h"
#include "ansi.h"
#include <stdio.h>
#include "fix14.h"

#ifndef BLOCK_H
#define BLOCK_H

#define fullLife    178
#define midLife     177
#define lowLife     176
#define noLife      32

struct block_t {
    struct vector_t v1;
    struct vector_t v2;
    uint8_t state;
    uint8_t life;
    uint8_t hits;
};

void intBlock(struct block_t *b, struct vector_t *v_1, struct vector_t *v_2, uint8_t life);
void drawBlock(struct block_t *b);

#endif //BLOCK_H
