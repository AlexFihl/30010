#include "minigame.h"

void intminigame(struct minigame_t *s) //08/06
{
    s->shipLine=1;
    s->oldShipLine=0;
    s->timeSinceStart=0;
}

void drawSpaceship (struct minigame_t *s) //08/06
{
    uint8_t i;

    for(i=0;i<16;i++)
        lcdBuffer[i+(s->oldShipLine*128)]=0x00;
    for(i=0;i<16;i++)
        lcdBuffer[i+(s->shipLine*128)]=customcharacter_data[0][i];
    lcd_push_buffer(lcdBuffer);

    s->oldShipLine=s->shipLine;
}

void moveShip (struct minigame_t *s, int8_t deltaLine) //08/06
{
    if (s->shipLine+deltaLine>=0 && s->shipLine+deltaLine<=3)
    s->shipLine+=deltaLine;
}
static void minigameSrolling (struct obstacle_t *s)
{
    int16_t i;
    for (i = 0; i < 511; i++)
    {
        if (i!=127 && i!=255 && i!=383)
            lcdBuffer[i] = lcdBuffer[i + 1];
    }

    lcdBuffer[127] = s->obstacleBuffer[0];
    lcdBuffer[255] = s->obstacleBuffer[16];
    lcdBuffer[383] = s->obstacleBuffer[32];
    lcdBuffer[511] = s->obstacleBuffer[48];

    for (i = 0; i < 64; i++)
    {
        if (i!=15 && i!=31 && i!=47 && i!=63)
            s->obstacleBuffer[i] = s->obstacleBuffer[i + 1];
    }
    s->obstacleBuffer[15] = 0x00;
    s->obstacleBuffer[31] = 0x00;
    s->obstacleBuffer[47] = 0x00;
    s->obstacleBuffer[63] = 0x00;
}
void initObstacle (struct obstacle_t *s,uint8_t obstacleline,uint32_t timeSinceStart)
{
    uint8_t i;
    memset(s->obstacleBuffer,0x00,64);
    for(i=0;i<16;i++)
        s->obstacleBuffer[i+(obstacleline*16)]=customcharacter_data[1+obstacleline][i];
    s->timeStart=timeSinceStart;
    s->obstacleLine=obstacleline;
}

static void collision(struct minigame_t *s,struct obstacle_t *t)
{
    if ((s->timeSinceStart >= t->timeStart+113)&&(s->timeSinceStart <= t->timeStart+143)&&(s->shipLine==t->obstacleLine))
        setLed(1,0,0);
    else if (s->timeSinceStart > t->timeStart+143)
        setLed(0,1,0);
}

void playMinigame1() //08/06
{
    bufferReset();
    struct minigame_t minigame1;
    struct obstacle_t obstacle1;
    intminigame(&minigame1);
    drawSpaceship(&minigame1);
    uint8_t currentJoyStick = readJoyStick();
    uint8_t oldJoystick = readJoyStick();
    initObstacle(&obstacle1,3,minigame1.timeSinceStart);
    minigameSpeed = 5;
    while (1){
        if (updateMinigame > 0){
            currentJoyStick = readJoyStick();
            if (currentJoyStick != oldJoystick){
                if      ((currentJoyStick & 0x02) == 0x02) //When clicking the up button
                    moveShip(&minigame1, 1);
                else if ((currentJoyStick & 0x01) == 0x01) //When clicking the down button
                    moveShip(&minigame1, -1);
                oldJoystick = currentJoyStick;
            }
        minigameSrolling(&obstacle1);
        drawSpaceship(&minigame1);
        collision(&minigame1,&obstacle1);
        updateMinigame = 0;
        minigame1.timeSinceStart++;
        }
    }
}
