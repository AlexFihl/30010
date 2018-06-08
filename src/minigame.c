#include "minigame.h"


void drawSpaceship (uint8_t line)
{


 uint8_t i;
    for(i=0;i<16;i++)
        lcdBuffer[i+(line*128)]=customcharacter_data[0][i];
    lcd_push_buffer(lcdBuffer);
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
