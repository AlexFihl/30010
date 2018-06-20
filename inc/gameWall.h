#include "trigomentri.h"
#include "ansi.h"

#ifndef GAMEWALL_H
#define GAMEWALL_H

struct wall_t{
    struct vector_t v1;
    struct vector_t v2;
};

void intWall(struct wall_t *w, struct vector_t *v1, struct vector_t *v2);
void drawWall(struct wall_t *w);
//To be able to draw a window with a title

void window  (struct wall_t *w, char text[], uint8_t style);


#endif //GAMEWALL_H
