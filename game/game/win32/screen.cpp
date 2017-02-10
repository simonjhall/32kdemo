#include "../screen.h"
#include "../pad.h"
#include "../common.h"

#include <windows.h>
#include <assert.h>
#include <Windows.h>

HANDLE wHnd, rHnd;

//win32 character display

static const short s_scaleFactorX = 2;
static const short s_scaleFactorY = 1;

static CHAR_INFO s_consoleBuffer[(SCREEN_WIDTH * s_scaleFactorX) * (SCREEN_HEIGHT * s_scaleFactorY)];

void screen_init(void)
{
	//get the console handles
	wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	rHnd = GetStdHandle(STD_INPUT_HANDLE);

	ASSERT(wHnd != 0);
	ASSERT(rHnd != 0);

	//change font size
	//http://stackoverflow.com/questions/36590430/in-windows-does-setcurrentconsolefontex-change-consoles-font-size
	CONSOLE_FONT_INFOEX cfi = { sizeof(cfi) };
	BOOL ok = GetCurrentConsoleFontEx(wHnd, FALSE, &cfi);
	ASSERT(ok == TRUE);

	cfi.dwFontSize.X = 1;
	cfi.dwFontSize.Y = 2;

	ok = SetCurrentConsoleFontEx(wHnd, FALSE, &cfi);
	ASSERT(ok == TRUE);

	//change the buffer size first
	COORD bufferSize = { s_scaleFactorX * SCREEN_WIDTH, s_scaleFactorY * SCREEN_HEIGHT };
	ok = SetConsoleScreenBufferSize(wHnd, bufferSize);
	ASSERT(ok == TRUE);

	//then change the window dims
	SMALL_RECT windowSize = { 0, 0, s_scaleFactorX * SCREEN_WIDTH - 1, s_scaleFactorY * SCREEN_HEIGHT - 1 };
	ok = SetConsoleWindowInfo(wHnd, TRUE, &windowSize);
	ASSERT(ok == TRUE);
}

void plot_pixel(short x, short y, short col)
{
	x = x * s_scaleFactorX;
	y = y * s_scaleFactorY;

	if (x < 0 || y < 0)
		return;
	if (x >= SCREEN_WIDTH * s_scaleFactorX || y >= SCREEN_HEIGHT * s_scaleFactorY)
		return;

	//http://benryves.com/tutorials/winconsole/4
	s_consoleBuffer[(SCREEN_WIDTH * s_scaleFactorX) * y + x].Attributes = col ? (BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY) : 0;
}

void paint_screen(void)
{
	COORD bufferSize = { s_scaleFactorX * SCREEN_WIDTH, s_scaleFactorY * SCREEN_HEIGHT };
	COORD characterPos = { 0, 0 };
	SMALL_RECT writeArea = { 0, 0, s_scaleFactorX * SCREEN_WIDTH - 1, s_scaleFactorY * SCREEN_HEIGHT - 1 };

	WriteConsoleOutputA(wHnd, s_consoleBuffer, bufferSize, characterPos, &writeArea);
}

void clear_screen(int col)
{
	for (int y = 0; y < SCREEN_HEIGHT; y++)
		for (int x = 0; x < SCREEN_WIDTH; x++)
		{
			plot_pixel(x, y, col);
		}
}

unsigned char read_pad(unsigned int id)			//todo make a distinction!
{
	DWORD numEvents, numEventsRead;
	BOOL ok = GetNumberOfConsoleInputEvents(rHnd, &numEvents);
	ASSERT(ok == TRUE);

	unsigned char ret = 0;

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
				}
			}
		}

		delete[] eventBuffer;
	}

	return ret;
}
