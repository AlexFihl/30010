#include "stm32f30x_conf.h"
#include <stdio.h>

#ifndef TRIGOMENTRI_H
#define TRIGOMENTRI_H

#include "fix14.h"

// === LUT SIZES ===
#define SIN_SIZE 512

// === LUT DECLARATIONS ===

struct vector_t
{
    int32_t x, y;
};

int32_t sinn(int32_t i);
int32_t coss(int32_t i);
void intVector(struct vector_t *v, int32_t x, int32_t y);

#endif	// TRIGOMENTRI_H
