#include "striker.h"



void intStriker(struct striker_t *s) //08/06
{
    struct vector_t v;
    intVector(&v, 110, 61);
    s->center.x = v.x;
    s->center.y = v.y;
    s->oldCenter.x = v.x-1;
    s->length = 20 << FIX14_SHIFT;
    s->oldLength = 19 << FIX14_SHIFT;
}

void updateStriker(struct striker_t *s, int32_t deltaCenter) //08/06
{
    s->center.x += (deltaCenter << FIX14_SHIFT);
}

void changeStrikerLength(struct striker_t *s, int32_t deltaLength) //08/06
{
    s->length += (deltaLength << FIX14_SHIFT);
}

void drawStriker(struct striker_t *s) //08/06
{
    int i;
    if ((s->center.x >> FIX14_SHIFT) != (s->oldCenter.x >> FIX14_SHIFT) || (s->length >> FIX14_SHIFT) != (s->oldLength >> FIX14_SHIFT))
    {
        gotoxy((s->oldCenter.x - (s->oldLength >> 1) ) >> FIX14_SHIFT, s->center.y >> FIX14_SHIFT);
        for(i = 0; i < ((s->oldLength) >> FIX14_SHIFT); i++)
            printf("%c", 32);
        gotoxy((s->center.x - (s->length >> 1) ) >> FIX14_SHIFT, s->center.y >> FIX14_SHIFT);
        for(i = 0; i < ((s->length) >> FIX14_SHIFT); i++)
            printf("%c", 219); //Ellers prøv 223
        s->oldCenter.x = s->center.x;
        s->oldLength = s->length;
    }
}

void resetStriker(struct striker_t *s) //09/06
{
    struct vector_t v;
    intVector(&v, 110, 61);
    s->center.x = v.x;
    s->center.y = v.y;
}

int8_t updateStrikerPlacment(struct striker_t *s) //09/06
{
    uint8_t currentJoyStick = readJoyStick();
    if      ((currentJoyStick & 0x04) == 0x04) //When clicking the left button
    {
        updateStriker(s, -2);
        return -2;
    }
    else if ((currentJoyStick & 0x08) == 0x08) //When clicking the right button
    {
         updateStriker(s, 2);
         return 2;
    }
}
