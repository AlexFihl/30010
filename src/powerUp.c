#include "powerUp.h"

const uint16_t signA[12] =
{
    43, 45, 49, 158, 249, 175, 174, 33, 159, 190, 241, 207
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
    resetbgcolor();
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

void printPowerupHelp()
{
    gotoxy(83, 22);
    printf("%c = longer striker", signA[0]);
    gotoxy(83, 23);
    printf("%c = shorter striker", signA[1]);
    gotoxy(83, 24);
    fgcolor(2);
    printf("%c", signA[2]);
    fgcolor(15);
    printf(" = one extra life");
    gotoxy(83, 25);
    printf("%c = instant death", signA[3]);
    gotoxy(83, 26);
    printf("%c = double balls", signA[4]);
    gotoxy(83, 27);
    printf("%c = faster ball", signA[5]);
    gotoxy(83, 28);
    printf("%c = slower ball", signA[6]);
    gotoxy(83, 29);
    printf("%c = instant win", signA[7]);
    gotoxy(83, 30);
    printf("%c = key to unlock mingame (3 is required)", signA[8]);
    gotoxy(83, 31);
    printf("%c = striker shoots", signA[9]);
    gotoxy(83, 32);
    printf("%c = catch ball on striker (multiple balls will disappear)", signA[10]);
    gotoxy(83, 33);
    printf("%c = teleport behind blocks", signA[11]);
}
