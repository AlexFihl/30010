#include "minigame.h"

void intminigame(struct minigame_t *s) //08/06
{
    s->shipLine=1;
    s->oldShipLine=0;

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

void playMinigame1() //08/06
{
    uint8_t bufferTemp[512];
    memcpy(bufferTemp, lcdBuffer, 512 * sizeof(int));
    //bufferTemp = lcdBuffer;
    bufferReset();
    struct minigame_t minigame1;
    intminigame(&minigame1);
    drawSpaceship(&minigame1);
    uint8_t currentJoyStick = readJoyStick();
    uint8_t oldJoystick = readJoyStick();
    while (1){
            currentJoyStick = readJoyStick();
            if (currentJoyStick != oldJoystick){
                if      ((currentJoyStick & 0x02) == 0x02) //When clicking the up button
                    moveShip(&minigame1, 1);
                else if ((currentJoyStick & 0x01) == 0x01) //When clicking the down button
                    moveShip(&minigame1, -1);
                drawSpaceship(&minigame1);
                oldJoystick = currentJoyStick;
            }
    }




    bufferReset();
    memcpy(lcdBuffer, bufferTemp, 512 * sizeof(int));
}
