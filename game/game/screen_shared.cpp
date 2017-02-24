#include "screen.h"

int abs(int x)
{
	if (x < 0)
		return -x;
	else
		return x;
}

inline void _set_colour(short col)
{
#ifdef __EASY__
	unsigned int white = (0 - (unsigned int)col) & 0xffffff;

	__asm__ __volatile__
	(
		//pen colour
		"move.l %0, %%d1\n\t"
		"move.b #80, %%d0\n\t"
		"trap #15\n\t"

		: /*outputs*/
		: "g" (white) /*inputs*/
		: "d0", "d1" /*clobbers*/
		);
#endif
}

inline void _plot_pixel(short x, short y, short col)
{
#ifdef __EASY__
	__asm__ __volatile__
	(
		//plot pixel
		"move.w %0, %%d1\n\t"				//ensure that the argument type size matches the move size here for x/y
		"move.w %1, %%d2\n\t"
		"move.b #82, %%d0\n\t"
		"trap #15\n\t"

		: /*outputs*/
		: "g" (x), "g" (y) /*inputs*/
		: "d0", "d1", "d2" /*clobbers*/
		);
#else
	plot_pixel(x, y, col);
#endif
}

void draw_square(short x_pos, short y_pos, short width, short height, short col)
{
	for (short y = y_pos; y < y_pos + height; y++)
		for (short x = x_pos; x < x_pos + width; x++)
		{
			plot_pixel(x, y, col);
		}
}

void draw_line(int x0, int y0, int x1, int y1, short col)
{

	int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = (dx > dy ? dx : -dy) / 2, e2;

	for (;;) {
		plot_pixel(x0, y0, col);
		if (x0 == x1 && y0 == y1) break;
		e2 = err;
		if (e2 > -dx) { err -= dy; x0 += sx; }
		if (e2 < dy) { err += dx; y0 += sy; }
	}
}

void draw_fast_line(short x, short y, short x2, short y2, short col)
{
	_set_colour(col);

	bool yLonger = false;
	short shortLen = y2 - y;
	short longLen = x2 - x;
	if (abs(shortLen) > abs(longLen)) {
		short swap = shortLen;
		shortLen = longLen;
		longLen = swap;
		yLonger = true;
	}
	short decInc;
	if (longLen == 0) decInc = 0;
	else
		decInc = (int)(shortLen << 8) / longLen;

	if (yLonger) {
		if (longLen > 0) {
			longLen += y;
			for (int j = 0x80 + (x << 8); y <= longLen; ++y) {
				_plot_pixel(j >> 8, y, col);
				j += decInc;
			}
			return;
		}
		longLen += y;
		for (int j = 0x80 + (x << 8); y >= longLen; --y) {
			_plot_pixel(j >> 8, y, col);
			j -= decInc;
		}
		return;
	}

	if (longLen > 0) {
		longLen += x;
		for (int j = 0x80 + (y << 8); x <= longLen; ++x) {
			_plot_pixel(x, j >> 8, col);
			j += decInc;
		}
		return;
	}
	longLen += x;
	for (int j = 0x80 + (y << 8); x >= longLen; --x) {
		_plot_pixel(x, j >> 8, col);
		j -= decInc;
	}
}
