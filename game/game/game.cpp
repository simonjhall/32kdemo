// game.cpp : Defines the entry point for the console application.
//

#ifdef _WIN32
#include "stdafx.h"
#endif

#include "debugger/debugger.h"

#include "screen.h"
#include "pad.h"
#include "text.h"
#include "common.h"

#include "MiniGame.h"
#include "PacmanGame.h"
#include "ScreenDemo.h"

int c;
int main(void)
{
	install_debugger();

	font_init();

	screen_init();

	clear_screen(0);

	volatile long a = 0;
	volatile int b = 0;
	c = a / b;

	PacmanGame pacman;
	ScreenDemo screen;

	MiniGame *pGame = &pacman;

	unsigned long frame_count = 0;

	while (1)
	{
		unsigned char pad = read_pad(0);

		pGame->Update(pad, frame_count++);
		pGame->Render();
		
		paint_screen();
		
		pGame->Clear();
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
#ifdef __m68k__
	__asm__ __volatile__
	(
		"move.w #0, %%sr\n\t"
		//"suba.l #1024, %%sp\n\t"
		: /*outputs*/
		: /*inputs*/
		: /*clobbers*/
		);
#endif

	main();
	while (1);
}
#endif