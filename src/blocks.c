#include "blocks.h"

void intBlock(struct block_t *b, struct vector_t *v_1, struct vector_t *v_2, uint8_t life,  uint8_t color)
{
    b->v1.x = v_1->x;
    b->v1.y = v_1->y;
    b->v2.x = v_2->x;
    b->v2.y = v_2->y;
    b->state = 3;
    b->hits = 0;
    b->life = life;
    b->color = color;
}

void drawBlock(struct block_t *b)
{
    fgcolor(b->color);
    uint16_t i, j;
    for(i = ((b->v1.y) >> FIX14_SHIFT); i <= ((b->v2.y) >> FIX14_SHIFT); i++)
    {
        gotoxy((b->v1.x >> FIX14_SHIFT), i);
        for(j = ((b->v1.x) >> FIX14_SHIFT); j <= ((b->v2.x) >> FIX14_SHIFT); j++)
            switch(b->state)
            {
            case(3):
                printf("%c", fullLife);
                break;
            case(2):
                printf("%c", midLife);
                break;
            case(1):
                printf("%c", lowLife);
                break;
            case(0):
                printf("%c", noLife);
                break;
            }
    }
    fgcolor(15);
}
