#include "stm32f30x_conf.h"
#include <stdio.h>

#ifndef TRIGOMENTRI_H
#define TRIGOMENTRI_H

#define FIX14_SHIFT 14
#define FIX14_MULT(a, b) ( (a)*(b) >> FIX14_SHIFT )
#define FIX14_DIV(a, b) ( (a) << FIX14_SHIFT / (b) )

// === LUT SIZES ===
#define SIN_SIZE 512

// === LUT DECLARATIONS ===

struct vector_t
{
    int32_t x, y;
};

extern const signed short SIN[512];
void printFix(int32_t i);
int32_t expand(int32_t i);
int32_t sin(int32_t i);
int32_t cos(int32_t i);
void intVector(struct vector_t *v, int32_t x, int32_t y);
void rotate(struct vector_t *v, int32_t angle);


#endif	// TRIGOMENTRI_H
