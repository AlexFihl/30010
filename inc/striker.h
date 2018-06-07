#include "stm32f30x_conf.h"
#include "trigomentri.h"
#include "ansi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FIX14_SHIFT 14
#define FIX14_MULT(a, b) ( (a)*(b) >> FIX14_SHIFT )
#define FIX14_DIV(a, b) ( (a) << FIX14_SHIFT / (b) )

#ifndef STRIKER_H
#define STRIKER_H

struct striker_t {
    struct vector_t center;
    uint32_t length;
};

void intStriker(struct striker_t *s);
void updateStriker(struct striker_t *s, uint32_t deltaCenter);
void changeStrikerLength(struct striker_t *s, uint32_t deltaLength);
void drawStriker(struct striker_t *s);

#endif //STRIKER_H
