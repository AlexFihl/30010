#include "ball.h"

int32_t ballSpeed;

void intBall(struct ball_t *b, int32_t x, int32_t y, int32_t vx, int32_t vy)
{
    intVector(&(b->position), x, y);
    //b->oldPos.x = b->position.x;
    //b->oldPos.y = b->position.y;
    b->velocity = 2 << FIX14_SHIFT;
    b->angle = 0; //-45 deg
    b->hitCount = 0;
}

void setBallSpeedFactor(int32_t speedFactor) //09/06
{
    ballSpeed = speedFactor;
}

void addToBallSpeedFactor(int32_t deltaSpeed)
{
    ballSpeed += deltaSpeed;
    if(ballSpeed <= 0x00000000)
        ballSpeed = 0x00000800;
}

void moveBall(struct ball_t *b, int32_t deltaX, int32_t deltaY) //09/06
{
    b->position.x += deltaX;
    b->position.y += deltaY;
}

void teleportBall(struct ball_t *b)
{
    b->position.y = 5 << FIX14_SHIFT;
}

static int32_t getXVel(struct ball_t *b) //09/06
{
    int32_t rad = sinn(b->angle);
    int32_t vel = b->velocity;
    return FIX14_MULT(FIX14_MULT(vel, rad), ballSpeed);
}

static int32_t getYVel(struct ball_t *b) //09/06
{
    int32_t rad = -coss(b->angle);
    int32_t vel = b->velocity;
    return FIX14_MULT(FIX14_MULT(vel, rad), ballSpeed);
}

void updatePosition(struct ball_t *b, struct wall_t *w, struct block_t ** blocks, uint16_t numberOfBlocks, struct player_t *p, struct striker_t *s)
{
    uint32_t wallx1, wallx2, wally1, wally2;
    uint8_t i;
    wallx1 = (w->v1.x) >> FIX14_SHIFT;
    wally1 = (w->v1.y) >> FIX14_SHIFT;
    wallx2 = (w->v2.x) >> FIX14_SHIFT;
    wally2 = (w->v2.y) >> FIX14_SHIFT;
    int32_t newX = b->position.x + getXVel(b);
    int32_t newY = b->position.y + getYVel(b);
    //Checking that it hits the wall
    if (newX < ((wallx1 + 1) << FIX14_SHIFT) || newX >= (wallx2 << FIX14_SHIFT))
    {
        setFreq(3000);
        b->angle =  -b->angle;
        newX = b->position.x + getXVel(b);
        b->hitCount++;
    }
    if (newY < ((wally1 + 1) << FIX14_SHIFT))
    {
        setFreq(3000);
        b->angle =  256 - b->angle;
        newY = b->position.y + getYVel(b);
        b->hitCount++;
    }
    else if (newY >= (wally2 << FIX14_SHIFT))
    {
        lossLife(p);
        return;
    }

    //Checking if it hits a block
    for(i = 0; i < numberOfBlocks; i++)
    {
        struct block_t block;
        block = (*blocks)[i];
        if(block.state > 0)
        {
            if (newX >= block.v1.x && newX <= (block.v2.x + (1 << 14)) && newY >= block.v1.y && newY <= (block.v2.y + (1 << 14)))
            {
                //setFreq(1000);
                if(block.v1.x >= b->oldPos.x || (block.v2.x + (1 << 14)) <= b->oldPos.x)
                {
                    b->angle =  - b->angle;
                    newX = b->position.x + getXVel(b);
                    b->hitCount++;
                }
                if(block.v1.y >= b->oldPos.y || (block.v2.y  + (1 << 14)) <= b->oldPos.y)
                {
                    b->angle = 256 - b->angle;
                    newY = b->position.y + getYVel(b);
                    b->hitCount++;
                }
                (((*blocks)[i]).hits)++;
                if (FIX14_DIV((*blocks)[i].hits, block.life) >= 0x00004000)
                {
                     (((*blocks)[i]).state) = 0;
                     p->score += (*blocks)[i].pointGiver;
                }
                else if (FIX14_DIV((*blocks)[i].hits, block.life) >= 0x00003000)
                     (((*blocks)[i]).state) = 1;
                else if (FIX14_DIV((*blocks)[i].hits, block.life) >= 0x00002000)
                     (((*blocks)[i]).state) = 2;
                else if (FIX14_DIV((*blocks)[i].hits, block.life) >= 0x00001000)
                     (((*blocks)[i]).state) = 3;
                else (((*blocks)[i]).state) = 4;

            }
        }
    }
    //Checking the striker
    int32_t lenghtHalf = (s->length >> FIX14_SHIFT) >> 2;
    if ((newX >= s->center.x - lenghtHalf) && (newX <= s->center.x + lenghtHalf) && (newY >= s->center.y - 0x00003000)) //0x00003000 = 0.750
    {
        //setFreq(10000);
        if(newX > s->center.x - (s->length >> 1) && newX < s->center.x + (s->length >> 1)) //Checking the middle
        {
            int32_t y=s->length;
            b->angle =  256 - b->angle + (((newX - s->center.x) *100 )/(y >> 1));
        }
        while (b->angle > 256 && b->angle < -256)
        {
            if (b->angle >= 256)
                b->angle -= 512;
            if (b->angle <= -256)
                b->angle += 512;
        }
        if (b->angle <= -128) b->angle = -120;
        else if (b->angle >= 128) b->angle = 120;
        newX = b->position.x + getXVel(b);
        newY = b->position.y + getYVel(b);
        b->hitCount++;
    }
    b->position.x = newX;
    b->position.y = newY;
}

void resetBall(struct ball_t *b) //09/06
{
    intVector(&(b->position), 110, 60);
    b->velocity = 2 << FIX14_SHIFT;
    b->angle = 0; //Sholud be 0* when the striker can change the angle
}

void drawBall(struct ball_t *b)
{
    if ((b->oldPos.x >> FIX14_SHIFT) != (b->position.x >> FIX14_SHIFT) || (b->oldPos.y >> FIX14_SHIFT) != (b->position.y >> FIX14_SHIFT))
    {
        gotoxy((b->oldPos.x) >> FIX14_SHIFT, (b->oldPos.y) >> FIX14_SHIFT);
        printf("%c", 32);
        gotoxy((b->position.x) >> FIX14_SHIFT, (b->position.y) >> FIX14_SHIFT);
        printf("%c", 111);
        b->oldPos.x = b->position.x;
        b->oldPos.y = b->position.y;
    }
}
