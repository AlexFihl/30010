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

static void menu()
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

void simon()
{

}

void alex()
{

}

void mads()
{
    int32_t x;
    x = playMinigame1();
    x++;
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
    //The actual game
    //menu();
    //alex();
    mads();

    setLed(0,0,1);
    while(1)
    {

    }
}
