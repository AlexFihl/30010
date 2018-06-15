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

static uint8_t aGame1(struct player_t *p, uint8_t gameCount, int32_t startBallSpeed, int8_t deltaStrikerStart, int8_t deltaGamingSpeed) //09/06
{
    //Setting the ball speed
    setBallSpeedFactor(0x00002000 + startBallSpeed + (gameCount << 8)); //0x00002000 = 0.500
    //It will rise 0x00000100 per game
    //Making the wall
    struct wall_t wall;
    struct vector_t v1, v2, v3, v4;
    struct powerUp_t power[5];
    uint8_t powerUpsInUse = 0;
    uint16_t i, j, x, y; //used for blocks
    intVector(&v1, 3, 1);
    intVector(&v2, 218, 63);
    intWall(&wall, &v1, &v2);
    drawWall(&wall);

    //Setting up the blocks
    struct block_t* blocks = malloc(100 * sizeof *blocks);
    intVector(&v3, 10, 7);
    //There will be rows of 4 in hight per level.
    uint8_t yEnd = 4 * (gameCount + 1) + 7;
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
    intStriker(&striker1, deltaStrikerStart);
    drawStriker(&striker1);

    //Setting up the wall
    struct ball_t b;
    intBall(&b, 110, 60, -5, -5);
    drawBall(&b);
    uint8_t oldLife = p->life + 1;
    uint16_t numberOfBlocksLeft;
    int8_t deltaX, ballOnStriker = 0;
    while(1)
    {
        if (updateGame > 0)
        {
            setFreq(0);
            if (readADC2() >= 3000)
                return 2;
            if (readADC1() >= 3000)
                return 1;
            if (p->life == 0)
                return 0;
            else if (p->life > oldLife)
                oldLife = p->life;
            else if(p->life != oldLife || ballOnStriker == 1)
            {
                ballOnStriker = 0;
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

            //Moving the striker
            deltaX = getDeltaX(&striker1, &wall);
            updateStriker(&striker1, deltaX);
            drawStriker(&striker1);
            //Update the ball
            updatePosition(&b, &wall, &blocks, numberOfBlocks, p, &striker1);
            drawBall(&b);

            //Spawning a power up
            for (i = 0; i < numberOfBlocks; i++)
            {
                if((blocks[i]).state == 0 && (blocks[i]).oldState >= 1 && powerUpsInUse < 5 && rand()%100 < 10) //Uncomment this 10% chance for an power up
                {
                    uint32_t x1,y1,xTemp,yTemp;
                    xTemp = (blocks[i].v2.x - blocks[i].v1.x) >> FIX14_SHIFT;
                    x1 = (blocks[i].v1.x + FIX14_DIV(xTemp, 2)) >> FIX14_SHIFT;
                    yTemp = (blocks[i].v2.y - blocks[i].v1.y) >> FIX14_SHIFT;
                    y1 = (blocks[i].v1.y + FIX14_DIV(yTemp, 2)) >> FIX14_SHIFT;
                    struct vector_t vP;
                    intVector(&vP, x1, y1);
                    struct powerUp_t powerTemp;
                    initPowerUp(&powerTemp, &vP, rand()%12); //Real thing
                    //initPowerUp(&powerTemp, &vP, 8); //Testing
                    power[powerUpsInUse] = powerTemp;
                    powerUpsInUse++;
                }
            }

            //Drawing the blocks
            for (i = 0; i < x*y; i++)
                drawBlock(&blocks[i]);

            //Printing a power up
            for(i = 0; i < powerUpsInUse; i++)
            {
                updatePowerUp(&power[i], &striker1, &wall);
                applyPowerUp(&power[i], &striker1, &wall, &b, p, &ballOnStriker);
                drawPowerUp(&power[i], blocks, yEnd, numberOfBlocks);
            }
            //removing a catched powerUp
            for(i=0; i < powerUpsInUse; i++)
            {
                if(power[i].catched == 1 || power[i].dead == 1)
                {
                    for(j=i; j<powerUpsInUse; j++)
                        power[j] = power[j+1];
                    power[powerUpsInUse-1] = power[powerUpsInUse];
                    powerUpsInUse--;
                }

            }
            //Check have many blocks there are
            numberOfBlocksLeft = 0;
            for (i = 0; i < numberOfBlocks; i++)
                if(blocks[i].state > 0)
                    numberOfBlocksLeft++;
            if (numberOfBlocksLeft == 0)
                return 1;
            printLCDGame(numberOfBlocksLeft, p);
            if(p->catchKeys == 3)
            {
                uint32_t scoreFromMinigame = playMinigame1();
                p->score += scoreFromMinigame / 100;
                while((readJoyStick() & 0x10) == 0x10) {};
                p->catchKeys = 0;
            }
            updateGame = 0;
        }
    }
}

void fullGame(struct player_t *p, int32_t startBallSpeed, int8_t deltaStrikerStart, int8_t deltaGamingSpeed)
{
    setGameSpeed(8 + deltaGamingSpeed);
    uint8_t gameEnd = 1, gameCount = 0;
    while (gameEnd != 2 && gameEnd > 0 && gameCount < 10)
    {
        clrsrc();
        gameEnd = aGame1(p, gameCount, startBallSpeed, deltaStrikerStart, deltaGamingSpeed);
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
