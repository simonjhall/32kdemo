#include "../screen.h"
#include "../common.h"

void screen_init(void)
{
}

void plot_packed_pixel(short x, short y, int c)
{
	/* update me for big daddy v1.0 */
	/* this is the breadboard version */
	volatile unsigned short *pFb = (volatile unsigned short *)(512 * 1024);
	unsigned char b = c >> (x & 7);
	pFb[(y * (256 / 8) + x / 8)] |= ((b << 8) | b);
}

void plot_pixel(short x, short y, short c)
{
	//fill me in
}

void paint_screen(void)
{
}

void clear_screen(int col)
{
	/* needs updating for big daddy v1.0, this is the breadboard version */
	volatile unsigned short *pFb = (volatile unsigned short *)(512 * 1024);
	for (unsigned int count = 0; count < 8 * 1024; count++)
		pFb[count] = 0;
}
