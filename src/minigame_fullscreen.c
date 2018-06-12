#include "minigame_fullscreen.h"

const char customcharacter_data[3][128] = { //08/06


{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xF8, 0xF8, 0xF8, 0x0E, 0x06, 0x06, 0x06, 0x46, 0x46, 0xC6, 0xCE, 0xC8, 0xC8, 0xC0, 0x00, 0x00, 0x00, 0xC6, 0xC6, 0xE6, 0x7E, 0x7E, 0x7E, 0xFE, 0xF0, 0xF0, 0xE0, 0x80, 0x00, 0x00, 0x00, 0x06, 0x06, 0xFE, 0xFE, 0xFE, 0xFE, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC6, 0xC6, 0xE&, 0x7E, 0x7E, 0x7E, 0xFE, 0xF0, 0xF0, 0xE0, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xF8, 0xF8, 0xF8, 0x0E, 0x0E, 0x07, 0x07, 0x07, 0x07, 0x1E, 0x1E, 0x1D, 0x0E, 0x0E, 0x0E, 0x06, 0x06, 0x06, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0x06, 0x06, 0x0E, 0x0E, 0x0E, 0x00, 0x00, 0x06, 0xFE, 0xFE, 0xFE, 0xFE, 0x06, 0x06, 0x00, 0x00, 0x00, 0xF8, 0xF8, 0xF8, 0xF8, 0xE0, 0x06, 0x06, 0x06, 0x06, 0x1E, 0x1E, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0xE0, 0xE0, 0xE1, 0xE3, 0xE3, 0x33, 0x13, 0x12, 0x12, 0x72, 0x72, 0x73, 0xE1, 0xE1, 0xE1, 0xE0, 0x32, 0x32, 0x13, 0x13, 0x13, 0x12, 0x30, 0xE0, 0xE0, 0xE0, 0xC3, 0xC3, 0x13, 0x13, 0x33, 0x33, 0xF2, 0xF2, 0xF2, 0xF3, 0xE3, 0xE3, 0xC3, 0x82, 0x12, 0x12, 0xF2, 0xF3, 0xF3, 0x11, 0xE0, 0xE2, 0xE2, 0xE3, 0x33, 0x33, 0x12, 0x10, 0x10, 0x10, 0x70, 0xE3, 0xE3, 0xE3, 0xC3, 0xC3, 0x13, 0xF2, 0xF0, 0xF0, 0xF0, 0x11, 0x11, 0x01, 0x03, 0x03, 0x12, 0xF2, 0xF2, 0xF2, 0xF3, 0x11, 0x11, 0x00, 0x10, 0xF0, 0xF0, 0xF2, 0xF3, 0xF3, 0x93, 0x93, 0x93, 0xF2, 0xF0, 0x00, 0x00, 0xE0, 0xE0, 0xE0, 0xF2, 0x93, 0x93, 0x93, 0xB3, 0x32, 0x32, 0x00, 0x30, 0x30, 0x30, 0x11, 0x11, 0x11, 0xF3, 0xF2, 0xF2, 0xF2, 0x12, 0x13, 0x13, 0x31, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x07, 0x07, 0x07, 0x1C, 0x18, 0x18, 0x18, 0x18, 0x1C, 0x1C, 0x04, 0x07, 0x1F, 0x1F, 0x1F, 0x1C, 0x1C, 0x18, 0x18, 0x18, 0x18, 0x1E, 0x07, 0x07, 0x07, 0x03, 0x03, 0x00, 0x00, 0x18, 0x18, 0x1F, 0x1F, 0x1F, 0x18, 0x01, 0x01, 0x01, 0x03, 0x07, 0x07, 0x1F, 0x1F, 0x1F, 0x00, 0x07, 0x1F, 0x1F, 0x1F, 0x1C, 0x1C, 0x18, 0x18, 0x18, 0x38, 0x7C, 0x7F, 0x7F, 0xE7, 0xE3, 0xE3, 0xC0, 0xC7, 0x9F, 0x9F, 0x9F, 0x98, 0x98, 0x18, 0x18, 0x18, 0x18, 0x1F, 0x1F, 0x1F, 0x18, 0x18, 0x00, 0x18, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x18, 0x18, 0x18, 0x1D, 0x1D, 0x04, 0x04, 0x02, 0x1E, 0x1E, 0x1C, 0x19, 0x19, 0x19, 0x1F, 0x07, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x1F, 0x1F, 0x1F, 0x1F, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}


};

