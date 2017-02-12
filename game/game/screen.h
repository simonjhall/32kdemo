#pragma once

void screen_init(void);
void plot_pixel(short x, short y, short col);
void plot_packed_pixel(short x, short y, int c);
void paint_screen(void);
void clear_screen(int col);

