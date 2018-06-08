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

#include "fix14.h"

extern struct timer_t mainTimer;
extern uint8_t updateLCD;
extern const char customcharacter_data[2][16];
extern uint8_t lcdBuffer[512];

uint8_t updateGame;
uint8_t gameSpeed;



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
    struct ball_t b;
    intBall(&b, 110, 62, 1, 5);
    drawBall(&b);
    while(1)
    {
        if (updateGame > 0)
        {
            updatePosition(&b, &wall, &blocks, x * y);
            drawBall(&b);
            updateGame = 0;
        }
    }
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
    gameSpeed = 10;
    setUpTimer2();
    startTimer2();
    setupLCD();
    //alex();
    bufferReset();
    uint8_t i;
<<<<<<< HEAD
      for(i=0;i<16;i++)
           lcdBuffer[i]=customcharacter_data[0][i];
    lcd_push_buffer(lcdBuffer);
    setLed(0,0,1);
=======
    for(i=0;i<16;i++)
        lcdBuffer[i]=customcharacter_data[0][i];
    lcd_write_string("Hej", 0, 1);
    lcd_push_buffer(lcdBuffer);
    setLed(0,1,1);
>>>>>>> a5130c2cda39268e66d66f0ee6b0194b33cc8d71
    while(1)
    {

    }
}

