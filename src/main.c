#include "stm32f30x_conf.h"
#include "30010_io.h"
#include "ansi.h"
#include "trigomentri.h"
#include "ball.h"
#include "io.h"
#include "time.h"
#include "lcd.h"
#include "blocks.h"
#include <stdio.h>
#include <stdlib.h>
#include "gameWall.h"
#include "striker.h"
#include "customcharset.h"
#include "player.h"
#include "minigame.h"
#include "game.h"
#include "fix14.h"


void simon()
{
    struct wall_t wall;
    struct vector_t v1, v2, v3, v4;
    uint16_t i, x, y;
    intVector(&v1, 3, 1);
    intVector(&v2, 218, 63);
    intWall(&wall, &v1, &v2);
    drawWall(&wall);
    struct block_t* blocks = malloc(100 * sizeof *blocks);
    intVector(&v3, 10, 10);
    intVector(&v4, 45, 45);
    x = 4;
    y = 12;
    intMultipleBlocks(&blocks, v3, v4, x, y);
    for (i = 0; i < x * y; i++)
        drawBlock(&blocks[i]);
    struct striker_t striker1;
    intStriker(&striker1);
    drawStriker(&striker1);
    struct ball_t b;
    intBall(&b, 110, 62, -5, -5);
    drawBall(&b);
    uint8_t k = 1;
    struct player_t player;
    intPlayer(&player);
    while(1)
    {
        if (updateGame > 0)
        {
            updatePosition(&b, &wall, &blocks, x * y, &player, &striker1);
            drawBall(&b);
            switch (k)
            {
            case(1):
                changeStrikerLength(&striker1,-4);
                drawStriker(&striker1);
                break;
            case(2):
                updateStriker(&striker1,-10);
                drawStriker(&striker1);
                break;
            case(3):
                changeStrikerLength(&striker1,4);
                drawStriker(&striker1);
                break;
            case(4):
                updateStriker(&striker1,10);
                drawStriker(&striker1);
                break;
            case(5):
                k = 0;
                break;
            default:
                break;
            }
            k++;
            for (i = 0; i < x*y; i++)
                drawBlock(&blocks[i]);
            updateGame = 0;
            setLed(1,1,0);
        }
    }
}

void alex()
{
    uint8_t menuPoint = 0, oldMenuPoint = 1, returnFromSubMenu = 0;
    clrsrc();
    struct wall_t w;
    struct vector_t v1, v2;
    intVector(&v1, 80, 20);
    intVector(&v2, 140, 40);
    intWall(&w, &v1, &v2);
    window(&w, "Menu", 0);

    uint8_t currentJoyStick = readJoyStick();
    uint8_t oldJoystick = readJoyStick();

    //Making a player
    struct player_t player;
    intPlayer(&player);
    char * name = "Player 1\0";
    setPlayerName(&player, name);


    gotoxy(106,23);
    printf("Play Game");
    gotoxy(106, 25);
    printf("Settings");
    gotoxy(105, 27);
    printf("High scores");
    while(1)
    {
        currentJoyStick = readJoyStick();
        if (currentJoyStick != oldJoystick)
        {
            if      ((currentJoyStick & 0x01) == 0x01) //When clicking the up button
                menuPoint--;
            else if ((currentJoyStick & 0x02) == 0x02) //When clicking the down button
                menuPoint++;
            if(menuPoint < 0) menuPoint = 0;
            else if(menuPoint > 2) menuPoint = 2;
        }
        if(returnFromSubMenu == 1)
        {
            clrsrc();
            window(&w, "Menu", 0);
            returnFromSubMenu = 0;
            gotoxy(106,23);
            printf("Play Game");
            gotoxy(106, 25);
            printf("Settings");
            gotoxy(105, 27);
            printf("High scores");
        }
        if(menuPoint != oldMenuPoint)
        {
            gotoxy(106,23);
            printf("Play Game");
            gotoxy(106, 25);
            printf("Settings");
            gotoxy(105, 27);
            printf("High scores");
            inverse(1);
        }

        switch(menuPoint)
        {
        case 0:
            gotoxy(106,23);
            if(menuPoint != oldMenuPoint)
            {
                printf("Play Game");
                inverse(0);
            }
            if((currentJoyStick & 0x10) == 0x10 && (oldJoystick & 0x10) == 0x00)
            {
                setPlayerLife(&player, 3);
                fullGame(&player);
                returnFromSubMenu = 1;
            }
            break;
        case 1:
            gotoxy(106, 25); //Settings
            if(menuPoint != oldMenuPoint)
            {
                printf("Settings");
                inverse(0);
            }
            if((currentJoyStick & 0x10) == 0x10 && (oldJoystick & 0x10) == 0x00)
            {
                clrsrc();
                window(&w, "Settings", 0);
                gotoxy(101, 23);
                printf("Player name:");
                gotoxy(101, 24);
                printf("Max 10 charters!");
                gotoxy(101, 25);
                name = getInput();
                setPlayerName(&player, name);
                returnFromSubMenu = 1;
            }
            break;
        case 2:
            gotoxy(105, 27);
            if(menuPoint != oldMenuPoint)
            {
                printf("High scores");
                inverse(0);
            }
            break;
        default:
            menuPoint = 0;
            inverse(0);
            break;
        }

        oldMenuPoint = menuPoint;
        oldJoystick = currentJoyStick;
    }
}

void mads()
{
    playMinigame1();

}

int main(void)
{
    startUpABC();
    //PuTTy need to be in 220 times 65.
    init_usb_uart(115200); // Initialize USB serial at 9600 baud
    resetbgcolor();
    clrsrc();
    showCursor();
    joyStickSetUp();
    ledSetup();
    setUpTimer2();
    startTimer2();
    setupLCD();
    alex();
    //mads();

    setLed(0,0,1);
    while(1)
    {

    }
}
