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

static inline unsigned long get_pc_group_0(void)
{
	/* The
	value saved for the program counter is advanced 2–10 bytes beyond the address of the
	first word of the instruction that made the reference causing the bus error. If the bus error
	occurred during the fetch of the next instruction, the saved program counter has a value in
	the vicinity of the current instruction, even if the current instruction is a branch, a jump, or
	a return instruction. */
	return *(unsigned long *)(exception_save.a.a.ss + 10);
}

static inline unsigned long get_addr_group_0(void)
{
	return *(unsigned long *)(exception_save.a.a.ss + 2);
}

static inline unsigned short get_insn_group_0(void)
{
	return *(unsigned long *)(exception_save.a.a.ss + 6);
}

static inline unsigned char get_code_group_0(void)
{
	return *(unsigned char *)(exception_save.a.a.ss + 1);
}

static inline unsigned long get_pc_group_1(void)
{
	return *(unsigned long *)(exception_save.a.a.ss + 2);					//stops on the faulty instruction
}

static inline unsigned long get_pc_group_2(void)
{
	return *(unsigned long *)(exception_save.a.a.ss + 2) - 2;				//-2 as chk and div are two bytes, trap is not always though
}

static inline unsigned short get_sr_group_0(void)
{
	return *(unsigned short *)(exception_save.a.a.ss + 8);
}

static inline unsigned short get_sr_group_12(void)
{
	return *(unsigned short *)exception_save.a.a.ss;
}

static inline bool was_supervisor(unsigned short sr)
{
	if (sr & (1 << 13))
		return true;
	else
		return false;
}

static short print_group_0(short _x, short y)
{
	short x = draw_string(_x, y, "PC ", 3);
	x = draw_hex_number(x, y, get_pc_group_0());

	x = draw_string(x + 8, y, "SR ", 3);
	x = draw_hex_number(x, y, get_sr_group_0());

	if (was_supervisor(get_sr_group_0()))
		x = draw_string(x + 8, y, "S", 1);
	else
		x = draw_string(x + 8, y, "U", 1);

	x += 8;

	if (get_code_group_0() & (1 << 3))
		x = draw_string(x, y, "IF ", 3);
	else
	{
		x = draw_string(x, y, "D", 1);
		if (get_code_group_0() & (1 << 4))
			x = draw_string(x, y, "R", 1);
		else
			x = draw_string(x, y, "W", 1);
	}

	x = draw_hex_number(x - 24, y - get_line_height(), get_addr_group_0());

	y += get_line_height();

	return y;
}

static short print_group_12(short _x, short y, unsigned long pc)
{
	short x = draw_string(_x, y, "PC ", 3);
	x = draw_hex_number(x, y, pc);

	x = draw_string(x + 24, y, "SR ", 3);
	x = draw_hex_number(x, y, get_sr_group_12());

	if (was_supervisor(get_sr_group_12()))
		draw_string(x + 24, y, "supervisor", 10);
	else
		draw_string(x + 24, y, "user", 4);

	y += get_line_height();

	return y;
}

static short print_stack(short _x, short y, long up, long down, unsigned long stack)
{
	//align the stack
	stack = stack & ~3;

	for (unsigned long s = stack - down; s <= stack + up; s += 4)
	{
		short x = draw_hex_number(_x, y, (unsigned short)s);
		x = draw_hex_number(x + 8, y, *(unsigned long *)s);

		if (s == stack)
			x = draw_string(x + 8, y, "<", 1);

		y += get_line_height();
	}

	return y;
}

static short print_backtrace(short _x, short y, unsigned long a6, short depth)
{
	while (depth-- >= 0)
	{
		unsigned short x = _x;
		if (a6 & 3)
		{
			x = draw_hex_number(x, y, a6);
			x = draw_string(x + 8, y, "invalid fp", 10);
			return y;
		}

		unsigned long next_a6 = *(unsigned long *)a6;
		unsigned long lr = *(unsigned long *)(a6 + 4);

		x = draw_hex_number(x, y, lr);

		if (next_a6 < a6)			//probably junk
			break;

		a6 = next_a6;
		y += get_line_height();
	}

	return y;
}

extern "C" void _addr_error(void);

extern "C" void addr_error(void) __attribute__((interrupt_handler));
void addr_error(void)
{
	int y = 8;
	int x = 8;
	draw_string(x, y, "address error", 13);
	y += get_line_height();

	y = print_group_0(x, y);
	
	print_registers(x, y);

	y += get_line_height() * (7 + 2);

	print_stack(x, y, 8, 4,
		was_supervisor(get_sr_group_0()) ? exception_save.a.a.ss : exception_save.a.a.us);

	print_backtrace(x + 16 * 8, y, exception_save.a.a.a6, 5);
}

extern "C" void _illegal(void);

extern "C" void illegal(void) __attribute__((interrupt_handler));
void illegal(void)
{
	int y = 8;
	int x = 8;
	draw_string(x, y, "illegal instruction", 19);
	y += get_line_height();

	y = print_group_12(x, y, get_pc_group_2());

	print_registers(x, y);

	y += get_line_height() * (7 + 2);

	print_stack(x, y, 8, 4,
		was_supervisor(get_sr_group_12()) ? exception_save.a.a.ss : exception_save.a.a.us);

	print_backtrace(x + 16 * 8, y, exception_save.a.a.a6, 5);
}

extern "C" void _zero_divide(void);

extern "C" void zero_divide(void) __attribute__((interrupt_handler));
void zero_divide(void)
{
	int y = 8;
	int x = 8;
	draw_string(x, y, "divide by zero", 14);
	y += get_line_height();

	y = print_group_12(x, y, get_pc_group_2());

	print_registers(x, y);

	y += get_line_height() * (7 + 2);

	print_stack(x, y, 8, 4,
		was_supervisor(get_sr_group_12()) ? exception_save.a.a.ss : exception_save.a.a.us);

	print_backtrace(x + 16 * 8, y, exception_save.a.a.a6, 5);
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
