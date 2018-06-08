#include "minigame.h"

void intminigame(struct minigame_t *s)
{
    s->shipLine=1;
    s->oldShipLine=0;

}
void drawSpaceship (struct minigame_t *s)
{
 uint8_t i;

 for(i=0;i<16;i++)
        lcdBuffer[i+(s->oldShipLine*128)]=0x00;



    for(i=0;i<16;i++)
        lcdBuffer[i+(s->shipLine*128)]=customcharacter_data[0][i];
    lcd_push_buffer(lcdBuffer);

    s->oldShipLine=s->shipLine;
}

void moveShip (struct minigame_t *s, int8_t deltaLine)
{
    s->shipLine+=deltaLine;

}


void temp (){
 uint8_t currentJoyStick = readJoyStick();
        uint8_t oldJoyStick = currentJoyStick;

        if (   (currentJoyStick & 0x02) == 0x02 && (oldJoyStick & 0x02) == 0x00) //When clicking the down button
        {

        }
        else if (  (currentJoyStick & 0x01) == 0x01 && (oldJoyStick & 0x01) == 0x00)  //When clicking the up button
        {

        }
}
