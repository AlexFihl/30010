#include "player.h"

void intPlayer(struct player_t *p) //08/06
{
    p->score = 0;
    p->life = 3;
    p->name = "";
}

void lossLife(struct player_t *p) {(p->life)--;} //08/06
void gainLife(struct player_t *p) {(p->life)++;} //08/06
