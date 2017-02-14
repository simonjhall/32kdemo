#pragma once
void font_init(void);
short get_line_height(void);

short draw_string(short x_pos, short y_pos, const char *pString, short length);
short clear_string(short x_pos, short y_pos, const char *pString, short length);

short draw_hex_number(short x_pos, short y_pos, unsigned long num);
short draw_hex_number(short x_pos, short y_pos, unsigned short num);