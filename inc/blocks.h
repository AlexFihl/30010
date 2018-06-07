#include "stm32f30x_conf.h"
#include "trigomentri.h"
#include "ansi.h"
#include <stdio.h>
#include "fix14.h"

#ifndef BLOCK_H
#define BLOCK_H

struct block_t {
    struct vector_t v1;
    struct vector_t v2;
};

void intBlock(struct block_t *b, struct vector_t *v_1, struct vector_t *v_2);
void drawBlock(struct block_t *b);

#endif //BLOCK_H
