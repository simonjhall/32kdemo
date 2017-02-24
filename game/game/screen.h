#pragma once

void screen_init(void);
void paint_screen(void);
void clear_screen(int col);

void plot_pixel(short x, short y, short col);
void plot_packed_pixel(short x, short y, int c);

void draw_square(short x_pos, short y_pos, short width, short height, short col);
void draw_line(int x0, int y0, int x1, int y1, short col);
void draw_fast_line(short x, short y, short x2, short y2, short col);