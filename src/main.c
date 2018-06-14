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
#include "player.h"
#include "minigame.h"
#include "game.h"
#include "fix14.h"

#define maksMainMenu 4

//For flash memory
#define startAddress 0x0800F800

static void subSettingsMenu(struct player_t *p, int32_t * startBallSpeed, struct wall_t *w, int8_t * deltaStrikerStart, int8_t * deltaGamingSpeed)
{
    uint8_t menuPoint = 0, oldMenuPoint = 1, returnFromSubMenu = 0, backS = 0;
    clrsrc();
    window(w, "Settings", 0);
    uint8_t currentJoyStick, oldJoystick = readJoyStick();
    while(1)
    {
        currentJoyStick = readJoyStick();
        if (currentJoyStick != oldJoystick)
        {
            if      ((currentJoyStick & 0x01) == 0x01) //When clicking the up button
                menuPoint--;
            else if ((currentJoyStick & 0x02) == 0x02) //When clicking the down button
                menuPoint++;
            if(menuPoint < 0) menuPoint = 4;
            else if(menuPoint > 4) menuPoint = 0;
        }
        if(returnFromSubMenu == 1)
        {
            clrsrc();
            window(w, "Settings", 0);
            gotoxy(104, 23);
            printf("Player name");
            gotoxy(104, 25);
            printf("Striker lenght");
            gotoxy(104, 27);
            printf("Game speed");
            gotoxy(104, 29);
            printf("Ball speed");
            gotoxy(104, 31);
            printf("Back");
            returnFromSubMenu = 0;
        }
        if(menuPoint != oldMenuPoint)
        {
            gotoxy(104, 23);
            printf("Player name");
            gotoxy(104, 25);
            printf("Striker lenght");
            gotoxy(104, 27);
            printf("Game speed");
            gotoxy(104, 29);
            printf("Ball speed");
            gotoxy(104, 31);
            printf("Back");
            inverse(1);
        }
        switch(menuPoint)
        {
        case 0:
            if(menuPoint != oldMenuPoint)
            {
                gotoxy(104, 23);
                printf("Player name");
                inverse(0);
            }
            if((currentJoyStick & 0x10) == 0x10 && (oldJoystick & 0x10) == 0x00)
            {
                clrsrc();
                window(w, "Name", 0);
                char * name;
                gotoxy(101, 23);
                printf("Player name:");
                gotoxy(101, 24);
                printf("Max 10 charters!");
                gotoxy(101, 25);
                name = getInput();
                setPlayerName(p, name);
                returnFromSubMenu = 1;
            }
            break;
        case 1:
            if(menuPoint != oldMenuPoint)
            {
                gotoxy(104, 25);
                printf("Striker lenght");
                inverse(0);
            }
            if((currentJoyStick & 0x10) == 0x10 && (oldJoystick & 0x10) == 0x00)
            {
                oldJoystick = currentJoyStick;
                clrsrc();
                window(w, "Striker lenght", 0);
                int8_t oldS = 1;
                while(1)
                {
                    if(oldS != (*deltaStrikerStart))
                    {
                        gotoxy(101, 23);
                        printf("Striker lenght start: %02d", (*deltaStrikerStart) + 20);
                        oldS = (*deltaStrikerStart);
                    }
                    currentJoyStick = readJoyStick();
                    if      ((currentJoyStick & 0x04) == 0x04 && (oldJoystick & 0x04) == 0x00) //When clicking the left button
                        (*deltaStrikerStart) -= 2;
                    else if ((currentJoyStick & 0x08) == 0x08 && (oldJoystick & 0x08) == 0x00) //When clicking the right button
                        (*deltaStrikerStart) += 2;
                    if((currentJoyStick & 0x10) == 0x10 && (oldJoystick & 0x10) == 0x00)
                        break;
                    if((*deltaStrikerStart) > 10)
                        (*deltaStrikerStart) = 10;
                    else if((*deltaStrikerStart) < -10)
                        (*deltaStrikerStart) = -10;
                    oldJoystick = currentJoyStick;

                }
                returnFromSubMenu = 1;
            }
            break;
        case 2:
            if(menuPoint != oldMenuPoint)
            {
                gotoxy(104, 27);
                printf("Game speed");
                inverse(0);
            }
            if((currentJoyStick & 0x10) == 0x10 && (oldJoystick & 0x10) == 0x00)
            {
                oldJoystick = currentJoyStick;
                clrsrc();
                window(w, "Game speed", 0);
                int8_t oldG = 1;
                while(1)
                {
                    if(oldG != *deltaGamingSpeed)
                    {
                        gotoxy(101, 23);
                        switch(*deltaGamingSpeed)
                        {
                        case -3:
                            printf("Gaming speed: Fastest ");
                            break;
                        case -2:
                            printf("Gaming offset: Faster ");
                            break;
                        case -1:
                            printf("Gaming offset: Fast   ");
                            break;
                        case 0:
                            printf("Gaming offset: Normal ");
                            break;
                        case 1:
                            printf("Gaming offset: Slow   ");
                            break;
                        case 2:
                            printf("Gaming offset: Slower ");
                            break;
                        case 3:
                            printf("Gaming offset: Slowest");
                            break;
                        }
                        oldG = *deltaGamingSpeed;
                    }
                    currentJoyStick = readJoyStick();
                    if      ((currentJoyStick & 0x01) == 0x01 && (oldJoystick & 0x01) == 0x00) //When clicking the up button
                        (*deltaGamingSpeed)++;
                    else if ((currentJoyStick & 0x02) == 0x02 && (oldJoystick & 0x02) == 0x00) //When clicking the down button
                        (*deltaGamingSpeed)--;
                    if((currentJoyStick & 0x10) == 0x10 && (oldJoystick & 0x10) == 0x00)
                        break;
                    if((*deltaGamingSpeed) > 3)
                        (*deltaGamingSpeed) = 3;
                    else if((*deltaGamingSpeed) < -3)
                        (*deltaGamingSpeed) = -3;
                    oldJoystick = currentJoyStick;
                }
                returnFromSubMenu = 1;
            }
            break;
        case 3:
            if(menuPoint != oldMenuPoint)
            {
                gotoxy(104, 29);
                printf("Ball speed");
                inverse(0);
            }
            if((currentJoyStick & 0x10) == 0x10 && (oldJoystick & 0x10) == 0x00)
            {
                oldJoystick = currentJoyStick;
                clrsrc();
                window(w, "Ball speed", 0);
                int32_t oldB = 1;
                while(1)
                {
                    if(oldB != *startBallSpeed)
                    {
                        gotoxy(101, 23);
                        switch(*startBallSpeed)
                        {
                        case -3:
                            printf("Ball speed: Fastest   ");
                            break;
                        case -2:
                            printf("Ball offset: Faster   ");
                            break;
                        case -1:
                            printf("Ball offset: Fast     ");
                            break;
                        case 0:
                            printf("Ball offset: Normal   ");
                            break;
                        case 1:
                            printf("Gaming offset: Slow   ");
                            break;
                        case 2:
                            printf("Gaming offset: Slower ");
                            break;
                        case 3:
                            printf("Gaming offset: Slowest");
                            break;
                        }
                        oldB = *startBallSpeed;
                    }
                    currentJoyStick = readJoyStick();
                    if      ((currentJoyStick & 0x01) == 0x01 && (oldJoystick & 0x01) == 0x00) //When clicking the up button
                        (*startBallSpeed)++;
                    else if ((currentJoyStick & 0x02) == 0x02 && (oldJoystick & 0x02) == 0x00) //When clicking the down button
                        (*startBallSpeed)--;
                    if((currentJoyStick & 0x10) == 0x10 && (oldJoystick & 0x10) == 0x00)
                        break;
                    if((*startBallSpeed) > 3)
                        (*startBallSpeed) = 3;
                    else if((*startBallSpeed) < -3)
                        (*startBallSpeed) = -3;
                    oldJoystick = currentJoyStick;
                }
                startBallSpeed = (startBallSpeed * 8) << 12;
                returnFromSubMenu = 1;
            }
            break;
        case 4:
            if(menuPoint != oldMenuPoint)
            {
                gotoxy(104, 31);
                printf("Back");
                inverse(0);
            }
            if((currentJoyStick & 0x10) == 0x10 && (oldJoystick & 0x10) == 0x00)
            {
                backS = 1;
            }
            break;
        default:
            menuPoint = 0;
            break;
        }
        oldMenuPoint = menuPoint;
        oldJoystick = currentJoyStick;
        if(backS == 1)
            break;
    }
}

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
    for (i=0; i<5; i++)
    {
        gotoxy(104, 23+i*2);
        for(j=0; j<10; j++)
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

static void saveHighScore(struct player_t *p)
{
    char names[5][10];
    uint32_t point[5];
    uint32_t i, j, h;
    for(i=0; i<5; i++)
    {
        for(j=0; j<10; j++)
            names[i][j] = *(uint16_t *)(startAddress + j*2 + i*24);
        point[i] = *(uint16_t *)(startAddress + 20 + i*24) << 16;
        point[i] |= *(uint16_t *)(startAddress + 22 + i*24);
    }

    if(point[4] > p->score)
        return;
    if(point[0] > p->score)
    {

        for(i=4; i > 0; i--)
            if(point[i] < p->score && point[i-1] > p->score)
                break;
    }
    else
        i = 0;
    for(j=4; j > i; j--)
    {
        for(h=0; h<10; h++)
            names[j][h] = names[j-1][h];
        point[j] = point[j-1];
    }
    point[i] = p->score;
    for(j=0; j<10; j++)
        names[i][j] = (p->name)[j];

    //Saving to the flash agian
    FLASH_Unlock(); // Unlock FLASH for modification
    FLASH_ClearFlag( FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR );
    FLASH_ErasePage( startAddress ); // Erase entire page before writing
    for(j=0; j<5; j++)
    {
        for(i=0; i<10; i++)
        {
            FLASH_ProgramHalfWord(startAddress + i*2 + j * 24, names[j][i]);
        }
        FLASH_ProgramHalfWord(startAddress + 20 + j*24, point[j] >> 16); //For getting the top 4 byte of point
        FLASH_ProgramHalfWord(startAddress + 20 + 2 + j*24, point[j]);
    }
    FLASH_Lock();
}

static void printHelp()
{
    gotoxy(83, 23);
    printf("For controlling the striker in the game you can use the ");
    gotoxy(83,24);
    printf("joystick to move left and right. For quiting the game ");
    gotoxy(83,25);
    printf("turn the right knop up. Under settings can you choose ");
    gotoxy(83, 26);
    printf("the difficulty you want to start from. You can choose ");
    gotoxy(83, 27);
    printf("the starting size of the striker or the starting speed ");
    gotoxy(83, 28);
    printf("of the ball. You can also choose if you want less good ");
    gotoxy(83, 29);
    printf("power ups.");
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
    int8_t deltaStrikerStart = 0, deltaGamingSpeed = 0;
    int32_t startBallSpeed = 0;
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
    char * name = "Player\0";
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
            if(menuPoint < 0) menuPoint = maksMainMenu;
            else if(menuPoint > maksMainMenu) menuPoint = 0;
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
                fullGame(&player, startBallSpeed, deltaStrikerStart, deltaGamingSpeed);
                //Set player name should be implemtentet
                saveHighScore(&player);
                resetPlayer(&player);
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
                subSettingsMenu(&player, &startBallSpeed, &w, &deltaStrikerStart, &deltaGamingSpeed);
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
                oldJoystick = currentJoyStick;
                clrsrc();
                window(&w, "High Scores", 0);
                printHighScore();
                while(1)
                {
                    currentJoyStick = readJoyStick();
                    if((currentJoyStick & 0x10) == 0x10 && (oldJoystick & 0x10) == 0x00)
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
            if((currentJoyStick & 0x10) == 0x10 && (oldJoystick & 0x10) == 0x00)
            {
                oldJoystick = currentJoyStick;
                clrsrc();
                window(&w, "Help", 0);
                printHelp();
                while(1)
                {
                    currentJoyStick = readJoyStick();
                    if((currentJoyStick & 0x10) == 0x10 && (oldJoystick & 0x10) == 0x00)
                        break;
                    oldJoystick = currentJoyStick;
                }
                returnFromSubMenu = 1;
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
                uint32_t score;
                score = playMinigame1();
                window(&w, "Score", 0);
                gotoxy(102, 28);
                printf("Finale score:  %04lu", score);
                while((readJoyStick() & 0x10) == 0x00){}
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
    /*
    uint8_t i, j;
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
    FLASH_Lock();*/

    struct player_t p;
    intPlayer(&p);
    p.score = 501;
    char * name3 = "Alex\0";
    setPlayerName(&p, name3);
    saveHighScore(&p);
    printHighScore();

}

static void resetHighScore()
{
    uint8_t i, j;
    char name[5][10] = {"Best\0", "Better\0", "Good\0", "Bad\0", "Worst\0"};
    uint32_t point[5] = {0x000003E8,0x000001F4,0x000000FA,0x00000064,0x00000020};
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
    FLASH_ProgramHalfWord(startAddress + 142, 0x0001);
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
    //FLASH_ProgramHalfWord(startAddress + 142, 0x0001);
    if(*(uint16_t *)(startAddress + 144) == 0x0000)
        resetHighScore();
    startUpABC();
    //PuTTy need to be in 220 times 65.
    init_usb_uart(115200); // Initialize USB serial at 9600 baud
    //resetingPutty
    resetbgcolor();
    clrsrc();
    showCursor();
    joyStickSetUp();
    ledSetup();
    setupLCD();
    //Starting the timer for all things
    setUpTimer15();

    //Starting the timer for the buzzer
    setUpTimer2();
    setUpSpeaker();
    //The actual game
    //alex();
    menu();
    //mads();

    setLed(0,0,1);
    while(1)
    {

    }
}
