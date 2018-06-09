#include "game.h"

void aGame1(struct player_t *p)
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

    uint8_t currentJoyStick = readJoyStick();
    uint16_t numberOfBlocksLeft;
    while(1){
        if (updateGame > 0){
            //Drawing the blocks
            for (i = 0; i < x*y; i++)
                drawBlock(&blocks[i]);
            //Moving the striker
            currentJoyStick = readJoyStick();
            if      ((currentJoyStick & 0x04) == 0x04) //When clicking the left button
                updateStriker(&striker1, -2);
            else if ((currentJoyStick & 0x08) == 0x08) //When clicking the right button
                updateStriker(&striker1, 2);
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
                break;
            char str1[17], str2[12], str3[8];
            sprintf(str1, "Blocks left: %03d", numberOfBlocksLeft); //16 long
            lcd_write_string(str1, 0, 0);
            sprintf(str2, "Score: %04lu", p->score);
            lcd_write_string(str2, 0, 1);
            sprintf(str3, "Life: %d", p->life);
            lcd_write_string(str3, 0, 2);
            lcd_update();

            updateGame = 0;
        }
    }

    clrsrc();
    gotoxy(1,1);
    printf("game is gone, well gone");
}

void setGameSpeed(int8_t gameSpeedIn) {gameSpeed = gameSpeedIn;}
