#include <stdio.h>
#include "vt100.h"

void vt100_move(uint8_t x, uint8_t y)
{
    printf("%c[%d;%dH", VT100_ESC, y, x);
}

void vt100_clear_screen(void)
{
    printf("%c[2J%c[?25l", VT100_ESC, VT100_ESC);
}
