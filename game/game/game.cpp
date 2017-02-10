// game.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <assert.h>
#include <math.h>

HANDLE wHnd, rHnd;

static const short s_width = 256;
static const short s_height = 256;

static const short s_scaleFactorX = 2;
static const short s_scaleFactorY = 1;

static CHAR_INFO s_consoleBuffer[(s_width * s_scaleFactorX) * (s_height * s_scaleFactorY)];
static CHAR_INFO s_lastConsoleBuffer[(s_width * s_scaleFactorX) * (s_height * s_scaleFactorY)];

#if defined _WIN32
void plot_pixel(short x, short y, short col)
{
	x = x * s_scaleFactorX;
	y = y * s_scaleFactorY;

	if (x < 0 || y < 0)
		return;
	if (x >= s_width * s_scaleFactorX || y >= s_height * s_scaleFactorY)
		return;

	//http://benryves.com/tutorials/winconsole/4
	s_consoleBuffer[(s_width * s_scaleFactorX) * y + x].Attributes = col ? (BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY) : 0;
}
#elif defined __m68k__
void plot(long x, long y, short col)
{
	unsigned int white = 0xffffff;
	/*volatile unsigned short *pRam = (volatile unsigned short *)(513 * 1024);
	pRam[y * 256 + x] = 0xff;*/
	__asm__ __volatile__
	(
		//pen colour
		"move.l %0, %%d1\n\t"
		"move.b #80, %%d0\n\t"
		"trap #15\n\t"

		//plot pixel
		"move.l %1, %%d1\n\t"
		"move.l %2, %%d2\n\t"
		"move.b #82, %%d0\n\t"
		"trap #15\n\t"

		: /*outputs*/
	: "g" (white), "g" (x), "g" (y) /*inputs*/
		: "d0", "d1", "d2" /*clobbers*/
		);
}
#endif

void draw_square(short x_pos, short y_pos, short width, short height, short col)
{
	for (short y = y_pos; y < y_pos + height; y++)
		for (short x = x_pos; x < x_pos + width; x++)
		{
			plot_pixel(x, y, col);
		}
}

void clear_screen(int col)
{
	for (int y = 0; y < s_height; y++)
		for (int x = 0; x < s_width; x++)
		{
			plot_pixel(x, y, col);
		}
}

void paint_screen(void)
{
	COORD bufferSize = { s_scaleFactorX * s_width, s_scaleFactorY * s_height };
	COORD characterPos = { 0, 0 };
	SMALL_RECT writeArea = { 0, 0, s_scaleFactorX * s_width - 1, s_scaleFactorY * s_height - 1 };

	WriteConsoleOutputA(wHnd, s_consoleBuffer, bufferSize, characterPos, &writeArea);
	memcpy(&s_lastConsoleBuffer, &s_consoleBuffer, sizeof(s_consoleBuffer));
}

int main()
{
	//get the console handles
	wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	rHnd = GetStdHandle(STD_INPUT_HANDLE);

	assert(wHnd != 0);
	assert(rHnd != 0);

	//change font size
	//http://stackoverflow.com/questions/36590430/in-windows-does-setcurrentconsolefontex-change-consoles-font-size
	CONSOLE_FONT_INFOEX cfi = { sizeof(cfi) };
	BOOL ok = GetCurrentConsoleFontEx(wHnd, FALSE, &cfi);
	assert(ok == TRUE);

	cfi.dwFontSize.X = 1;
	cfi.dwFontSize.Y = 2;

	ok = SetCurrentConsoleFontEx(wHnd, FALSE, &cfi);
	assert(ok == TRUE);

	//change the window dims
	SMALL_RECT windowSize = { 0, 0, s_scaleFactorX * s_width - 1, s_scaleFactorY * s_height - 1};
	ok = SetConsoleWindowInfo(wHnd, TRUE, &windowSize);
	assert(ok == TRUE);

	//and the buffer size
	COORD bufferSize = { s_scaleFactorX * s_width, s_scaleFactorY * s_height };
	ok = SetConsoleScreenBufferSize(wHnd, bufferSize);
	assert(ok == TRUE);

	memset(&s_lastConsoleBuffer, sizeof(s_lastConsoleBuffer), 0x7f);
	clear_screen(0);

	short x = 128 - 25;
	short y = 128 - 25;

	while (1)
	{
		DWORD numEvents, numEventsRead;
		ok = GetNumberOfConsoleInputEvents(rHnd, &numEvents);
		assert(ok == TRUE);

		if (numEvents != 0)
		{
			INPUT_RECORD *eventBuffer = new INPUT_RECORD[numEvents];
			ReadConsoleInput(rHnd, eventBuffer, numEvents, &numEventsRead);

			for (DWORD count = 0; count < numEventsRead; count++)
			{
				if (eventBuffer[count].EventType == KEY_EVENT)
				{
					switch (eventBuffer[count].Event.KeyEvent.wVirtualKeyCode)
					{
					case VK_ESCAPE:
						exit(0);
					case VK_LEFT:
						x--;
						break;
					case VK_RIGHT:
						x++;
						break;
					case VK_UP:
						y--;
						break;
					case VK_DOWN:
						y++;
						break;
					default:
						break;
					}
				}
			}

			delete[] eventBuffer;
		}

		clear_screen(0);

		draw_square(x, y, 50, 50, 1);

		paint_screen();
	}


    return 0;
}

