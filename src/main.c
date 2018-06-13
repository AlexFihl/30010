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

#define maksMainMenu 4

//For flash memory
#define startAddress 0x0800F800

static void printHighScore()
{
    gotoxy(101, 23);
    printf("1: ");
    gotoxy(101, 25);
    printf("2: ");
    gotoxy(101, 27);
    printf("3: ");
    gotoxy(101, 29);
    printf("4: ");
    gotoxy(101, 31);
    printf("5: ");
    uint8_t i,j;
    for (i=0; i<5;i++)
    {
        gotoxy(104, 23+i*2);
        for(j=0;j<10;j++)
        {
            if(*(uint16_t *)(startAddress + j*2 + i*24) == '\0')
                break;
            else
                printf("%c", *(uint16_t *)(startAddress + j*2 + i*24));
        }
        gotoxy(114, 23+i*2);
        uint32_t point;
        point = *(uint16_t *)(startAddress + 20 + i*24) << 16;
        point |= *(uint16_t *)(startAddress + 22 + i*24);
        printf("%04lu", point);
    }
}

static void printFullMainMenu()
{
    gotoxy(106, 23);
    printf("Play Game");
    gotoxy(106, 25);
    printf("Settings");
    gotoxy(105, 27);
    printf("High scores");
    gotoxy(108, 29);
    printf("Help");
    gotoxy(104, 31);
    printf("Play minigame");
}

static void menu()
{
    uint8_t menuPoint = 0, oldMenuPoint = 1, returnFromSubMenu = 1;
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
            else if(menuPoint > maksMainMenu) menuPoint = maksMainMenu;
        }
        if(returnFromSubMenu == 1)
        {
            clrsrc();
            window(&w, "Menu", 0);
            printFullMainMenu();
            returnFromSubMenu = 0;
        }
        if(menuPoint != oldMenuPoint)
        {
            printFullMainMenu();
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
                //fullGame(&player);
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
            if((currentJoyStick & 0x10) == 0x10 && (oldJoystick & 0x10) == 0x00)
            {
                clrsrc();
                window(&w, "High Scores: ", 0);
                printHighScore();
                while((readJoyStick() & 0x10) == 0x10)
                while(1)
                {
                    currentJoyStick = readJoyStick();
                    if((readJoyStick()& 0x10) == 0x10 && (oldJoystick & 0x10) == 0x00)
                        break;
                    oldJoystick = currentJoyStick;
                }
                oldJoystick = currentJoyStick;
                returnFromSubMenu = 1;
            }
            break;
        case 3:
            gotoxy(108, 29);
            if(menuPoint != oldMenuPoint)
            {
                printf("Help");
                inverse(0);
            }
            break;
        case 4:
            gotoxy(104, 31);
            if(menuPoint != oldMenuPoint)
            {
                printf("Play minigame");
                inverse(0);
            }
            if((currentJoyStick & 0x10) == 0x10 && (oldJoystick & 0x10) == 0x00)
            {
                clrsrc();
                oldJoystick = currentJoyStick;
                playMinigame1();
                returnFromSubMenu = 1;
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
    uint8_t i, j;
    //char * name[5] = {"Alex\0", "Simon\0", "Mads\0", "Alex\0", "Mads\0"};
    char name[5][10] = {"Alex\0", "Simon\0", "Mads\0", "Alex\0", "Mads\0"};
    uint32_t point[5] = {0x0000039B,0x00000342,0x00000222,0x00000123,0x0000000B};
    FLASH_Unlock(); // Unlock FLASH for modification
    FLASH_ClearFlag( FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR );
    FLASH_ErasePage( startAddress ); // Erase entire page before writing
    for(j=0; j<5; j++)
    {
        gotoxy(1,1);
        for(i=0; i<10; i++)
        {
            printf("%c", name[j][i]);
            FLASH_ProgramHalfWord(startAddress + i*2 + j * 24, name[j][i]);
        }
        FLASH_ProgramHalfWord(startAddress + 20 + j*24, point[j] >> 16); //For getting the top 4 byte of point
        FLASH_ProgramHalfWord(startAddress + 20 + 2 + j*24, point[j]);
    }
    FLASH_Lock();
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
    //alex();
    menu();
    //mads();

    setLed(0,0,1);
    while(1)
    {

    }
}
