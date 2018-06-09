#include "customcharset.h"
#include "30010_io.h"
#include "lcd.h"
#include "io.h"

#ifndef MINIGAME_H
#define MINIGAME_H

extern uint8_t lcdBuffer[512];

struct minigame_t
{
    uint8_t shipLine;
    uint8_t oldShipLine;

};

struct obstacle_t
{
    uint8_t obstacleLine;
    uint32_t timeStart;
};

void drawSpaceship (struct minigame_t *s);
void intminigame(struct minigame_t *s);
void moveShip (struct minigame_t *s, int8_t deltaLine);
void playMinigame1();
#endif //MINIGAME_H


