#include "player.h"

void intPlayer(struct player_t *p)
{
    p->score = 0;
    p->life = 3;
    p->name = "";
}

void lossLife(struct player_t *p) {(p->life)--;}
void gainLife(struct player_t *p) {(p->life)++;}
