#include "debugger.h"
#include "../text.h"

struct ExceptionState
{
	union
	{
		unsigned long A[9];
		struct _A
		{
			unsigned long a0, a1, a2, a3, a4, a5, a6, ss, us;
		} a;
	} a;
	union
	{
		unsigned long D[8];
		struct _D
		{
			unsigned long d0, d1, d2, d3, d4, d5, d6, d7;
		} d;
	} d;
};

extern ExceptionState exception_save;

static void print_registers(short _x, short y)
{
	for (int count = 0; count < 9; count++)
	{
		int x = _x;

		//A0-6
		if (count < 7)
		{
			x = draw_string(x, y, "A", 1);
			char c = '0' + count;
			x = draw_string(x, y, &c, 1);
		}
		else if (count == 7)					//A7 SS
			x = draw_string(x, y, "SS", 2);
		else
			x = draw_string(x, y, "US", 2);		//A7 US

												//the register
		x = draw_hex_number(x + 8, y, exception_save.a.A[count]);

		if (count < 8)
		{
			//D0-7
			x += 24;
			x = draw_string(x, y, "D", 1);
			char c = '0' + count;
			x = draw_string(x, y, &c, 1);

			x = draw_hex_number(x + 8, y, exception_save.d.D[count]);
		}

		//next line
		y += get_line_height();
	}
}

static unsigned long get_pc_group_12(void)
{
	return *(unsigned long *)(exception_save.a.a.ss + 2);
}

static unsigned short get_sr_group_12(void)
{
	return *(unsigned short *)exception_save.a.a.ss;
}

static bool was_supervisor(unsigned short sr)
{
	if (sr & (1 << 13))
		return true;
	else
		return false;
}

static void print_group_12(short _x, short y)
{
	short x = draw_string(_x, y, "PC ", 3);
	x = draw_hex_number(x, y, get_pc_group_12());

	x = draw_string(x + 24, y, "SR ", 3);
	x = draw_hex_number(x, y, get_sr_group_12());

	if (was_supervisor(get_sr_group_12()))
		draw_string(x + 24, y, "supervisor", 10);
	else
		draw_string(x + 24, y, "user", 4);
}

static void print_stack(short _x, short y, long up, long down, unsigned long stack)
{
	for (unsigned long s = stack - down; s <= stack + up; s += 4)
	{
		short x = draw_hex_number(_x, y, s);
		x = draw_hex_number(x + 24, y, *(unsigned long *)s);

		if (s == stack)
			x = draw_string(x + 16, y, "<=", 2);

		y += get_line_height();
	}
}

extern "C" void _addr_error(void);

extern "C" void addr_error(void) __attribute__((interrupt_handler));
void addr_error(void)
{
}

extern "C" void _illegal(void);

extern "C" void illegal(void) __attribute__((interrupt_handler));
void illegal(void)
{
}

extern "C" void _zero_divide(void);

extern "C" void zero_divide(void) __attribute__((interrupt_handler));
void zero_divide(void)
{
	int y = 0;
	draw_string(0, y, "illegal instruction", 19);
	y += get_line_height();

	print_group_12(16, y);

	y += get_line_height() * 2;
	
	print_registers(16, y);

	y += get_line_height() * (9 + 2);

	print_stack(16, y, 16, 16,
		was_supervisor(get_sr_group_12()) ? exception_save.a.a.ss : exception_save.a.a.us);
}

typedef void (*Handler)(void);

static Handler * const g_pVectorTable = 0;

static void install(Handler func, long offset)
{
	g_pVectorTable[offset] = func;
}

void install_debugger(void)
{
	install(_addr_error, 3);
	install(_illegal, 4);
	install(_zero_divide, 5);
}
