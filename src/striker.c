#include "striker.h"



void intStriker(struct striker_t *s)
{
    struct vector_t v;
    intVector(&v, 110, 63);
    s->center = v;
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
    gotoxy((s->center.x - FIX14_DIV(s->length,2) ) >> FIX14_SHIFT, s->center.y >> FIX14_SHIFT);
    for(i = 0; i < ((s->length) >> FIX14_SHIFT); i++)
        printf("%c", 223); //Ellers prøv 219
}
