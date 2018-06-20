#include "gameWall.h"


void intWall(struct wall_t *w, struct vector_t *v1, struct vector_t *v2) //08/06
{
    w->v1.x = v1->x;
    w->v1.y = v1->y;
    w->v2.x = v2->x;
    w->v2.y = v2->y;
}

void drawWall(struct wall_t *w) //The same style as the windows style 0  //08/06
{
    uint8_t i;
    gotoxy((w->v1.x) >> FIX14_SHIFT, (w->v1.y) >> FIX14_SHIFT);
    printf("%c", 201);
    for(i = ((w->v1.x) >> FIX14_SHIFT) + 1; i < ((w->v2.x) >> FIX14_SHIFT); i++)
        printf("%c", 205);
    printf("%c", 187);
    for (i = ((w->v1.y) >> FIX14_SHIFT) + 1; i <= ((w->v2.y) >> FIX14_SHIFT); i++) //Print the vertical lines
    {
        gotoxy((w->v1.x) >> FIX14_SHIFT,i);
        printf("%c", 186);
        gotoxy((w->v2.x) >> FIX14_SHIFT,i);
        printf("%c", 186);
    }
}

void window(struct wall_t *w, char text[], uint8_t style)
{
    uint32_t x1, y1, x2, y2;
    x1 = (w->v1.x) >> FIX14_SHIFT;
    y1 = (w->v1.y) >> FIX14_SHIFT;
    x2 = (w->v2.x) >> FIX14_SHIFT;
    y2 = (w->v2.y) >> FIX14_SHIFT;
    uint8_t topLenght = 6 + strlen(text);
    uint8_t spaces1, spaces2;
    if (x2-x1+1 <= 6)
    {
        spaces1 = 0;
        spaces2 = 0;
    }
    else
    {
        int16_t x = (x2 - x1 + 1) - topLenght;
        if (x >= 0)
        {
            spaces1 = x/2 + 1;
            spaces2 = (x%2 == 1) ? x/2 + 2 : x/2 + 1;
        }
        else
        {
            spaces1 = 0;
            spaces2 = x >= -1 ? 1 : 0;
        }
    }

    uint8_t rhC,lhC,llC,rlC,hC,vC,lC,rC;
    switch(style)
    {
    case 1: //Single line With words
    case 3:
        rhC = 191; // The upper right corner
        lhC = 218; // The upper left corner
        llC = 192; // The lower left corner
        rlC = 217; // The lower right corner
        vC = 179; // Vertical line
        hC = 196; // Horizontal line
        lC = 180; // Right hand bar of text
        rC = 195; // Left hand bar of text
        break;
    case 2:
    default: //Double lines
        rhC = 187;
        lhC = 201;
        llC = 200;
        rlC = 188;
        vC = 186;
        hC = 205;
        lC = 185;
        rC = 204;
        break;
    }
    gotoxy(x1, y1);
    printf("%c", lhC); //Printing the upper most left corner
    uint8_t i;
    if (style < 2)
    {
        printf("%c",lC); //Printing the left bar for text
        for (i = 0; i < spaces1; i++)
            printf(" ");
        for (i = 0; i < strlen(text) && i + x1 + 2 < x2 - 1 ; i++)
            printf("%c",text[i]);
        if (i < strlen(text))
        {
            if (x2-4 > x1 + 1)
            {
                gotoxy(x2-4,y1);
                printf(".");
            }
            if (x2-3 > x1 + 1)
            {
                gotoxy(x2-3,y1);
                printf(".");
            }
            if(x2-2 > x1 + 1)
            {
                gotoxy(x2-2,y1);
                printf(".");
            }

        }
        else
        {
            for(i = 0; i < spaces2; i++)
                printf(" ");
        }
        gotoxy(x2-1,y1);
        printf("%c",rC);
    } else
    {
        gotoxy(x1+1,y1);
        for (i = x1 + 1; i < x2; i++)
            printf("%c",hC);
    }


    printf("%c",rhC);
    for (i = y1+1; i < y2; i++) //Print the vertical lines
    {
        gotoxy(x1,i);
        printf("%c",vC);
        gotoxy(x2,i);
        printf("%c",vC);
    }
    gotoxy(x1,y2);
    printf("%c",llC); //Printing the lower left corner
    for (i = x1 + 1; i < x2; i++)
        printf("%c",hC); //Printing the lover line
    printf("%c",rlC); //Printing the lower right corner
}
