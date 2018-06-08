#include "customcharset.h"
#include "30010_io.h"
#include "lcd.h"
#include "io.h"

#ifndef MINIGAME_H
#define MINIGAME_H

struct minigame_t
{
    uint8_t shipLine;
    uint8_t oldShipLine;

};


void temp ();
void drawSpaceship (struct minigame_t *s);
void intminigame(struct minigame_t *s);
void moveShip (struct minigame_t *s, int8_t deltaLine);

#endif //MINIGAME_H


