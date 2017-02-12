// game.cpp : Defines the entry point for the console application.
//

#ifdef _WIN32
#include "stdafx.h"
#endif

#include "screen.h"
#include "pad.h"
#include "common.h"

#include "small_font.h"
#include "small_font_0.h"

//http://www.angelcode.com/products/bmfont/doc/file_format.html#bin
struct Info
{
	char type;
	unsigned char size[4];

	unsigned char fontSize[2];
	unsigned char bitField;
	unsigned char charSet;
	unsigned char stretchH[2];
	unsigned char aa;
	unsigned char paddingUp, paddingRight, paddingDown, paddingLeft;
	unsigned char spacingHoriz, spacingVert;
	unsigned char outline;
};

struct Common
{
	char type;
	unsigned char size[4];

	unsigned char lineHeight[2];
	unsigned char base[2];
	unsigned char scaleW[2];
	unsigned char scaleH[2];
	unsigned char pages[2];
	unsigned char bitField;
	unsigned char alphaChnl;
	unsigned char redChnl;
	unsigned char greenChnl;
	unsigned char blueChnl;
};

struct Pages
{
	char type;
	unsigned char size[4];
};

struct Chars
{
	char type;
	unsigned char size[4];

	struct Char
	{
		unsigned char id[4];
		unsigned char x[2];
		unsigned char y[2];
		unsigned char width[2];
		unsigned char height[2];
		unsigned char xoffset[2];
		unsigned char yoffset[2];
		unsigned char xadvance[2];
		unsigned char page;
		unsigned char chnl;
	} m_char[1];
};

struct KerningPairs
{
	char type;
	unsigned char size[4];

	unsigned char first[4];
	unsigned char second[4];
	unsigned char amount[2];
};

struct FontHeader
{
	char ident[3]; char version;
	//info
	//common
	//pages
	//chars
	//kerning pairs
};

void draw_square(short x_pos, short y_pos, short width, short height, short col)
{
	for (short y = y_pos; y < y_pos + height; y++)
		for (short x = x_pos; x < x_pos + width; x++)
		{
			plot_pixel(x, y, col);
		}
}

Common *g_pCommon;
Chars *g_pChars;
unsigned short g_numChars;
const unsigned char *g_pFirstPixel;
unsigned char g_charMapping[256];

unsigned char get_font_pixel(unsigned short x, unsigned short y)
{
	unsigned char p = g_pFirstPixel[y * (256 >> 3) + (x >> 3)];
	//for some reason the pixels are inverted in the image
	p = ~p;
	return (p >> (7 - (x & 7))) & 1;
}

void build_char_mapping(void)
{
	for (unsigned short outer = 0; outer < 256; outer++)
	{
		for (unsigned short inner = 0; inner < g_numChars; inner++)
			if (g_pChars->m_char[inner].id[0] == outer)
			{
				g_charMapping[outer] = (unsigned char)inner;
				break;
			}
	}
}

short draw_character(short x_pos, short y_pos, unsigned char c)
{
	//find the character
	unsigned short found = g_charMapping[c];

	for (unsigned short y = 0; y < g_pChars->m_char[found].height[0]; y++)
		for (unsigned short x = 0; x < g_pChars->m_char[found].width[0]; x++)
			plot_pixel(x_pos + x + g_pChars->m_char[found].xoffset[0], y_pos + y + g_pChars->m_char[found].yoffset[0],
				get_font_pixel(x + g_pChars->m_char[found].x[0], y + g_pChars->m_char[found].y[0]));

	return g_pChars->m_char[found].xadvance[0];
}

void draw_string(short x_pos, short y_pos, const char *pString, short length)
{
	for (short count = 0; count < length; count++)
		x_pos += draw_character(x_pos, y_pos, pString[count]);
}

int main(void)
{
	FontHeader *pHeader = (FontHeader *)game_small_font_fnt;
	ASSERT(pHeader->ident[0] == 'B' && pHeader->ident[1] == 'M' && pHeader->ident[2] == 'F');
	ASSERT(pHeader->version == 3);

	Info *pInfo = (Info *)(&pHeader->version + 1);
	ASSERT(pInfo->type == 1);
	g_pCommon = (Common *)((char *)pInfo + pInfo->size[0] + 5);
	ASSERT(g_pCommon->type == 2);
	Pages *pPages = (Pages *)((char *)g_pCommon + g_pCommon->size[0] + 5);
	ASSERT(pPages->type == 3);
	g_pChars = (Chars *)((char *)pPages + pPages->size[0] + 5);
	ASSERT(g_pChars->type == 4);
	g_numChars = (g_pChars->size[0] + (g_pChars->size[1] << 8)) / 20;

	ASSERT(game_small_font_0_pbm[0] == 'P' && game_small_font_0_pbm[1] == '4');
	//code in drawing routine expects coords to only be in first byte
	ASSERT(game_small_font_0_pbm[3] == '2' && game_small_font_0_pbm[4] == '5' && game_small_font_0_pbm[5] == '6');
	g_pFirstPixel = &game_small_font_0_pbm[10];

	build_char_mapping();

	screen_init();

	clear_screen(0);

	//draw_character(50, 50, 'A');
	draw_string(50, 50, "the legend is true", 18);
	paint_screen();

	while (1);



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