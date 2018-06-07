#include "ansi.h"

#define ESC 0x1B

void fgcolor(uint8_t foreground)
{
    /*  Value      foreground     Value     foreground
        ------------------------------------------------
          0        Black            8       Dark Gray
          1        Red              9       Light Red
          2        Green           10       Light Green
          3        Brown           11       Yellow
          4        Blue            12       Light Blue
          5        Purple          13       Light Purple
          6        Cyan            14       Light Cyan
          7        Light Gray      15       White
    */
    uint8_t type = 22;             // normal text
    if (foreground > 7)
    {
        type = 1;                // bold text
        foreground -= 8;
    }
    printf("%c[%d;%dm", ESC, type, foreground+30);
}

void bgcolor(uint8_t background)
{
    /* IMPORTANT:   When you first use this function you cannot get back to true white background in HyperTerminal.
       Why is that? Because ANSI does not support true white background (ANSI white is gray to most human eyes).
                    The designers of HyperTerminal, however, preferred black text on white background, which is why
                    the colors are initially like that, but when the background color is first changed there is no
     	              way comming back.
       Hint:        Use resetbgcolor(); clrscr(); to force HyperTerminal into gray text on black background.

        Value      Color
        ------------------
          0        Black
          1        Red
          2        Green
          3        Brown
          4        Blue
          5        Purple
          6        Cyan
          7        Gray
    */
    printf("%c[%dm", ESC, background+40);
}

void color(uint8_t foreground, uint8_t background)
{
// combination of fgcolor() and bgcolor() - uses less bandwidth
    uint8_t type = 22;             // normal text
    if (foreground > 7)
    {
        type = 1;                // bold text
        foreground -= 8;
    }
    printf("%c[%d;%d;%dm", ESC, type, foreground+30, background+40);
}

void resetbgcolor()
{
// gray on black text, no underline, no blink, no reverse
    printf("%c[m", ESC);
}

void showCursor()
{
    printf("%c[?25l", ESC);
}

void clrsrc()
{
    printf("%c[2J", ESC);
}

void clreol()
{
    printf("%c[K", ESC);
}

void gotoxy(uint8_t x, uint8_t y)
{
    printf("%c[%d;%dH", ESC, y, x);
}

void underline(uint8_t on)
{
    printf("%c[%dm", ESC,  (on == 1) ? 04 : 24);
}

void blink(uint8_t on)
{
    printf("%c[%dm", ESC,  (on == 1) ? 05 : 25);
}

void inverse(uint8_t on)
{
    printf("%c[%dm", ESC,  (on == 1) ? 07 : 27);
}

void window(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, char text[], uint8_t style)
{
    windowBG(x1, y1, x2, y2, text, style, 15, 0, 15, 0);
}

void windowFG(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, char text[], uint8_t style, uint8_t colorText, uint8_t colorBond)
{
    windowBG(x1, y1, x2, y2, text, style, colorText, 0, colorBond, 0);
}

void windowBG(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, char text[], uint8_t style, uint8_t colorText, uint8_t colorTextbg, uint8_t colorBond, uint8_t colorBondbg)
{
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
        hC = 179; // Horizontal line
        vC = 196; // Vertical line
        lC = 180; // Right hand bar of text
        rC = 195; // Left hand bar of text
        break;
    case 2:
    default: //Double lines
        rhC = 187;
        lhC = 201;
        llC = 200;
        rlC = 188;
        hC = 186;
        vC = 205;
        lC = 185;
        rC = 204;
        break;
    }
    gotoxy(x1, y1);
    color(colorBond,colorBondbg);
    printf("%c", lhC); //Printing the upper most left corner
    uint8_t i;
    if (style < 2)
    {
        printf("%c",lC); //Printing the left bar for text
        color(colorText, colorTextbg);
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
        color(colorBond,colorBondbg);
        gotoxy(x2-1,y1);
        printf("%c",rC);
    } else
    {
        gotoxy(x1+1,y1);
        for (i = x1 + 1; i < x2; i++)
            printf("%c",vC);
    }


    printf("%c",rhC);
    for (i = y1+1; i < y2; i++) //Print the vertical lines
    {
        gotoxy(x1,i);
        printf("%c",hC);
        gotoxy(x2,i);
        printf("%c",hC);
    }
    gotoxy(x1,y2);
    printf("%c",llC); //Printing the lower left corner
    for (i = x1 + 1; i < x2; i++)
        printf("%c",vC); //Printing the lover line
    printf("%c",rlC); //Printing the lower right corner
    color(15, 0);
}

//This function will read from the console and get the input from it.
char * getInput()
{
    char * line = malloc(32 * sizeof(char));
    uint8_t x;
    line[0] = uart_getc();
    x = 1;
    while (line[x - 1] != 0x0D)
    {
        line[x] = uart_getc();
        x++;
    }
    line[x-1] = '\0';
    return line;
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
