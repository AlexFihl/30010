#include "player.h"

void intPlayer(struct player_t *p) //08/06
{
    p->score = 0;
    p->life = 3;
    p->name = malloc(11 * sizeof(char));
    p->name[0] = '\0';
    p->catchKeys = 0;
}

void setPlayerName(struct player_t *p, char * name)
{
    uint8_t i;
    for (i = 0; i < 11; i++)
        if(name[i] == '\0')
            break;
        else
            (p->name)[i] = name[i];
    p->name[i] = '\0';
}

void lossLife(struct player_t *p) {(p->life)--;} //08/06
void gainLife(struct player_t *p) {(p->life)++;} //08/06
void setPlayerLife(struct player_t *p, int8_t life) {p->life = life;}

void resetPlayer(struct player_t *p)
{
    p->score = 0;
    p->life = 3;
    p->catchKeys = 0;
    p->name = "Player\0";
}
