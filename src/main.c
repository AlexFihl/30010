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

#include "fix14.h"

extern struct timer_t mainTimer;
extern uint8_t updateLCD;
extern const char customcharacter_data[2][16];
extern uint8_t lcdBuffer[512];

uint8_t updateGame;
uint8_t gameSpeed;

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
    while(1){
        if (updateGame > 0){
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
    //Making the wall
    struct wall_t wall;
    struct vector_t v1, v2, v3, v4;
    uint16_t i, x, y; //used for blocks
    intVector(&v1, 3, 1);
    intVector(&v2, 218, 63);
    intWall(&wall, &v1, &v2);
    drawWall(&wall);

    //Setting up the blocks
    struct block_t* blocks = malloc(100 * sizeof *blocks);
    intVector(&v3, 10, 10);
    intVector(&v4, 45, 45);
    x = 4;
    y = 12;
    uint16_t numberOfBlocks = x * y;
    intMultipleBlocks(&blocks, v3, v4, x, y);
    for (i = 0; i < numberOfBlocks; i++)
        drawBlock(&blocks[i]);

    //Setting up the striker
    struct striker_t striker1;
    intStriker(&striker1);
    drawStriker(&striker1);

    //Setting up the wall
    struct ball_t b;
    intBall(&b, 110, 60, -5, -5);
    drawBall(&b);

    //Making a player
    struct player_t player;
    intPlayer(&player);
    //Printing score
    gotoxy(1,64);
    printf("Hits: %03lu", player.score);
    uint8_t currentJoyStick = readJoyStick();
    uint16_t numberOfBlocksLeft;
    while(1){
        if (updateGame > 0){
            //Drawing the blocks
            for (i = 0; i < x*y; i++)
                drawBlock(&blocks[i]);
            //Moving the striker
            currentJoyStick = readJoyStick();
            if      (   (currentJoyStick & 0x04) == 0x04) //When clicking the left button
                updateStriker(&striker1, -2);
            else if (   (currentJoyStick & 0x08) == 0x08) //When clicking the right button
                updateStriker(&striker1, 2);
            drawStriker(&striker1);
            //Update the ball
            updatePosition(&b, &wall, &blocks, x * y, &player, &striker1);
            drawBall(&b);

            //Printing the ball
            gotoxy(7,64);
            printf("%03lu", player.score);
            updateGame = 0;

            //Check have many blocks there are
            numberOfBlocksLeft = 0;
            for (i = 0; i < numberOfBlocks; i++)
                if(blocks[i].state > 0)
                    numberOfBlocksLeft++;
            if (numberOfBlocksLeft == 0)
                break;
            gotoxy(12, 63);
            printf("Number of blocks left: %02d", numberOfBlocksLeft);

        }
    }

    clrsrc();
    gotoxy(1,1);
    printf("game is gone, well gone");
}

void mads()
{

    bufferReset();
    struct minigame_t minigame1;
    intminigame(&minigame1);
    drawSpaceship(&minigame1);
    uint8_t currentJoyStick = readJoyStick();
    uint8_t oldJoystick = readJoyStick();
    while (1){
            currentJoyStick = readJoyStick();
            if (currentJoyStick != oldJoystick){
                if      (   (currentJoyStick & 0x02) == 0x02) //When clicking the up button
                    moveShip(&minigame1, 1);
                else if (   (currentJoyStick & 0x01) == 0x01) //When clicking the down button
                    moveShip(&minigame1, -1);
                drawSpaceship(&minigame1);
                oldJoystick = currentJoyStick;
            }
    }
}

int main(void){
    startUpABC();
    //PuTTy need to be in 220 times 65.
    init_usb_uart(115200); // Initialize USB serial at 9600 baud
    resetbgcolor();
    clrsrc();
    showCursor();
    joyStickSetUp();
    ledSetup();
    gameSpeed = 5;
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

