#include "ball.h"


void intBall(struct ball_t *b, int32_t x, int32_t y, int32_t vx, int32_t vy)
{
    intVector(&(b->position), x, y);
    b->oldPos.x = b->position.x;
    b->oldPos.y = b->position.y;
    intVector(&(b->velocity), vx, vy);
    b->hitCount = 0;
}

void updatePosition(struct ball_t *b, struct wall_t *w, struct block_t ** blocks, uint16_t numberOfBlocks)
{
    uint32_t wallx1, wallx2, wally1, wally2;
    wallx1 = (w->v1.x) >> FIX14_SHIFT;
    wally1 = (w->v1.y) >> FIX14_SHIFT;
    wallx2 = (w->v2.x) >> FIX14_SHIFT;
    wally2 = (w->v2.y) >> FIX14_SHIFT;
    b->oldPos.x = b->position.x;
    b->oldPos.y = b->position.y;
    int32_t newX = b->position.x + FIX14_MULT(b->velocity.x, speed);
    int32_t newY = b->position.y + FIX14_MULT(b->velocity.y, speed);
    //Checking that it hits the wall
    if (newX < ((wallx1 + 1) << FIX14_SHIFT) || newX >= (wallx2 << FIX14_SHIFT))
    {
        b->velocity.x = -b->velocity.x;
        newX = b->position.x + FIX14_MULT(b->velocity.x, speed);
        b->hitCount++;
    }
    if (newY < ((wally1 + 1) << FIX14_SHIFT))
    {
        b->velocity.y = -b->velocity.y;
        newY = b->position.y + FIX14_MULT(b->velocity.y, speed);
        b->hitCount++;
    } else if (newY >= (wally2 << FIX14_SHIFT))
    {
        b->velocity.y = -b->velocity.y;
        newY = b->position.y + FIX14_MULT(b->velocity.y, speed);
        b->hitCount++;
    }

    //Checking if it hits a block


    b->position.x = newX;
    b->position.y = newY;
}

void drawBall(struct ball_t *b)
{
    if ((b->oldPos.x >> FIX14_SHIFT) != (b->position.x >> FIX14_SHIFT) || (b->oldPos.y >> FIX14_SHIFT) != (b->position.y >> FIX14_SHIFT))
    {
        gotoxy((b->oldPos.x) >> FIX14_SHIFT, (b->oldPos.y) >> FIX14_SHIFT);
        printf("%c", 32);
        gotoxy((b->position.x) >> FIX14_SHIFT, (b->position.y) >> FIX14_SHIFT);
        printf("%c", 111);
    }
}
