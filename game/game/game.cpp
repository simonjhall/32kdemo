// game.cpp : Defines the entry point for the console application.
//

#ifdef _WIN32
#include "stdafx.h"
#endif

#include "screen.h"
#include "pad.h"
#include "common.h"


void draw_square(short x_pos, short y_pos, short width, short height, short col)
{
	for (short y = y_pos; y < y_pos + height; y++)
		for (short x = x_pos; x < x_pos + width; x++)
		{
			plot_pixel(x, y, col);
		}
}


int main(void)
{
	screen_init();

	clear_screen(0);

	short x = 128 - 25;
	short y = 128 - 25;

	while (1)
	{
		unsigned char pad = read_pad(0);
		
		if (pad & PAD_UP_SH)
			y--;
		if (pad & PAD_DOWN_SH)
			y++;
		if (pad & PAD_LEFT_SH)
			x--;
		if (pad & PAD_RIGHT_SH)
			x++;

		clear_screen(0);

		draw_square(x, y, 50, 50, 1);

		paint_screen();
	}

    return 0;
}

#if __DADDY__ == 1
//not present on any other platform

extern "C" void vblank(void) __attribute__((interrupt_handler));
extern "C" void vblank(void)
{
}
#endif

#if defined __m68k__

extern "C" void _start(void) __attribute__((noreturn));
extern "C" void _start(void)
{
	main();
	while (1);
}
#endif