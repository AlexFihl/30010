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

#include "fix14.h"

extern struct timer_t mainTimer;
extern uint8_t updateLCD;

void dag3_5_1()
{
    uint8_t x, oldx;
    x = readJoyStick();
    oldx = x;
    while(1)
    {
        if(x != oldx)
        {
            gotoxy(1,1);
            switch(x)
            {
            case 0x01:
                printf("Up");
                break;
            case 0x02:
                printf("Down");
                break;
            case 0x04:
                printf("Left");
                break;
            case 0x08:
                printf("Right");
                break;
            case 0x10:
                printf("Center");
                break;
            case 0x05:
                printf("Up/Left");
                break;
            case 0x09:
                printf("Up/Right");
                break;
            case 0x06:
                printf("Down/Left");
                break;
            case 0x0a:
                printf("Down/Right");
                break;
            case 0x15:
                printf("Up/Left/Center");
                break;
            case 0x19:
                printf("Up/Right/Center");
                break;
            case 0x16:
                printf("Down/Left/Center");
                break;
            case 0x1a:
                printf("Down/Right/Center");
                break;
            case 0x11:
                printf("Up/Center");
                break;
            case 0x18:
                printf("Right/Center");
                break;
            case 0x14:
                printf("Left/Center");
                break;
            case 0x12:
                printf("Down/Center");
                break;
            default:
                printf("                     ");
                break;
            }
            oldx = x;
        }
        x = readJoyStick();

    }
}

void testDag2()
{
    char text[] = "";
    clrsrc();
    uint8_t x1 = 1, y1 = 1, x2 = 40, y2 = 15;
    window(x1,y1,x2,y2,text,3);
    gotoxy(2,2);
    struct ball_t ball;
    intBall(&ball,10,10,15,50);
    window(x1,y2+1,x1+10,y2+3,"Hits:",0); //Hit window
    gotoxy(x1+2,y2+2);
    printf("%d", ball.hitCount);
    uint16_t oldCount = 0;
    uint32_t i = 0;
    while(1)
    {
        if (i == 100000)
        {
            drawBall(&ball);
            updatePosition(&ball,x1,y1,x2,y2);
            if (oldCount != ball.hitCount)
            {
                gotoxy(x1+2,y2+2);
                printf("%d", ball.hitCount);
                oldCount = ball.hitCount;
            }
            i=0;
        }
        i++;
    }
}

void dag3_5_2()
{
    uint8_t x, oldx;
    x = readJoyStick();
    oldx = x;
    while(1)
    {
        if(x != oldx)
        {
            gotoxy(1,1);
            switch(x)
            {
            case 0x01:
                printf("Up                ");
                setLed(0,0,1);
                break;
            case 0x02:
                printf("Down              ");
                setLed(0,1,0);
                break;
            case 0x04:
                printf("Left              ");
                setLed(0,1,1);
                break;
            case 0x08:
                printf("Right             ");
                setLed(1,0,0);
                break;
            case 0x10:
                printf("Center            ");
                setLed(1,0,1);
                break;
            case 0x05:
                printf("Up/Left           ");
                setLed(1,1,1);
                break;
            case 0x09:
                printf("Up/Right          ");
                setLed(1,1,1);
                break;
            case 0x06:
                printf("Down/Left         ");
                setLed(1,1,1);
                break;
            case 0x0a:
                printf("Down/Right        ");
                setLed(1,1,1);
                break;
            case 0x15:
                printf("Up/Left/Center    ");
                setLed(1,1,1);
                break;
            case 0x19:
                printf("Up/Right/Center   ");
                setLed(1,1,1);
                break;
            case 0x16:
                printf("Down/Left/Center  ");
                setLed(1,1,1);
                break;
            case 0x1a:
                printf("Down/Right/Center ");
                setLed(1,1,1);
                break;
            case 0x11:
                printf("Up/Center         ");
                setLed(1,1,1);
                break;
            case 0x18:
                printf("Right/Center      ");
                setLed(1,1,1);
                break;
            case 0x14:
                printf("Left/Center       ");
                setLed(1,1,1);
                break;
            case 0x12:
                printf("Down/Center       ");
                setLed(1,1,1);
                break;
            default:
                printf("                  ");
                setLed(0,0,0);
                break;
            }
            oldx = x;
        }
        x = readJoyStick();

    }
}

void dag3_6_1_StopWatch()
{
    window(1,1,33,6,"Stop watch", 1);
    gotoxy(2,3);
    printf("Time since start:");
    gotoxy(2,4);
    printf("Split time 1:");
    gotoxy(2,5);
    printf("Split time 2:");
    gotoxy(21,3);
    printf("--:--:--.--");
    gotoxy(21,4);
    printf("--:--:--.--");
    gotoxy(21,5);
    printf("--:--:--.--");

    uint16_t oldSec = 0;
    uint8_t currentJoyStick = readJoyStick();
    uint8_t oldJoyStick = currentJoyStick;
    while(1)
    {
        if (oldSec != mainTimer.seconds)
        {
            gotoxy(21,3);
            drawAWatch(mainTimer);
            oldSec = mainTimer.seconds;
        }
        currentJoyStick = readJoyStick();
        if (        (currentJoyStick & 0x10) == 0x10 && (oldJoyStick & 0x10) == 0x00) //When clicking the center button
            TIM2->CR1 ^= 0x0001;
        else if (   (currentJoyStick & 0x04) == 0x04 && (oldJoyStick & 0x04) == 0x00) //When clicking the left button
        {
            gotoxy(21,4);
            drawAWatch(mainTimer);
        }
        else if (   (currentJoyStick & 0x08) == 0x08 && (oldJoyStick & 0x08) == 0x00) //When clicking the right button
        {
            gotoxy(21,5);
            drawAWatch(mainTimer);
        }
        else if (   (currentJoyStick & 0x02) == 0x02 && (oldJoyStick & 0x02) == 0x00) //When clicking the down button
        {
            mainTimer.hours = 0;
            mainTimer.minuts = 0;
            mainTimer.seconds = 0;
            mainTimer.hseconds = 0;
        }
        oldJoyStick = currentJoyStick;
    }
}

uint8_t decision(char * line)
{
    if(strcmp(line, "stop") == 0) return 0;
    else if(strcmp(line, "start") == 0) return 1;
    else if(strcmp(line, "split1") == 0) return 2;
    else if(strcmp(line, "split2") == 0) return 3;
    else if(strcmp(line, "reset") == 0) return 4;
    else return 5; //Help
}

void dag3_6_2_StopWatch()
{
    window(1,1,33,6,"Stop watch", 1);
    gotoxy(2,3);
    printf("Time since start:");
    gotoxy(2,4);
    printf("Split time 1:");
    gotoxy(2,5);
    printf("Split time 2:");
    gotoxy(21,3);
    printf("--:--:--.--");
    gotoxy(21,4);
    printf("--:--:--.--");
    gotoxy(21,5);
    printf("--:--:--.--");

    uint16_t oldSec = 0;
    uint8_t x;
    char * line;
    while (1)
    {
        if (oldSec != mainTimer.seconds) //Updating the clock on screen
        {
            gotoxy(21,3);
            drawAWatch(mainTimer);
            oldSec = mainTimer.seconds;
        }
        gotoxy(1,7);
        printf("Input:          ");
        gotoxy(8,7);
        line = getInput();
        x = decision(line);



        if (x == 1) //When clicking the center button
            startTimer2();
        else if (x == 0)
            stopTimer2();
        else if (x == 2) //When clicking the left button
        {
            gotoxy(21,4);
            drawAWatch(mainTimer);
        }
        else if (x == 3) //When clicking the right button
        {
            gotoxy(21,5);
            drawAWatch(mainTimer);
        }
        else if (x == 4) //When clicking the down button
        {
            resetTimer(&mainTimer);
        }
    }
}

void alex()
{

    struct wall_t wall;
    struct vector_t v1, v2, v3, v4;
    uint16_t i, x, y;
    intVector(&v1, 3, 1);
    intVector(&v2, 218, 63);
    intWall(&wall, &v1, &v2);
    drawWall(&wall);
    struct block_t* blocks = malloc(100 * sizeof *blocks);
    intVector(&v3, 5, 5);
    intVector(&v4, 40, 40);
    x = 4;
    y = 12;
    intMultipleBlocks(&blocks, v3, v4, x, y);
    for (i = 0; i < x * y; i++)
        drawBlock(&blocks[i]);
    struct striker_t striker1;
    intStriker(&striker1);
    drawStriker(&striker1);
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

    setLed(1,1,1);
    while(1)
    {

    }
}

