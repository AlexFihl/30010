#include "powerUp.h"

const uint16_t signA[13] = {
    43, 45, 49, 158, 249, 175, 174, 33, 159, 190, 167, 207
};

void initPowerUp(struct powerUp_t *p, struct vector_t *v, uint8_t powerUpType)
{
    p->v.x = v->x;
    p->v.y = v->y;
    p->old.x = v->x;
    p->old.y = v->y;
    p->downSpeed = 1 << FIX14_SHIFT;
    p->catched = 0;
    p->dead = 0;
    p->type = powerUpType;
    p->sign = signA[powerUpType];
}

void drawPowerUp(struct powerUp_t *p, struct block_t * b, uint16_t lowerBond, uint32_t numberOfBlocks)
{
    if(p->type == 2)
        fgcolor(2);
    if(p->v.y > lowerBond)
    {
        gotoxy(p->old.x >> FIX14_SHIFT, p->old.y >> FIX14_SHIFT);
        if(p->old.y > lowerBond)
            printf("%c", 32);
        gotoxy(p->v.x >> FIX14_SHIFT, p->v.y >> FIX14_SHIFT);
        if(p->catched == 0 && p->dead == 0)
            printf("%c", p->sign);
    }
    else
    {
        uint16_t j;
        for(j=0; j < numberOfBlocks; j++)
        {

            if(p->old.x > b[j].v1.x && p->old.x < b[j].v2.x && p->old.y > b[j].v1.y && p->old.y < b[j].v2.y && b[j].state == 0)
            {
                gotoxy(p->old.x >> FIX14_SHIFT, p->old.y >> FIX14_SHIFT);
                printf("%c", 32);
            }
            if(p->v.x > b[j].v1.x && p->v.x < b[j].v2.x && p->v.y > b[j].v1.y && p->v.y < b[j].v2.y && p->catched == 0 && b[j].state == 0)
            {
                gotoxy(p->v.x >> FIX14_SHIFT, p->v.y >> FIX14_SHIFT);
                printf("%c", p->sign);
            }

        }
    }
    resetbgcolor();
}

void applyPowerUp(struct powerUp_t *p, struct striker_t *s, struct wall_t *w, struct ball_t *b, struct player_t *pl, int8_t * ballOnStriker, int8_t *skipLevel)
{
    if(p->catched == 1)
    {
        switch (p->type)
        {
        case 0x0:
            changeStrikerLength(s, 4, w);
            break;
        case 0x1:
            changeStrikerLength(s, -4, w);
            break;
        case 0x2:
            gainLife(pl);
            break;
        case 0x3:
            lossLife(pl);
            break;
        case 0x4:
            break;
        case 0x5:
            addToBallSpeedFactor(0x00000800); //=0.125
            break;
        case 0x6:
            addToBallSpeedFactor(-0x00000800);//=-0.125
            break;
        case 0x7:
            *skipLevel = 1;
            break;
        case 0x8:
            pl->catchKeys++;
            break;
        case 0x9:
            break;
        case 0xA:
            (*ballOnStriker) = 1;
            break;
        case 0xB:
            teleportBall(b);
            break;
        }

    }
}

void updatePowerUp(struct powerUp_t *p, struct striker_t *s, struct wall_t *w)
{
    p->old.y = p->v.y;
    uint32_t newY;
    newY = p->v.y + p->downSpeed;
    int32_t hl = FIX14_DIV((s->length >> FIX14_SHIFT), 2);
    if(p->v.x > (s->center.x - hl) && p->v.x < (s->center.x + hl) && (s->center.y >> FIX14_SHIFT) == (newY >> FIX14_SHIFT))
        p->catched = 1;
    else if(p->v.y >= w->v2.y)
        p->dead = 1;
    else
        p->v.y = newY;
}
