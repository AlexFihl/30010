#include "game.h"



static void endGameScreen(struct player_t *p)
{
    uint8_t i;
    struct wall_t w;
    struct vector_t v1;
    struct vector_t v2;
    intVector(&v1, 90, 30);
    intVector(&v2, 130, 35);
    intWall(&w, &v1, &v2);
    window(&w, "You cleared the game", 0);
    gotoxy(101,31);
    printf("Name: ");
    for (i = 0; i < strlen(p->name); i++)
        printf("%c", p->name[i]);
    gotoxy(101,32);
    printf("Final Score: %06lu", p->score);
    while(((readJoyStick() & 0x10) == 0x10)) {}
    while(((readJoyStick() & 0x10) != 0x10)) {}
    clrsrc();
}

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
    while(((readJoyStick() & 0x10) == 0x10)) {}
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
    for(i = 0; i < p->catchKeys; i++)
        for (j = 0; j < 16; j++)
            putInBuffer(keys[j], j + i * 17, 3);

    lcd_update();
}

static void bossKey(struct block_t **a, struct striker_t *s, struct wall_t *w,uint16_t numberOfBlocks)
{
    if (readADC1()>2000)
    {
        uint8_t i;
        bufferReset();
        push_Buffer();
        clrsrc();
        while(readADC1()>2000)//while not clicking center
        {

        }
        drawWall(w);
        s->boss=1;
        for (i = 0; i < numberOfBlocks; i++)
            ((*a)[i]).boss=1;
        }
}

static uint8_t aGame1(struct player_t *p, uint8_t gameCount, int32_t startBallSpeed, int8_t deltaStrikerStart, int8_t deltaGamingSpeed, int8_t *scoreMultiplier)
{
    //Setting the ball speed
    setBallSpeedFactor(0x00002000 + (startBallSpeed<<9) + (gameCount << 8)); //0x00002000 = 0.500
    //It will rise 0x00000100 per game
    //Making the wall
    struct wall_t wall;
    struct vector_t v1, v2, v3, v4;
    struct powerUp_t power[5];
    uint8_t powerUpsInUse = 0, numberOfBalls = 0;
    //The power ups flags
    int8_t skipLevel = 0, ballOnStriker = 0, strikerShoting = 0, multiplyBalls = 0, teleportBallFlag = 0;
    uint16_t i, j, x, y; //used for blocks
    intVector(&v1, 3, 1);
    intVector(&v2, 218, 63);
    intWall(&wall, &v1, &v2);
    drawWall(&wall);

    //Setting up the blocks
    x = 18;
    //1 row per level
    y = gameCount + 1;
    struct block_t* blocks = malloc((x * y + 10) * sizeof *blocks);
    intVector(&v3, 10, 7);
    //There will be rows of 4 in hight per level.
    uint8_t yEnd = 4 * (gameCount + 1) + 7;
    intVector(&v4, 210, yEnd);
    uint8_t lifeOnBlocks = (gameCount >> 1) + 1;
    uint16_t numberOfBlocks = intMultipleBlocks(&blocks, v3, v4, x, y, lifeOnBlocks);
    for (i = 0; i < numberOfBlocks; i++)
        drawBlock(&blocks[i]);
    uint16_t numberOfBlocksLeft = numberOfBlocks;

    //Setting up the striker
    struct striker_t striker1;
    intStriker(&striker1, deltaStrikerStart);
    drawStriker(&striker1);

    //setting up the strikershooting powerup
    int8_t shootBalls = 0;
    struct strikerShooting_t strikerShooting1;
    initStrikerShooting(&strikerShooting1, &striker1);

    //Setting up the ball
    struct ball_t b;
    struct ball_t *balls = malloc(10 * sizeof *balls);
    intBall(&b, 110, 60);
    balls[0] = b;
    drawBall(&balls[0]);
    numberOfBalls = 1;
    uint8_t oldLife = p->life + 1;
    int8_t deltaX;
    while(1)
    {
        if (updateGame > 0)
        {
            setFreq(0);
            if (readADC2() >= 3000)
            {
                free(blocks);
                return 2;
            }
            if (p->life == 0) //Checking if the player is dead
            {
                free(blocks);
                return 0;
            }
            else if (p->life > oldLife)
                oldLife = p->life;
            else if(p->life != oldLife || ballOnStriker == 1)
            {
                ballOnStriker = 0;
                printLCDGame(numberOfBlocksLeft, p);
                for(i = 0; i < numberOfBalls; i++)
                {
                    intBall(&balls[i], 110, 60);
                    resetBall(&balls[i]);
                }
                resetStriker(&striker1);
                while(((readJoyStick() & 0x10) != 0x10))
                {
                    if (updateGame > 0)
                    {
                        deltaX = getDeltaX(&striker1, &wall);
                        for(i = 0; i < numberOfBalls; i++)
                        {
                            moveBall(&balls[i], deltaX << FIX14_SHIFT, 0);
                            drawBall(&balls[i]);
                        }
                        updateStriker(&striker1, deltaX);
                        drawStriker(&striker1);
                        updateGame = 0;
                    }
                }
                oldLife = p->life;
            }

            bossKey(&blocks,&striker1,&wall,numberOfBlocks);

            //Moving the striker
            deltaX = getDeltaX(&striker1, &wall);
            updateStriker(&striker1, deltaX);
            drawStriker(&striker1);

            //Update the balls
            for(i = 0; i < numberOfBalls; i++)
            {
                updatePosition(&balls[i], &wall, &blocks, numberOfBlocks, p, &striker1);
                drawBall(&balls[i]);
            }
            //Removing dead balls
            for(i = 0; i < numberOfBalls; i++)
                if(balls[i].dead == 1)
                {
                    for(j=i; j<numberOfBalls; j++)
                        balls[j] = balls[j+1];
                    balls[numberOfBalls-1] = balls[numberOfBalls];
                    numberOfBalls--;
                    if(numberOfBalls == 0)
                    {
                        lossLife(p);
                        numberOfBalls = 1;
                    }
                }
            //Spawning a power up
            for (i = 0; i < numberOfBlocks; i++)
                if((blocks[i]).hits >= (blocks[i]).life && (blocks[i]).oldState != 0 && powerUpsInUse < 5)
                {
                    uint32_t x1,y1,xTemp,yTemp;
                    xTemp = (blocks[i].v2.x - blocks[i].v1.x) >> FIX14_SHIFT;
                    x1 = (blocks[i].v1.x + FIX14_DIV(xTemp, 2)) >> FIX14_SHIFT;
                    yTemp = (blocks[i].v2.y - blocks[i].v1.y) >> FIX14_SHIFT;
                    y1 = (blocks[i].v1.y + FIX14_DIV(yTemp, 2)) >> FIX14_SHIFT;
                    struct vector_t vP;
                    intVector(&vP, x1, y1);
                    struct powerUp_t powerTemp;
                    initPowerUp(&powerTemp, &vP, cusRandom()%12); //Real thing
                    power[powerUpsInUse] = powerTemp;
                    powerUpsInUse++;
                }

            //Printing a power up
            for(i = 0; i < powerUpsInUse; i++)
            {
                updatePowerUp(&power[i], &striker1, &wall);
                applyPowerUp(&power[i], &striker1, &wall, p, &ballOnStriker, &skipLevel, &strikerShoting, &multiplyBalls, &teleportBallFlag);
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

            if(skipLevel == 1)
            {
                for (i = 0; i < numberOfBlocks; i++)
                {
                    blocks[i].hits = blocks[i].life;
                }
                skipLevel = 0;
            }

            if(teleportBallFlag == 1)
            {
                for(i = 0; i < numberOfBalls; i++)
                    teleportBall(&balls[i]);
                teleportBallFlag = 0;
            }

            if(strikerShoting == 1)
            {
                if (strikerShooting1.shootSpeed == 0 && shootBalls < 10)
                {
                    setFreq(500);
                    shotCaller(&strikerShooting1, &striker1, strikerShooting1.amountOfBalls);
                    strikerShooting1.shootSpeed=10;
                    shootBalls++;
                    strikerShooting1.amountOfBalls++;
                }
                strikerShooting1.shootSpeed--;
                if(shootBalls == 10)
                {
                    strikerShoting = 0;
                    shootBalls = 0;
                }
            }

            for(i=0; i<strikerShooting1.amountOfBalls; i++)
            {
                updateShot(&wall, &blocks, numberOfBlocks, p, &striker1, &strikerShooting1, i);
                drawShot(&strikerShooting1, i);
            }

            for(i=0; i < strikerShooting1.amountOfBalls; i++)
            {
                if(strikerShooting1.isAlive[i] == 0)
                {
                    for(j=i; j<strikerShooting1.amountOfBalls; j++)
                    {
                        strikerShooting1.position[j] = strikerShooting1.position[j+1];
                        strikerShooting1.oldPosition[j] = strikerShooting1.oldPosition[j+1];
                        strikerShooting1.isAlive[j] = strikerShooting1.isAlive[j+1];
                    }
                    strikerShooting1.position[strikerShooting1.amountOfBalls - 1] = strikerShooting1.position[strikerShooting1.amountOfBalls];
                    strikerShooting1.oldPosition[strikerShooting1.amountOfBalls - 1] = strikerShooting1.oldPosition[strikerShooting1.amountOfBalls];
                    strikerShooting1.isAlive[strikerShooting1.amountOfBalls - 1] = strikerShooting1.isAlive[strikerShooting1.amountOfBalls];
                    strikerShooting1.amountOfBalls--;
                }

            }

            if(multiplyBalls == 1 && numberOfBalls < 10)
            {
                struct ball_t bTemp;
                intBall(&bTemp, striker1.center.x >> FIX14_SHIFT, 60);
                balls[numberOfBalls] = bTemp;
                multiplyBalls = 0;
                numberOfBalls++;
            }
            //Updating, giving points, and drawing the blocks
            for (i = 0; i < numberOfBlocks; i++)
            {
                updateBlockState(&blocks[i]);
                if(blocks[i].oldState != 0 && blocks[i].state == 0)
                    (p->score) += 1 * (*scoreMultiplier);
                drawBlock(&blocks[i]);
            }

            //Check have many blocks there are
            numberOfBlocksLeft = 0;
            for (i = 0; i < numberOfBlocks; i++)
                if(blocks[i].state > 0)
                    numberOfBlocksLeft++;

            if (numberOfBlocksLeft == 0)
            {
                free(blocks);
                return 1;
            }
            printLCDGame(numberOfBlocksLeft, p);

            //Chehcing the power up flag for the minigame
            if(p->catchKeys == 3)
            {
                setFreq(0);
                uint32_t scoreFromMinigame = playMinigame1();
                p->score += scoreFromMinigame / 100;
                while((readJoyStick() & 0x10) == 0x10) {};
                while((readJoyStick() & 0x10) == 0x00) {};
                p->catchKeys = 0;
            }
            updateGame = 0;
        }
    }
}

void fullGame(struct player_t *p, int32_t startBallSpeed, int8_t deltaStrikerStart, int8_t deltaGamingSpeed, int8_t *scoreMultiplier)
{
    setGameSpeed(10 + deltaGamingSpeed);
    uint8_t gameEnd = 1, gameCount = 0;
    while (gameEnd != 2 && gameEnd > 0 && gameCount < 10)
    {
        clrsrc();
        gameEnd = aGame1(p, gameCount, startBallSpeed, deltaStrikerStart, deltaGamingSpeed, scoreMultiplier);
        printLCDGame(0, p);
        gameCount++;
    }
    clrsrc();
    if (gameEnd == 0)
        deathScreen(p);
    else
        endGameScreen(p);
}
