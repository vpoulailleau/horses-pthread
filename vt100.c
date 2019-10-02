#include <stdio.h>
#include "vt100.h"

void vt100_move(uint8_t x, uint8_t y)
{
	putchar(VT100_ESC);
	putchar('[');
	putchar('0' + (y / 100));
	putchar('0' + ((y / 10) % 10));
	putchar('0' + (y % 10));
	putchar(';');
	putchar('0' + (x / 100));
	putchar('0' + ((x / 10) % 10));
	putchar('0' + (x % 10));
	putchar('H');
}

void vt100_clear_screen(void)
{
	putchar(VT100_ESC);
	putchar('[');
	putchar('2');
	putchar('J');
	putchar(VT100_ESC);
	putchar('[');
	putchar('?');
	putchar('2');
	putchar('5');
	putchar('l');
}
