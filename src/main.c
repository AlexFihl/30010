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

extern struct timer_t mainTimer;
extern uint8_t updateLCD;
extern const char customcharacter_data[2][16];
extern uint8_t lcdBuffer[512];

extern uint8_t updateGame;
extern uint8_t gameSpeed;

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
    //Making a player
    uint8_t i;
    struct player_t player;
    intPlayer(&player);
    char name[] = "Alex\0";
    setPlayerName(&player, name);
    uint8_t gameEnd;
    gameEnd = aGame1(&player);
    clrsrc();
    gotoxy(1,1);
    if (gameEnd == 0)
    {
        struct wall_t w;
        struct vector_t v1;
        struct vector_t v2;
        intVector(&v1, 100, 30);
        intVector(&v2, 120, 35);
        intWall(&w, &v1, &v2);
        window(&w, "You Died", 1);
        gotoxy(101,31);
        printf("Name: ");
        for (i = 0; i < strlen(player.name); i++)
            printf("%c", player.name[i]);
        gotoxy(101,32);
        printf("Final Score: %06lu", player.score);
    }
    else printf("You Won the level");
}

void mads()
{
    playMinigame1();
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
    setGameSpeed(5);
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

