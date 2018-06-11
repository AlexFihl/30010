#include "game.h"

const uint8_t heart[] = { //09/06
    0x1C, 0x3E, 0x7E, 0xFC, 0xFC, 0x7E, 0x3E, 0x1C
};



uint8_t aGame1(struct player_t *p) //09/06
{
    //Making the wall
    struct wall_t wall;
    struct vector_t v1, v2, v3, v4;
    uint16_t i, j, x, y; //used for blocks
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
    uint8_t oldLife = p->life + 1;
    uint16_t numberOfBlocksLeft;
    while(1){
        if (updateGame > 0){
            if (p->life == 0)
                return 0;
            else if(p->life != oldLife)
            {
                resetBall(&b);
                resetStriker(&striker1);
                while(((readJoyStick() & 0x10) != 0x10))
                {
                    if (updateGame > 0){
                        moveBall(&b, updateStrikerPlacment(&striker1) << FIX14_SHIFT, 0);
                        drawStriker(&striker1);
                        drawBall(&b);
                        updateGame = 0;
                    }
                }
                oldLife = p->life;
            }
            //Drawing the blocks
            for (i = 0; i < x*y; i++)
                drawBlock(&blocks[i]);

            //Moving the striker
            updateStrikerPlacment(&striker1);
            drawStriker(&striker1);
            //Update the ball
            updatePosition(&b, &wall, &blocks, numberOfBlocks, p, &striker1);
            drawBall(&b);

            //Check have many blocks there are
            numberOfBlocksLeft = 0;
            for (i = 0; i < numberOfBlocks; i++)
                if(blocks[i].state > 0)
                    numberOfBlocksLeft++;
            if (numberOfBlocksLeft == 0)
                return 1;

            //Printing out to the display
            bufferReset();
            char str1[17], str2[14], str3[7];
            sprintf(str1, "Blocks left: %03d", numberOfBlocksLeft); //16 long
            lcd_write_string(str1, 0, 0);
            sprintf(str2, "Score: %06lu", p->score);
            lcd_write_string(str2, 0, 1);
            sprintf(str3, "Life: ");
            lcd_write_string(str3, 0, 2);
            for(i = 0; i < p->life; i++)
                for (j = 0; j < 8; j++)
                    putInBuffer(heart[j], 6*5 + j + i * 9, 2);
            lcd_update();

            updateGame = 0;
        }
    }
}

void setGameSpeed(int8_t gameSpeedIn) {gameSpeed = gameSpeedIn;} //09/06
