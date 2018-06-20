#include "charset.h"
#include "30010_io.h"
#include "lcd.h"
#include "io.h"
#include "time.h"

#ifndef MINIGAME_H
#define MINIGAME_H


struct minigame_t
{
    uint8_t shipLine;
    uint8_t oldShipLine;
    uint32_t timeSinceStart;
    uint8_t obstacleBuffer[64];
    uint8_t life;
    uint8_t shootIsAlive;
    uint8_t shootLine;
    uint8_t shotsLeft;
    uint8_t chanceOfSpawn;
};

struct obstacle_t
{
    uint8_t obstacleLine;
    uint32_t timeStart;
    uint8_t isAlive;
};

void drawSpaceship (struct minigame_t *s);
void intminigame(struct minigame_t *s);
void moveShip (struct minigame_t *s, int8_t deltaLine);
uint32_t playMinigame1();

#endif //MINIGAME_H


