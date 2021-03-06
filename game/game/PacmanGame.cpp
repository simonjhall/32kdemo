#ifdef _WIN32
#include <stdio.h>
#endif

#include "PacmanGame.h"

#include "common.h"
#include "screen.h"

static const char maze[]
{
	'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
	'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
	'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
	'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
	'0', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '0', '0', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '0',
	'0', '1', '0', '0', '0', '0', '1', '0', '0', '0', '0', '0', '1', '0', '0', '1', '0', '0', '0', '0', '0', '1', '0', '0', '0', '0', '1', '0',
	'0', '1', '0', '0', '0', '0', '1', '0', '0', '0', '0', '0', '1', '0', '0', '1', '0', '0', '0', '0', '0', '1', '0', '0', '0', '0', '1', '0',
	'0', '1', '0', '0', '0', '0', '1', '0', '0', '0', '0', '0', '1', '0', '0', '1', '0', '0', '0', '0', '0', '1', '0', '0', '0', '0', '1', '0',
	'0', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '0',
	'0', '1', '0', '0', '0', '0', '1', '0', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '1', '0', '0', '0', '0', '1', '0',
	'0', '1', '0', '0', '0', '0', '1', '0', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '1', '0', '0', '0', '0', '1', '0',
	'0', '1', '1', '1', '1', '1', '1', '0', '0', '1', '1', '1', '1', '0', '0', '1', '1', '1', '1', '0', '0', '1', '1', '1', '1', '1', '1', '0',
	'0', '0', '0', '0', '0', '0', '1', '0', '0', '0', '0', '0', '1', '0', '0', '1', '0', '0', '0', '0', '0', '1', '0', '0', '0', '0', '0', '0',
	'0', '0', '0', '0', '0', '0', '1', '0', '0', '0', '0', '0', '1', '0', '0', '1', '0', '0', '0', '0', '0', '1', '0', '0', '0', '0', '0', '0',
	'0', '0', '0', '0', '0', '0', '1', '0', '0', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '0', '0', '1', '0', '0', '0', '0', '0', '0',
	'0', '0', '0', '0', '0', '0', '1', '0', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '1', '0', '0', '0', '0', '0', '0',
	'0', '0', '0', '0', '0', '0', '1', '0', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '1', '0', '0', '0', '0', '0', '0',
	'1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '0', '0', '0', '0', '0', '0', '0', '0', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1',
	'0', '0', '0', '0', '0', '0', '1', '0', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '1', '0', '0', '0', '0', '0', '0',
	'0', '0', '0', '0', '0', '0', '1', '0', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '1', '0', '0', '0', '0', '0', '0',
	'0', '0', '0', '0', '0', '0', '1', '0', '0', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '0', '0', '1', '0', '0', '0', '0', '0', '0',
	'0', '0', '0', '0', '0', '0', '1', '0', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '1', '0', '0', '0', '0', '0', '0',
	'0', '0', '0', '0', '0', '0', '1', '0', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '1', '0', '0', '0', '0', '0', '0',
	'0', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '0', '0', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '0',
	'0', '1', '0', '0', '0', '0', '1', '0', '0', '0', '0', '0', '1', '0', '0', '1', '0', '0', '0', '0', '0', '1', '0', '0', '0', '0', '1', '0',
	'0', '1', '0', '0', '0', '0', '1', '0', '0', '0', '0', '0', '1', '0', '0', '1', '0', '0', '0', '0', '0', '1', '0', '0', '0', '0', '1', '0',
	'0', '1', '1', '1', '0', '0', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '0', '0', '1', '1', '1', '0',
	'0', '0', '0', '1', '0', '0', '1', '0', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '1', '0', '0', '1', '0', '0', '0',
	'0', '0', '0', '1', '0', '0', '1', '0', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '1', '0', '0', '1', '0', '0', '0',
	'0', '1', '1', '1', '1', '1', '1', '0', '0', '1', '1', '1', '1', '0', '0', '1', '1', '1', '1', '0', '0', '1', '1', '1', '1', '1', '1', '0',
	'0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0',
	'0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0',
	'0', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '0',
	'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
	'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
	'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
};

static const char cross[8]
{
	(const char)0b10000001,
	(const char)0b00000000,
	(const char)0b00000000,
	(const char)0b00000000,
	(const char)0b00000000,
	(const char)0b00000000,
	(const char)0b00000000,
	(const char)0b10000001,
};

static const char horiz_bar[8]
{
	(const char)0b11111111,
	(const char)0b00000000,
	(const char)0b00000000,
	(const char)0b00000000,
	(const char)0b00000000,
	(const char)0b00000000,
	(const char)0b00000000,
	(const char)0b11111111,
};

static const char vert_bar[8]
{
	(const char)0b10000001,
	(const char)0b10000001,
	(const char)0b10000001,
	(const char)0b10000001,
	(const char)0b10000001,
	(const char)0b10000001,
	(const char)0b10000001,
	(const char)0b10000001,
};

int c;
static void render_tile(short x, short y)
{
	//c = *(int *)1;
	/*volatile int a = 0, b = 0;
	c = a / b;*/
	//asm("bkpt #0");
	ASSERT(x >= 0 && x < 28);
	ASSERT(y >= 0 && y < 36);

	char t = maze[y * 28 + x];
	switch (t)
	{
	case '0':
		for (short count = 0; count < 8; count++)
			plot_packed_pixel(x * 8, y * 8 + count, 0xff);
		break;
		/*
	case '+':
		for (short count = 0; count < 8; count++)
			plot_packed_pixel(x * 8, y * 8 + count, cross[count]);
		break;
	case '-':
		for (short count = 0; count < 8; count++)
			plot_packed_pixel(x * 8, y * 8 + count, horiz_bar[count]);
		break;
	case '|':
		for (short count = 0; count < 8; count++)
			plot_packed_pixel(x * 8, y * 8 + count, vert_bar[count]);
		break;
	default:
		ASSERT(0);*/
	default:break;
	}
}

char new_maze[36 * 28];

bool get_tile(short x, short y, char &r)
{
	if (x < 0 || x >= 36)
		return false;
	if (y < 0 || y >= 28)
		return false;

	r = maze[y * 28 + x];

	return true;
}

struct E
{
	char entry[3][3];
	char value;
} entries[100];
int num_entries = 0;

bool find_entry(char entry[3][3], char &r)
{
	for (int count = 0; count < num_entries; count++)
	{
		int same = 0;
		for (int y = 0; y < 3; y++)
			for (int x = 0; x < 3; x++)
				if (entries[count].entry[y][x] == entry[y][x])
					same++;

		if (same == 9)
		{
			r = entries[count].value;
			return true;
		}
	}
	return false;
}


void set_entry(char entry[3][3], char v)
{
	char t;
	ASSERT(!find_entry(entry, t));

	for (int y = 0; y < 3; y++)
		for (int x = 0; x < 3; x++)
			entries[num_entries].entry[y][x] = entry[y][x];
	entries[num_entries++].value = v;
	ASSERT(num_entries < sizeof(entries));
}

PacmanGame::PacmanGame()
{
#ifdef _WIN32

	char type = 0;

	for (short y = 0; y < 36; y++)
		for (short x = 0; x < 28; x++)
		{
			char r;
			get_tile(x, y, r);

			if (r != '0')
			{
				new_maze[y * 28 + x] = r;
				continue;
			}
			
			char local[3][3];
			for (short j = -1; j <= 1; j++)
			{
				ASSERT(j >= -1 && j <= 1);
				for (short i = -1; i <= 1; i++)
				{
					ASSERT(i >= -1 && i <= 1);

					char r;
					if (get_tile(x + i, y + j, r))
						local[j + 1][i + 1] = r;
					else
						local[j + 1][i + 1] = 'x';
				}
			}
			
			char v;
			if (!find_entry(local, v))
			{
				new_maze[y * 28 + x] = 'A' + type;
				set_entry(local, 'A' + type++);
			}
			else
				new_maze[y * 28 + x] = v;
		}

	for (short y = 0; y < 36; y++)
	{
		for (short x = 0; x < 28; x++)
			putc(new_maze[y * 28 + x], stdout);
		putc('\n', stdout);
	}
#endif
}

void PacmanGame::Update(unsigned char pad, unsigned long frame)
{
}

void PacmanGame::Render(void)
{
	for (short y = 0; y < 36; y++)
		for (short x = 0; x < 28; x++)
			render_tile(x, y);
}

void PacmanGame::Clear(void)
{
}

