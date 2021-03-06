#include "stm32f30x_conf.h"
#include "trigomentri.h"
#include "ansi.h"
#include <stdio.h>
#include "fix14.h"

#ifndef BLOCK_H
#define BLOCK_H

#define fullLife    219
#define midHighLife 178
#define midLife     177
#define lowLife     176
#define noLife      32

struct block_t {
    struct vector_t v1;
    struct vector_t v2;
    uint8_t state;
    uint8_t oldState;
    uint8_t life;
    uint8_t hits;
    uint8_t color;
    uint8_t boss;
};
//Taking from ANSI
/*  Value      foreground     Value     foreground
        ------------------------------------------------
          0        Black            8       Dark Gray
          1        Red              9       Light Red
          2        Green           10       Light Green
          3        Brown           11       Yellow
          4        Blue            12       Light Blue
          5        Purple          13       Light Purple
          6        Cyan            14       Light Cyan
          7        Light Gray      15       White
*/

void intBlock(struct block_t *b, struct vector_t *v1, struct vector_t *v2, uint8_t life, uint8_t color);
uint16_t intMultipleBlocks(struct block_t ** blocks, struct vector_t v1, struct vector_t v2, uint8_t quantityX, uint8_t quantityY, uint8_t lifeOnBlocks);
void drawBlock(struct block_t *b);
void updateBlockState(struct block_t *b);

#endif //BLOCK_H
