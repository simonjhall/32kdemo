#include "../screen.h"
#include "../common.h"
#include "../pad.h"

void screen_init(void)
{
}

void plot_pixel(short x, short y, short col)
{
	unsigned int white = col ? 0xffffff : 0;			//bgr

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
	//I am having no joy with this
#if 0
	unsigned char ret;
	unsigned long k;
	__asm__ __volatile__
		(
			"move.b #19, %%d0\n\t"
			"move.l #('A' << 24), %%d1\n\t"
			"trap #15\n\t"

			"move.l %%d1, %0\n\t"

		: "=r" (k) /*outputs*/
		: /*inputs*/
		: "d0", "d1"
		);
/*
#define VK_ESCAPE         0x1B
#define VK_LEFT           0x25
#define VK_UP             0x26
#define VK_RIGHT          0x27
#define VK_DOWN           0x28

	switch (k)
	{
		case VK_LEFT:
			ret |= PAD_LEFT_SH;
			break;
		case VK_RIGHT:
			ret |= PAD_RIGHT_SH;
			break;
		case VK_UP:
			ret |= PAD_UP_SH;
			break;
		case VK_DOWN:
			ret |= PAD_DOWN_SH;
			break;
		default:
			break;
	}*/

	return ret;
#endif
}
