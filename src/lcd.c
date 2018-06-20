#include "lcd.h"

uint8_t lcdBuffer[512];

void setupLCD()
{
    bufferReset();
    init_spi_lcd();
    lcd_reset();
}

void lcd_write_string(char * s, uint16_t slice, uint16_t line)
{
    uint16_t i, j;
    uint16_t x;
    x = slice + (line << 0x07);
    for (j = 0; j < strlen(s); j++)
        for(i = 0; i < 5; i++)
            lcdBuffer[x + 6 * j + i] = character_data[s[j] - 0x20][i];
}

void lcd_update()
{
    if (updateLCD == 1)
    {
        lcd_push_buffer(lcdBuffer);
        updateLCD = 0;
    }
}

void bufferReset()
{
    memset(lcdBuffer, 0x00, 512);
}

void leftScrollingText(uint8_t lineNumber)
{
    uint16_t i;
    uint8_t temp[4];
    if ((lineNumber & 0x01) == 0x01)
        temp[0] = lcdBuffer[0];
    if ((lineNumber & 0x02) == 0x02)
        temp[1] = lcdBuffer[128];
    if ((lineNumber & 0x04) == 0x04)
        temp[2] = lcdBuffer[256];
    if ((lineNumber & 0x08) == 0x08)
        temp[3] = lcdBuffer[384];
    for (i = 0; i < 127; i++)
    {
        if ((lineNumber & 0x01) == 0x01)
            lcdBuffer[i] = lcdBuffer[i + 1];
        if ((lineNumber & 0x02) == 0x02)
            lcdBuffer[i + 128] = lcdBuffer[i + 129];
        if ((lineNumber & 0x04) == 0x04)
            lcdBuffer[i + 256] = lcdBuffer[i + 257];
        if ((lineNumber & 0x08) == 0x08)
            lcdBuffer[i + 384] = lcdBuffer[i + 385];
    }
    if ((lineNumber & 0x01) == 0x01)
        lcdBuffer[127] = temp[0];
    if ((lineNumber & 0x02) == 0x02)
        lcdBuffer[255] = temp[1];
    if ((lineNumber & 0x04) == 0x04)
        lcdBuffer[383] = temp[2];
    if ((lineNumber & 0x08) == 0x08)
        lcdBuffer[511] = temp[3];
}

void push_Buffer()
{
    lcd_push_buffer(lcdBuffer);
}

void putInBuffer(uint8_t byte, uint16_t slice, uint16_t line) //09/06
{
    lcdBuffer[slice + line*128] = byte;
}

uint8_t getBuffer(int8_t slice, int8_t line)
{
    return lcdBuffer[slice + line*128];
}
