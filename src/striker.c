#include "striker.h"



void intStriker(struct striker_t *s)
{
    struct vector_t v;
    intVector(&v, 110, 62);
    s->center.x = v.x;
    s->center.y = v.y;
    s->oldCenter.x = v.x;
    s->length = 20 << FIX14_SHIFT;
}

void updateStriker(struct striker_t *s, uint32_t deltaCenter)
{
    s->center.x += deltaCenter;
}

void changeStrikerLength(struct striker_t *s, uint32_t deltaLength)
{
    s->length += deltaLength;
}

void drawStriker(struct striker_t *s)
{
    int i;
    if ((s->center.x >> FIX14_SHIFT) == (s->oldCenter.x >> FIX14_SHIFT))
    {
        gotoxy((s->oldCenter.x - FIX14_DIV(s->length,2) ) >> FIX14_SHIFT, s->center.y >> FIX14_SHIFT);
        for(i = 0; i < ((s->length) >> FIX14_SHIFT); i++)
            printf("%c", 32);
        gotoxy((s->center.x - FIX14_DIV(s->length,2) ) >> FIX14_SHIFT, s->center.y >> FIX14_SHIFT);
        for(i = 0; i < ((s->length) >> FIX14_SHIFT); i++)
            printf("%c", 219); //Ellers prøv 223
    }
}
