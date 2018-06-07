#include "ball.h"


void intBall(struct ball_t *b, int32_t x, int32_t y, int32_t vx, int32_t vy)
{
    intVector(&(b->position), x, y);
    b->oldPos.x = b->position.x;
    b->oldPos.y = b->position.y;
    intVector(&(b->velocity), vx, vy);
    b->hitCount = 0;
}

void updatePosition(struct ball_t *b, uint8_t wallx1, uint8_t wally1, uint8_t wallx2, uint8_t wally2)
{
    b->oldPos.x = b->position.x;
    b->oldPos.y = b->position.y;
    int32_t newX = b->position.x + FIX14_MULT(b->velocity.x, speed);
    int32_t newY = b->position.y + FIX14_MULT(b->velocity.y, speed);
    if (newX < ((wallx1 + 1) << FIX14_SHIFT) || newX >= (wallx2 << FIX14_SHIFT))
    {
        b->velocity.x = -b->velocity.x;
        b->position.x += FIX14_MULT(b->velocity.x, speed);
        b->hitCount++;
    } else b->position.x = newX;
    if (newY < ((wally1 + 1) << FIX14_SHIFT) || newY >= (wally2 << FIX14_SHIFT))
    {
        b->velocity.y = -b->velocity.y;
        b->position.y += FIX14_MULT(b->velocity.y, speed);
        b->hitCount++;
    } else b->position.y = newY;
}

