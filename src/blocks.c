#include "blocks.h"

void intBlock(struct block_t *b, struct vector_t *v_1, struct vector_t *v_2)
{
    b->v1.x = v_1->x;
    b->v1.y = v_1->y;
    b->v2.x = v_2->x;
    b->v2.y = v_2->y;
}

void drawBlock(struct block_t *b)
{
    uint16_t i, j;
    for(i = ((b->v1.y) >> FIX14_SHIFT); i <= ((b->v2.y) >> FIX14_SHIFT); i++)
    {
        gotoxy((b->v1.x >> FIX14_SHIFT), i);
        for(j = ((b->v1.x) >> FIX14_SHIFT); j <= ((b->v2.x) >> FIX14_SHIFT); j++)
            printf("%c", 178);
    }
}
