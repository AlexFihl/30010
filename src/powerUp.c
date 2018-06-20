#include "powerUp.h"

const uint16_t signA[12] =
{
    43, 45, 49, 158, 249, 175, 174, 33, 159, 190, 241, 207
};
const uint8_t colorA[12] =
{
    2, 1, 2, 1, 2, 1, 2, 2, 2, 2, 2, 2
};

void initPowerUp(struct powerUp_t *p, struct vector_t *v, uint8_t powerUpType)
{
    p->v.x = v->x;
    p->v.y = v->y;
    p->old.x = v->x;
    p->old.y = v->y;
    p->catched = 0;
    p->dead = 0;
    p->type = powerUpType;
    p->sign = signA[powerUpType];
    p->color = colorA[powerUpType];
}

void drawPowerUp(struct powerUp_t *p, struct block_t * b, uint16_t lowerBond, uint32_t numberOfBlocks)
{
    fgcolor(p->color);
    if(p->v.y > (lowerBond<< FIX14_SHIFT))
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
            if(p->old.x >= b[j].v1.x && p->old.x <= b[j].v2.x && p->old.y >= b[j].v1.y && p->old.y <= b[j].v2.y + 0x4000 && b[j].state == 0)
            {
                gotoxy(p->old.x >> FIX14_SHIFT, p->old.y >> FIX14_SHIFT);
                printf("%c", 32);
            }
            if(p->v.x >= b[j].v1.x && p->v.x <= b[j].v2.x && p->v.y >= b[j].v1.y && p->v.y <= b[j].v2.y + 0x4000 && p->catched == 0 && b[j].state == 0)
            {
                gotoxy(p->v.x >> FIX14_SHIFT, p->v.y >> FIX14_SHIFT);
                printf("%c", p->sign);
            }

        }
    }
    fgcolor(15);
}

void updatePowerUp(struct powerUp_t *p, struct striker_t *s, struct wall_t *w)
{
    p->old.y = p->v.y;
    uint32_t newY;
    newY = p->v.y + 0x00004000; // = 1
    int32_t hl = FIX14_DIV((s->length >> FIX14_SHIFT), 2);
    if(p->v.x > (s->center.x - hl) && p->v.x < (s->center.x + hl) && (s->center.y >> FIX14_SHIFT) == (newY >> FIX14_SHIFT))
        p->catched = 1;
    else if(p->v.y >= w->v2.y)
        p->dead = 1;
    else
        p->v.y = newY;
}

void applyPowerUp(struct powerUp_t *p, struct striker_t *s, struct wall_t *w, struct player_t *pl, int8_t * ballOnStriker, int8_t *skipLevel, int8_t *strikerShoting, int8_t *multiplyBalls, int8_t *telePortBallFlag)
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
            *multiplyBalls = 1;
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
            *strikerShoting = 1;
            break;
        case 0xA:
            *ballOnStriker = 1;
            break;
        case 0xB:
            *telePortBallFlag = 1;
            break;
        }

    }
}

void initStrikerShooting (struct strikerShooting_t *s, struct striker_t *st)
{
    int8_t i;
    for(i=0; i<10; i++)
    {
        s->isAlive[i] = 0;
        s->position[i].y = st->center.y - 0x00004000; //=1
        s->oldPosition[i].y = st->center.y - 0x00008000;
    }
    s->amountOfBalls = 0;
    s->speed = 1 << FIX14_SHIFT;
    s->shootSpeed = 10;
}

void shotCaller(struct strikerShooting_t *s, struct striker_t *st, uint8_t shotNumber)
{
    s->position[shotNumber].x = st->center.x;
    s->oldPosition[shotNumber].x = st->center.x;
    s->position[shotNumber].x = st->center.x;
    s->isAlive[shotNumber] = 1;
}

void updateShot(struct wall_t *w, struct block_t ** blocks, uint16_t numberOfBlocks, struct player_t *p, struct striker_t *s, struct strikerShooting_t *u, uint8_t shotNumber)
{
    uint32_t wally1;
    uint8_t i;
    wally1 = (w->v1.y) >> FIX14_SHIFT;
    int32_t newY = u->position[shotNumber].y - u->speed;

    //Checking that it hits the top

    if (newY <= ((wally1 + 1) << FIX14_SHIFT))
    {
        setFreq(3300);
        u->isAlive[shotNumber]=0;
    }

    //Checking if it hits a block
    for(i = 0; i < numberOfBlocks; i++)
    {
        struct block_t block;
        block = (*blocks)[i];
        if(block.state > 0)
        {
            if (newY >= block.v1.y && newY <= (block.v2.y + (1 << 14)) && u->position[shotNumber].x >= block.v1.x && u->position[shotNumber].x <= (block.v2.x + (1 << 14)))
            {
                setFreq(1300);
                u->isAlive[shotNumber]=0;
                (((*blocks)[i]).hits) = block.life;
            }
        }
    }


    u->position[shotNumber].y = newY;
}

void drawShot(struct strikerShooting_t *u, uint8_t shotNumber)
{
    if((u->isAlive[shotNumber] == 0) /*&& ((u->oldPosition[shotNumber].y >> FIX14_SHIFT) != (u->position[shotNumber].y >> FIX14_SHIFT))*/)
    {
        gotoxy((u->oldPosition[shotNumber].x) >> FIX14_SHIFT, (u->oldPosition[shotNumber].y) >> FIX14_SHIFT);
        printf("%c", 32);
        gotoxy((u->position[shotNumber].x) >> FIX14_SHIFT, (u->position[shotNumber].y) >> FIX14_SHIFT);
        printf("%c", 32);
    }
    else if ((u->oldPosition[shotNumber].y >> FIX14_SHIFT) != (u->position[shotNumber].y >> FIX14_SHIFT))
    {
        gotoxy((u->oldPosition[shotNumber].x) >> FIX14_SHIFT, (u->oldPosition[shotNumber].y) >> FIX14_SHIFT);
        printf("%c", 32);
        gotoxy((u->position[shotNumber].x) >> FIX14_SHIFT, (u->position[shotNumber].y) >> FIX14_SHIFT);
        printf("%c", 111);
        u->oldPosition[shotNumber].x = u->position[shotNumber].x;
        u->oldPosition[shotNumber].y = u->position[shotNumber].y;
    }
}

void printPowerupHelp()
{
    gotoxy(83, 22);
    fgcolor(colorA[0]);
    printf("%c", signA[0]);
    fgcolor(15);
    printf(" = longer striker");
    gotoxy(83, 23);
    fgcolor(colorA[1]);
    printf("%c", signA[1]);
    fgcolor(15);
    printf(" = shorter striker");
    gotoxy(83, 24);
    fgcolor(colorA[2]);
    printf("%c", signA[2]);
    fgcolor(15);
    printf(" = one extra life");
    gotoxy(83, 25);
    fgcolor(colorA[3]);
    printf("%c", signA[3]);
    fgcolor(15);
    printf(" = instant death");
    gotoxy(83, 26);
    fgcolor(colorA[4]);
    printf("%c", signA[4]);
    fgcolor(15);
    printf(" = double balls");
    gotoxy(83, 27);
    fgcolor(colorA[5]);
    printf("%c", signA[5]);
    fgcolor(15);
    printf(" = faster ball");
    gotoxy(83, 28);
    fgcolor(colorA[6]);
    printf("%c", signA[6]);
    fgcolor(15);
    printf(" = slower ball");
    gotoxy(83, 29);
    fgcolor(colorA[7]);
    printf("%c", signA[7]);
    fgcolor(15);
    printf(" = instant win");
    gotoxy(83, 30);
    fgcolor(colorA[8]);
    printf("%c", signA[8]);
    fgcolor(15);
    printf(" = key to unlock mingame (3 is required)");
    gotoxy(83, 31);
    fgcolor(colorA[9]);
    printf("%c", signA[9]);
    fgcolor(15);
    printf(" = striker shoots");
    gotoxy(83, 32);
    fgcolor(colorA[10]);
    printf("%c", signA[10]);
    fgcolor(15);
    printf(" = catch ball on striker (multiple balls will disappear)");
    gotoxy(83, 33);
    fgcolor(colorA[11]);
    printf("%c", signA[11]);
    fgcolor(15);
    printf(" = teleport behind block");
}
