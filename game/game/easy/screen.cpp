#include "../screen.h"
#include "../common.h"
#include "../pad.h"

void screen_init(void)
{
}

void plot_pixel(short x, short y, short col)
{
	//unsigned int white = col ? 0xffffff : 0;			//bgr
	unsigned int white = (0 - (unsigned int)col) & 0xffffff;

	/*if (x <= 0 || y <= 0)
		return;

	if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT)
		return;*/

	__asm__ __volatile__
		(
			//pen colour
			"move.l %0, %%d1\n\t"
			"move.b #80, %%d0\n\t"
			"trap #15\n\t"

			//plot pixel
			"move.w %1, %%d1\n\t"				//ensure that the argument type size matches the move size here for x/y
			"move.w %2, %%d2\n\t"
			"move.b #82, %%d0\n\t"
			"trap #15\n\t"

			: /*outputs*/
	: "g" (white), "g" (x), "g" (y) /*inputs*/
		: "d0", "d1", "d2" /*clobbers*/
		);
}

void plot_packed_pixel(short x, short y, int col)
{
	for (int count = 0; count < 8; count++)
		plot_pixel(x + count, y, (col >> (7 - count)) & 1);
}

void paint_screen(void)
{
}

void clear_screen(int col)
{
	for (int y = 0; y < SCREEN_HEIGHT; y++)
		for (int x = 0; x < SCREEN_WIDTH; x++)
			plot_pixel(x, y, col);
}

unsigned char read_pad(unsigned int id)		//todo 2nd pad and B/C buttons
{
	unsigned char ret = 0;
	unsigned long k;
	__asm__ __volatile__
		(
			"clr %%d0\n\t"
			"move.b #19, %%d0\n\t"
			"move.l #('A'<<24)+('D'<<16)+('W'<<8)+'S', %%d1\n\t"
			"trap #15\n\t"

			"move.l %%d1, %0\n\t"

		: "=r" (k) /*outputs*/
		: /*inputs*/
		: "d0", "d1"
		);

	if (k & 0xff000000UL)
		ret |= PAD_LEFT_SH;
	if (k & 0x00ff0000UL)
		ret |= PAD_RIGHT_SH;
	if (k & 0x0000ff00UL)
		ret |= PAD_UP_SH;
	if (k & 0x000000ffUL)
		ret |= PAD_DOWN_SH;

	return ret;
}
