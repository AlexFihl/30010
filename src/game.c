#include "game.h"

const uint8_t heart[] =   //09/06
{
    0x1C, 0x3E, 0x7E, 0xFC, 0xFC, 0x7E, 0x3E, 0x1C
};

static void deathScreen(struct player_t *p)
{
    uint8_t i;
    struct wall_t w;
    struct vector_t v1;
    struct vector_t v2;
    intVector(&v1, 100, 30);
    intVector(&v2, 120, 35);
    intWall(&w, &v1, &v2);
    window(&w, "You Died", 0);
    gotoxy(101,31);
    printf("Name: ");
    for (i = 0; i < strlen(p->name); i++)
        printf("%c", p->name[i]);
    gotoxy(101,32);
    printf("Final Score: %06lu", p->score);
    while(((readJoyStick() & 0x10) != 0x10)) {}
    clrsrc();
}

static void printLCDGame(uint16_t numberOfBlocksLeft, struct player_t *p)
{
    //Printing out to the display
    uint8_t i, j;
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
}

void fullGame(struct player_t *p)
{
    setGameSpeed(4);
    uint8_t gameEnd = 1, gameCount = 0;
    while (gameEnd != 2 && gameEnd > 0 && gameCount < 10)
    {
        clrsrc();
        gameEnd = aGame1(p, gameCount);
        printLCDGame(0, p);
        gameCount++;
    }
    clrsrc();
    gotoxy(1,1);
    if (gameEnd == 0)
    {
        deathScreen(p);
    }
}

static int8_t getDeltaX(struct striker_t *s, struct wall_t *w)
{
    int8_t deltaX = updateStrikerPlacment(s);
    int32_t hl = FIX14_DIV((s->length >> 14), 2);
    if(deltaX + s->center.x + hl >= w->v2.x)
        deltaX = 0;
    if(s->center.x + deltaX - hl < (w->v1.x + 0x00004000))
        deltaX = 0;
    return deltaX;
}

uint8_t aGame1(struct player_t *p, uint8_t gameCount) //09/06
{
    //Setting the ball speed
    setBallSpeedFactor(0x00003000); //0x00003000 = 0.750
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
    //There will be rows of 4 in hight per level.
    uint8_t yEnd = 4 * (gameCount + 1) + 10;
    intVector(&v4, 210, yEnd);
    x = 18;
    //1 row per level
    y = gameCount + 1;
    uint8_t lifeOnBlocks = gameCount / 2 + 1;
    uint16_t numberOfBlocks = intMultipleBlocks(&blocks, v3, v4, x, y, lifeOnBlocks);
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
    int8_t deltaX;
    while(1)
    {
        if (updateGame > 0)
        {
            if (readADC2() >= 3000)
                return 2;
            if (readADC1() >= 3000)
                return 1;
            if (p->life == 0)
                return 0;
            else if(p->life != oldLife)
            {
                printLCDGame(numberOfBlocksLeft, p);
                resetBall(&b);
                resetStriker(&striker1);
                while(((readJoyStick() & 0x10) != 0x10))
                {
                    if (updateGame > 0)
                    {
                        deltaX = getDeltaX(&striker1, &wall);
                        moveBall(&b, deltaX << FIX14_SHIFT, 0);
                        updateStriker(&striker1, deltaX);
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
            deltaX = getDeltaX(&striker1, &wall);
            updateStriker(&striker1, deltaX);
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
            printLCDGame(numberOfBlocksLeft, p);

            updateGame = 0;
        }
    }
}
