#pragma once

void screen_init(void);
void paint_screen(void);
void clear_screen(int col);

void plot_pixel(short x, short y, short col);
void plot_packed_pixel(short x, short y, int c);

void draw_square(short x_pos, short y_pos, short width, short height, short col);


