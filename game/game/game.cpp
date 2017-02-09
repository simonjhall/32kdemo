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

	float time = 0;

	while (1)
	{
		clear_screen(0);

		short x = short(sin(time) * 100);
		short y = short(cos(time) * 100);

		draw_square(128 + x - 25, 128 + y - 25, 50, 50, 1);

		paint_screen();

		time += 0.1f;
	}


    return 0;
}

