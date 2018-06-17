#include "minigame.h"

void intminigame(struct minigame_t *s) //08/06
{
    s->shipLine=1;
    s->oldShipLine=0;
    s->timeSinceStart=0;
    s->life=3;
    s->shootIsAlive=0;
    s->shotsLeft=3;
    s->chanceOfSpawn=97;
}

void drawSpaceship (struct minigame_t *s) //08/06
{
    uint8_t i;

    for(i=0; i<16; i++)
        putInBuffer(0x00, i, (s->oldShipLine));
    for(i=0; i<16; i++)
        putInBuffer(customcharacter_data[0][i], i, (s->shipLine));
    push_Buffer();
    s->oldShipLine=s->shipLine;
}

void moveShip (struct minigame_t *s, int8_t deltaLine) //08/06
{
    if (s->shipLine+deltaLine>=0 && s->shipLine+deltaLine<=3)
        s->shipLine+=deltaLine;
}

static void minigameSrolling (struct minigame_t *s)
{
    int16_t i, j;
    for(j = 0; j < 4; j++)
        for (i = 0; i < 127; i++)
            putInBuffer(getBuffer(i+1, j), i, j);

    putInBuffer(s->obstacleBuffer[0], 127, 0);
    putInBuffer(s->obstacleBuffer[16], 127, 1);
    putInBuffer(s->obstacleBuffer[32], 127, 2);
    putInBuffer(s->obstacleBuffer[48], 127, 3);

    for (i = 0; i < 64; i++)
        if (i!=15 && i!=31 && i!=47 && i!=63)
            s->obstacleBuffer[i] = s->obstacleBuffer[i + 1];
    s->obstacleBuffer[15] = 0x00;
    s->obstacleBuffer[31] = 0x00;
    s->obstacleBuffer[47] = 0x00;
    s->obstacleBuffer[63] = 0x00;
}

static void startObstacle (struct obstacle_t *s,struct minigame_t *t, uint32_t timeSinceStart)
{
    uint8_t i;
    for(i=0; i<16; i++)
        t->obstacleBuffer[i+(s->obstacleLine*16)]=customcharacter_data[1+s->obstacleLine][i];
    s->timeStart=timeSinceStart;
    s->isAlive=1;
}

static void rndStartObstacle(struct obstacle_t *s,struct minigame_t *t, uint32_t timeSinceStart)
{
    if (s->isAlive==0)
    {
        uint16_t r;
        r = rand()%100;
        if (r>t->chanceOfSpawn)
            startObstacle(s,t,timeSinceStart);
    }
}

static void shoot(struct minigame_t *s,struct obstacle_t *t)
{
    if (s->shipLine==t->obstacleLine)
    {
        t->isAlive=0;
        s->shootIsAlive=5;
        s->shootLine=s->shipLine;
    }
}

static void drawShoot(struct minigame_t *s)
{
    uint8_t j;
    uint8_t i;
    if (s->shootIsAlive > 1)
    {
        for (i = 0; i <16 ; i++)
        {
            putInBuffer(customcharacter_data[5][i], i+16, s->shootLine);
        }
        for (j=0; j<6; j++)
        {
            for (i = 0; i <16 ; i++)
            {
                putInBuffer(customcharacter_data[6][i], i+32+j*16, s->shootLine);
            }
        }
    }
    else if (s->shootIsAlive==1)
    {
        for (j=0; j<7; j++)
        {
            for (i = 0; i <16 ; i++)
            {
                putInBuffer(0x00, i+16+(j*16), s->shootLine);
            }
        }
    }

    if (s->shootIsAlive>0)
        s->shootIsAlive--;

}

void initObstacle (struct obstacle_t *s,struct minigame_t *t,uint8_t obstacleline)
{
    s->obstacleLine=obstacleline;
    s->isAlive=0;
    memset(t->obstacleBuffer,0x00,64);
}

static void collision(struct minigame_t *s,struct obstacle_t *t)
{
    if ((s->timeSinceStart >= t->timeStart+113)&&(s->timeSinceStart <= t->timeStart+143)&&(s->shipLine==t->obstacleLine)&&(t->isAlive!=0))
    {
        s->life--;
        t->isAlive=0;
    }
    else if (s->timeSinceStart > t->timeStart+143)
        t->isAlive=0;
    if (s->life==3)
        setLed(0,0,1);
    else if (s->life==2)
        setLed(0,1,0);
    else if (s->life==1)
        setLed(1,0,0);
}

static void startScreen()
{
    uint16_t i;
    uint16_t j;
    bufferReset();
    for (i=0; i<3; i++)
        for (j=0; j<127; j++)
            putInBuffer(customfullscreen[i][j], j, i);
    char str1[22];
    sprintf(str1, "Press center to start"); //16 long
    lcd_write_string(str1, 2, 3);
    push_Buffer();


}

static void endScreen(struct minigame_t *s)
{
    uint16_t i;
    uint16_t j;
    bufferReset();
    for (i=0; i<3; i++)
        for (j=0; j<127; j++)
            putInBuffer(customfullscreen[i+3][j], j, i);
    char str1[7];
    sprintf(str1, "%04lu", s->timeSinceStart); //16 long
    lcd_write_string(str1, 55, 3);
    push_Buffer();
}

uint32_t playMinigame1() //08/06
{
    uint8_t currentJoyStick = readJoyStick();
    uint8_t oldJoystick = readJoyStick();

    startScreen();
    while (((currentJoyStick & 0x10) != 0x10)||(oldJoystick==currentJoyStick))//click center to continue
    {
        oldJoystick = currentJoyStick;
        currentJoyStick = readJoyStick();
    }
    bufferReset();
    struct minigame_t minigame1;
    struct obstacle_t obstacle1;
    struct obstacle_t obstacle2;
    struct obstacle_t obstacle3;
    struct obstacle_t obstacle4;
    intminigame(&minigame1);
    drawSpaceship(&minigame1);
    initObstacle(&obstacle1,&minigame1,0);
    initObstacle(&obstacle2,&minigame1,1);
    initObstacle(&obstacle3,&minigame1,2);
    initObstacle(&obstacle4,&minigame1,3);
    setMiniGameSpeed(8);
    oldJoystick = currentJoyStick;
    while (1)
    {
        if (updateMinigame > 0)
        {
            currentJoyStick = readJoyStick();
            if (currentJoyStick != oldJoystick)
            {
                if      ((currentJoyStick & 0x02) == 0x02) //When clicking the up button
                    moveShip(&minigame1, 1);
                else if ((currentJoyStick & 0x01) == 0x01) //When clicking the down button
                    moveShip(&minigame1, -1);
                else if (((currentJoyStick & 0x10) == 0x10)&&(minigame1.shotsLeft!=0)) //When clicking the center button
                {
                    shoot(&minigame1,&obstacle1);
                    shoot(&minigame1,&obstacle2);
                    shoot(&minigame1,&obstacle3);
                    shoot(&minigame1,&obstacle4);
                    minigame1.shotsLeft--;
                }
                oldJoystick = currentJoyStick;
            }
            minigameSrolling(&minigame1);
            drawShoot(&minigame1);
            drawSpaceship(&minigame1);
            collision(&minigame1,&obstacle1);
            collision(&minigame1,&obstacle2);
            collision(&minigame1,&obstacle3);
            collision(&minigame1,&obstacle4);
            rndStartObstacle(&obstacle1,&minigame1,minigame1.timeSinceStart);
            rndStartObstacle(&obstacle2,&minigame1,minigame1.timeSinceStart);
            rndStartObstacle(&obstacle3,&minigame1,minigame1.timeSinceStart);
            rndStartObstacle(&obstacle4,&minigame1,minigame1.timeSinceStart);
            if (minigame1.life==0)
            {
                endScreen(&minigame1);
                return minigame1.timeSinceStart;
            }
            updateMinigame = 0;
            minigame1.timeSinceStart++;
            if (minigame1.timeSinceStart==400)
                setMiniGameSpeed(7);
            else if (minigame1.timeSinceStart==600)
                minigame1.chanceOfSpawn = 94;
            else if (minigame1.timeSinceStart==800)
                setMiniGameSpeed(6);
            else if (minigame1.timeSinceStart==1000)
                minigame1.chanceOfSpawn = 90;
            else if (minigame1.timeSinceStart==1200)
                setMiniGameSpeed(5);
            else if (minigame1.timeSinceStart==1500)
                setMiniGameSpeed(4);
            else if (minigame1.timeSinceStart==1900)
                setMiniGameSpeed(3);
            else if (minigame1.timeSinceStart==2400)
                setMiniGameSpeed(2);
            else if (minigame1.timeSinceStart==3000)
                setMiniGameSpeed(1);
        }
    }
}
