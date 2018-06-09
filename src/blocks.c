#include "blocks.h"

void intBlock(struct block_t *b, struct vector_t *v1, struct vector_t *v2, uint8_t life,  uint8_t color)
{
    b->v1.x = v1->x;
    b->v1.y = v1->y;
    b->v2.x = v2->x;
    b->v2.y = v2->y;
    b->state = 4;
    b->oldState = 5;
    b->hits = 0;
    b->life = life;
    b->color = color;
    b->pointGiver = 1;
}

void intMultipleBlocks(struct block_t ** blocks, struct vector_t v1, struct vector_t v2, uint8_t quantityX, uint8_t quantityY) //08/06
{
    struct vector_t v3, v4;
    uint32_t deltaX, deltaY;
    uint8_t i, j;
    deltaX = FIX14_DIV(((v2.x>>FIX14_SHIFT) - (v1.x>>FIX14_SHIFT) + 1) , quantityX);
    deltaY = FIX14_DIV(((v2.y>>FIX14_SHIFT) - (v1.y>>FIX14_SHIFT) + 1) , quantityY);
    for (i = 0; i < quantityY; i++)
    {
        uint32_t new1Y, new2Y;
        new1Y = ((deltaY * i) + v1.y)           >> FIX14_SHIFT;
        new2Y = ((deltaY * i) + v1.y + deltaY - 1)  >> FIX14_SHIFT;
        if(new2Y > (v2.y >> FIX14_SHIFT)) new2Y = (v2.y >> FIX14_SHIFT);
        for (j = 0; j < quantityX; j++)
        {
            struct block_t block;
            uint32_t new1X, new2X;
            new1X = ((deltaX * j) + v1.x)           >> FIX14_SHIFT;
            new2X = ((deltaX * j) + v1.x + deltaX - 1)  >> FIX14_SHIFT;
            if(new2X > (v2.x >> FIX14_SHIFT)) new2X = (v2.x >> FIX14_SHIFT);
            intVector(&v3, new1X, new1Y);
            intVector(&v4, new2X, new2Y);
            intBlock(&block, &v3, &v4, 4, j+1 + i);
            uint16_t k;
            k = i * quantityX;
            (*blocks)[j + k] = block;
        }
    }
}

void drawBlock(struct block_t *b) //08/06
{
    if(b->oldState != b->state)
    {
        fgcolor(b->color);
        uint16_t i, j;
        for(i = ((b->v1.y) >> FIX14_SHIFT); i <= ((b->v2.y) >> FIX14_SHIFT); i++)
        {
            gotoxy((b->v1.x >> FIX14_SHIFT), i);
            for(j = ((b->v1.x) >> FIX14_SHIFT); j <= ((b->v2.x) >> FIX14_SHIFT); j++)
                switch(b->state)
                {
                case(4):
                    printf("%c", fullLife);
                    break;
                case(3):
                    printf("%c", midHighLife);
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
        b->oldState = b->state;
        fgcolor(15);
    }
}


