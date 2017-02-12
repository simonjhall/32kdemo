#include "common.h"
#include "screen.h"
#include "text.h"

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

Common *g_pCommon;
Chars *g_pChars;
unsigned short g_numChars;
const unsigned char *g_pFirstPixel;
unsigned char g_charMapping[256];

static inline unsigned char get_font_pixel(unsigned short x, unsigned short y)
{
	unsigned char p = g_pFirstPixel[y * (256 >> 3) + (x >> 3)];
	//for some reason the pixels are inverted in the image
	p = ~p;
	return (p >> (7 - (x & 7))) & 1;
}

static inline unsigned char get_font_pixel_packed(unsigned short x, unsigned short y)
{
	unsigned char p = g_pFirstPixel[y * (256 >> 3) + (x >> 3)];
	//for some reason the pixels are inverted in the image
	p = ~p;
	return p;
}

static void build_char_mapping(void)
{
	for (unsigned short outer = 0; outer < 256; outer++)
	{
		for (unsigned short inner = 0; inner < g_numChars; inner++)
		{
			ASSERT((g_pChars->m_char[inner].width[0] & 7) == 0);
			ASSERT((g_pChars->m_char[inner].x[0] & 7) == 0);

			if (g_pChars->m_char[inner].id[0] == outer)
			{
				g_charMapping[outer] = (unsigned char)inner;
				break;
			}
		}
	}
}

static short draw_character_slow(short x_pos, short y_pos, unsigned char c)
{
	//find the character
	unsigned short found = g_charMapping[c];

	for (unsigned short y = 0; y < g_pChars->m_char[found].height[0]; y++)
		for (unsigned short x = 0; x < g_pChars->m_char[found].width[0]; x++)
			plot_pixel(x_pos + x + g_pChars->m_char[found].xoffset[0], y_pos + y + g_pChars->m_char[found].yoffset[0],
				get_font_pixel(x + g_pChars->m_char[found].x[0], y + g_pChars->m_char[found].y[0]));

	return g_pChars->m_char[found].xadvance[0];
}

static short draw_character_8wide(short x_pos, short y_pos, unsigned char c)
{
	ASSERT((x_pos & 7) == 0);

	//find the character
	unsigned short found = g_charMapping[c];

	for (unsigned short y = 0; y < g_pChars->m_char[found].height[0]; y++)
	{
		unsigned char p = get_font_pixel_packed(g_pChars->m_char[found].x[0], y + g_pChars->m_char[found].y[0]);
		plot_packed_pixel(x_pos + g_pChars->m_char[found].xoffset[0], y_pos + y + g_pChars->m_char[found].yoffset[0], p);
	}

	return g_pChars->m_char[found].xadvance[0];
}

static short clear_character_8wide(short x_pos, short y_pos, unsigned char c)
{
	ASSERT((x_pos & 7) == 0);

	//find the character
	unsigned short found = g_charMapping[c];

	for (unsigned short y = 0; y < g_pChars->m_char[found].height[0]; y++)
	{
		plot_packed_pixel(x_pos + g_pChars->m_char[found].xoffset[0], y_pos + y + g_pChars->m_char[found].yoffset[0], 0);
	}

	return g_pChars->m_char[found].xadvance[0];
}

void draw_string(short x_pos, short y_pos, const char *pString, short length)
{
	for (short count = 0; count < length; count++)
		x_pos += draw_character_8wide(x_pos, y_pos, pString[count]);
}

void clear_string(short x_pos, short y_pos, const char *pString, short length)
{
	for (short count = 0; count < length; count++)
		x_pos += clear_character_8wide(x_pos, y_pos, pString[count]);
}

void font_init(void)
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
}
