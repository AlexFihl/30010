#include "striker.h"



void intStriker(struct striker_t *s, int8_t deltaStrikerStart)
{
    struct vector_t v;
    intVector(&v, 110, 61);
    s->center.x = v.x;
    s->center.y = v.y;
    s->oldCenter.x = v.x-1;
    s->length = (20 + deltaStrikerStart) << FIX14_SHIFT;
    s->oldLength = (19 + deltaStrikerStart) << FIX14_SHIFT;
}

void updateStriker(struct striker_t *s, int32_t deltaCenter)
{
    s->center.x += (deltaCenter << FIX14_SHIFT);
}

void changeStrikerLength(struct striker_t *s, int32_t deltaLength, struct wall_t *w)
{
    s->length += (deltaLength << FIX14_SHIFT);
    if(((s->center.x - (s->length >> 1)) >> FIX14_SHIFT) <= (w->v1.x >> FIX14_SHIFT))
        s->center.x += (s->length >> 1) - (s->center.x - w->v1.x ) + (0x00004000);   //0x00004000 = 1
    if(((s->center.x + (s->length >> 1)) >> FIX14_SHIFT) >= (w->v2.x >> FIX14_SHIFT))
        s->center.x -= (s->length >> 1) - (w->v2.x - s->center.x) + (0x00004000);   //0x00004000 = 1
}

void drawStriker(struct striker_t *s)
{
    int i;
    if ((s->center.x >> FIX14_SHIFT) != (s->oldCenter.x >> FIX14_SHIFT) || (s->length >> FIX14_SHIFT) != (s->oldLength >> FIX14_SHIFT) || s->boss>0)
    {
        gotoxy((s->oldCenter.x - (s->oldLength >> 1) ) >> FIX14_SHIFT, s->center.y >> FIX14_SHIFT);
        for(i = 0; i < ((s->oldLength) >> FIX14_SHIFT); i++)
            printf("%c", 32);
        gotoxy((s->center.x - (s->length >> 1) ) >> FIX14_SHIFT, s->center.y >> FIX14_SHIFT);
        for(i = 0; i < ((s->length) >> FIX14_SHIFT); i++)
            printf("%c", 219); //Ellers prøv 223
        s->oldCenter.x = s->center.x;
        s->oldLength = s->length;
        s->boss=0;
    }
}

void resetStriker(struct striker_t *s)
{
    s->center.x = 110 << FIX14_SHIFT;
}

int8_t updateStrikerPlacment(struct striker_t *s)
{
    uint8_t currentJoyStick = readJoyStick();
    if      ((currentJoyStick & 0x04) == 0x04) //When clicking the left button
    {
        return -2;
    }
    else if ((currentJoyStick & 0x08) == 0x08) //When clicking the right button
    {
         return 2;
    }
    return 0;
}

int8_t getDeltaX(struct striker_t *s, struct wall_t *w)
{
    int8_t deltaX = updateStrikerPlacment(s);
    int32_t hl = s->length >> 1;
    if(deltaX + s->center.x + hl >= (w->v2.x - 0x00004000)) //0x00004000 = 1
        if(deltaX > 0)
            deltaX = 0;
    if(s->center.x + deltaX - hl < (w->v1.x + 0x00004000))  //0x00004000 = 1
        if(deltaX < 0)
            deltaX = 0;
    return deltaX;
}
