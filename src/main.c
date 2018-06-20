#include "stm32f30x_conf.h"
#include "30010_io.h"
#include "ansi.h"
#include "trigomentri.h"
#include "io.h"
#include "time.h"
#include "lcd.h"
#include <stdio.h>
#include <stdlib.h>
#include "gameWall.h"
#include "player.h"
#include "minigame.h"
#include "game.h"
#include "fix14.h"
#include "powerUp.h"

#define maksMainMenu 4

//For flash memory
#define startAddress 0x0800F800


static void subSettingsMenu(struct player_t *p, int32_t * startBallSpeed, struct wall_t *w, int8_t * deltaStrikerStart, int8_t * deltaGamingSpeed, int8_t *scoreMultiplier)
{
    int8_t menuPoint = 0, oldMenuPoint = 1, returnFromSubMenu = 0, backS = 0;
    clrsrc();
    window(w, "Settings", 0);
    uint8_t currentJoyStick, oldJoystick = readJoyStick();
    while(1)
    {
        if(updateMenu == 1)
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
                    *scoreMultiplier -= *deltaStrikerStart/5;
                    while(1)
                    {
                        if(updateMenu == 1)
                        {
                            if(oldS != (*deltaStrikerStart))
                            {
                                gotoxy(101, 23);
                                printf("Striker start lenght: %02d", (*deltaStrikerStart) + 20);
                                oldS = (*deltaStrikerStart);
                            }
                            currentJoyStick = readJoyStick();
                            if      ((currentJoyStick & 0x01) == 0x01 && (oldJoystick & 0x01) == 0x00) //When clicking the up button
                                (*deltaStrikerStart) += 2;
                            else if ((currentJoyStick & 0x02) == 0x02 && (oldJoystick & 0x02) == 0x00) //When clicking the down button
                                (*deltaStrikerStart) -= 2;
                            if((currentJoyStick & 0x10) == 0x10 && (oldJoystick & 0x10) == 0x00)
                                break;
                            if((*deltaStrikerStart) > 10)
                                (*deltaStrikerStart) = 10;
                            else if((*deltaStrikerStart) < -10)
                                (*deltaStrikerStart) = -10;
                            oldJoystick = currentJoyStick;
                            updateMenu = 0;
                        }
                    }
                    *scoreMultiplier += *deltaStrikerStart/5;
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
                    *scoreMultiplier += *deltaGamingSpeed;
                    while(1)
                    {
                        if(updateMenu == 1)
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
                                    printf("Gaming speed: Faster ");
                                    break;
                                case -1:
                                    printf("Gaming speed: Fast   ");
                                    break;
                                case 0:
                                    printf("Gaming speed: Normal ");
                                    break;
                                case 1:
                                    printf("Gaming speed: Slow   ");
                                    break;
                                case 2:
                                    printf("Gaming speed: Slower ");
                                    break;
                                case 3:
                                    printf("Gaming speed: Slowest");
                                    break;
                                }
                                oldG = *deltaGamingSpeed;
                            }
                            currentJoyStick = readJoyStick();
                            if      ((currentJoyStick & 0x01) == 0x01 && (oldJoystick & 0x01) == 0x00) //When clicking the up button
                                (*deltaGamingSpeed)--;
                            else if ((currentJoyStick & 0x02) == 0x02 && (oldJoystick & 0x02) == 0x00) //When clicking the down button
                                (*deltaGamingSpeed)++;
                            if((currentJoyStick & 0x10) == 0x10 && (oldJoystick & 0x10) == 0x00)
                                break;
                            if((*deltaGamingSpeed) > 3)
                                (*deltaGamingSpeed) = 3;
                            else if((*deltaGamingSpeed) < -3)
                                (*deltaGamingSpeed) = -3;
                            oldJoystick = currentJoyStick;
                            updateMenu = 0;
                        }
                    }
                    *scoreMultiplier -= *deltaGamingSpeed;
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
                    *scoreMultiplier -= *startBallSpeed;
                    while(1)
                    {
                        if(updateMenu == 1)
                        {
                            if(oldB != *startBallSpeed)
                            {
                                gotoxy(101, 23);
                                switch(*startBallSpeed)
                                {
                                case 3:
                                    printf("Ball speed: Fastest   ");
                                    break;
                                case 2:
                                    printf("Ball speed: Faster   ");
                                    break;
                                case 1:
                                    printf("Ball speed: Fast     ");
                                    break;
                                case 0:
                                    printf("Ball speed: Normal   ");
                                    break;
                                case -1:
                                    printf("Ball speed: Slow   ");
                                    break;
                                case -2:
                                    printf("Ball speed: Slower ");
                                    break;
                                case -3:
                                    printf("Ball speed: Slowest");
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
                            updateMenu = 0;
                        }
                    }
                    *scoreMultiplier += *startBallSpeed;
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
            if(backS == 1) break;
            updateMenu = 0;
        }
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
        for(j=0; j<11; j++)
        {
            if(*(uint16_t *)(startAddress + j*2 + i*26) == '\0')
                break;
            else
                printf("%c", *(uint16_t *)(startAddress + j*2 + i*26));
        }
        gotoxy(114, 23+i*2);
        uint32_t point;
        point = *(uint16_t *)(startAddress + 22 + i*26) << 16;
        point |= *(uint16_t *)(startAddress + 24 + i*26);
        printf("%06lu", point);
    }
}

static void saveHighScore(struct player_t *p)
{
    char names[5][11];
    uint32_t point[5];
    uint32_t i, j, h;
    for(i=0; i<5; i++)
    {
        for(j=0; j<11; j++)
            if(*(uint16_t *)(startAddress + j*2 + i*26) == '\0')
                break;
            else
                names[i][j] = *(uint16_t *)(startAddress + j*2 + i*26);
        point[i] = *(uint16_t *)(startAddress + 22 + i*26) << 16;
        point[i] |= *(uint16_t *)(startAddress + 24 + i*26);
    }

    if(point[4] >= p->score)
        return;
    if(point[0] > p->score)
        for(i=4; i > 0; i--)
            if(point[i] < p->score && point[i-1] >= p->score)
                break;
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
        for(i=0; i<11; i++)
        {
            FLASH_ProgramHalfWord(startAddress + i*2 + j * 26, names[j][i]);
        }
        FLASH_ProgramHalfWord(startAddress + 22 + j*26, point[j] >> 16); //For getting the top 4 byte of point
        FLASH_ProgramHalfWord(startAddress + 24 + j*26, point[j]);
    }
    FLASH_Lock();
}

static void printHelp()
{
    gotoxy(83, 22);
    printf("MAIN GAME");
    gotoxy(83, 23);
    printf("The objective is to break all the blocks without letting");
    gotoxy(83, 24);
    printf("the ball escape through the bottom, and catch power-ups");
    gotoxy(83, 25);
    printf("to help you. The striker is controlled by the joystick to");
    gotoxy(83, 26);
    printf("move left and right. To quit the game turn the right knob");
    gotoxy(83, 27);
    printf("up. Under settings can you choose the difficulty you want");
    gotoxy(83, 28);
    printf("to start from, this also changes the amount of points");
    gotoxy(83, 29);
    printf("you get form each block. You can choose:");
    gotoxy(83, 30);
    printf("striker size, game speed and ball speed");
    gotoxy(83, 32);
    printf("MINIGAME");
    gotoxy(83, 33);
    printf("The spaceship is controlled by the joystick, to move up");
    gotoxy(83, 34);
    printf("and down, to avoid the obstacles. The middle button on");
    gotoxy(83, 35);
    printf("the joystick is used to shoot the LAZER, but beware you");
    gotoxy(83, 36);
    printf("only have enough energy to shoot 3 times! The shield is");
    gotoxy(83, 37);
    printf("indicated by the led, blue = strong shield, green =");
    gotoxy(83, 38);
    printf("damaged shield and red = no shield left.");
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
    int8_t menuPoint = 0, oldMenuPoint = 1, returnFromSubMenu = 1;
    int8_t scoreMultiplier = 10;
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
                fullGame(&player, startBallSpeed, deltaStrikerStart, deltaGamingSpeed, &scoreMultiplier);
                //Set player name should be implemtentet
                saveHighScore(&player);
                resetPlayer(&player);
                bufferReset();
                push_Buffer();
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
                subSettingsMenu(&player, &startBallSpeed, &w, &deltaStrikerStart, &deltaGamingSpeed, &scoreMultiplier);
                returnFromSubMenu = 1;
                if(scoreMultiplier <= 0) scoreMultiplier = 1;
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
                    if(updateMenu == 1)
                    {
                        currentJoyStick = readJoyStick();
                        if((currentJoyStick & 0x10) == 0x10 && (oldJoystick & 0x10) == 0x00)
                            break;
                        oldJoystick = currentJoyStick;
                        updateMenu = 0;
                    }
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
                    if(updateMenu == 1)
                    {
                        currentJoyStick = readJoyStick();
                        if((currentJoyStick & 0x10) == 0x10 && (oldJoystick & 0x10) == 0x00)
                            break;
                        oldJoystick = currentJoyStick;
                        updateMenu = 0;
                    }
                }
                oldJoystick = currentJoyStick;
                clrsrc();
                window(&w, "Power Ups", 0);
                printPowerupHelp();
                while(1)
                {
                    if(updateMenu == 1)
                    {
                        currentJoyStick = readJoyStick();
                        if((currentJoyStick & 0x10) == 0x10 && (oldJoystick & 0x10) == 0x00)
                            break;
                        oldJoystick = currentJoyStick;
                        updateMenu = 0;
                    }
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
                while((readJoyStick() & 0x10) == 0x00) {}
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

static void resetHighScore()
{
    uint8_t i, j;
    char name[5][11] = {"Best\0", "Better\0", "Good\0", "Bad\0", "Worst\0"};
    uint32_t point[5] = {0x00002710,0x00001388,0x000009C4,0x000003E8,0x000001F4};
    FLASH_Unlock(); // Unlock FLASH for modification
    FLASH_ClearFlag( FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR );
    FLASH_ErasePage( startAddress ); // Erase entire page before writing
    for(j=0; j<5; j++)
    {
        gotoxy(1,1);
        for(i=0; i<11; i++)
        {
            printf("%c", name[j][i]);
            FLASH_ProgramHalfWord(startAddress + i*2 + j * 26, name[j][i]);
        }
        FLASH_ProgramHalfWord(startAddress + 22 + j*26, point[j] >> 16); //For getting the top 4 byte of point
        FLASH_ProgramHalfWord(startAddress + 24 + j*26, point[j]);
    }
    FLASH_Lock();
}

int main(void)
{
    startUpABC();
    //PuTTy need to be in 220 times 65.
    init_usb_uart(460800); // Initialize USB serial at 9600 baud
    //resetingPutty
    resetbgcolor();
    clrsrc();
    removeCursor();
    joyStickSetUp();
    if((readJoyStick() & 0x02) == 0x02) //When pressed down
        resetHighScore(); //For getting a clean high score
    ledSetup();
    //Starting the timer for all things
    setUpTimer15();

    //Starting the timer for the buzzer
    setUpTimer2();
    setUpSpeaker();

    //Setting up the lcd
    setupLCD();
    bufferReset();
    push_Buffer();
    //The actual menu
    menu();

    while(1){}
}
