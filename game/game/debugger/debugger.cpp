#include "debugger.h"

void addr_error(void) __attribute__((interrupt_handler));
void addr_error(void)
{
}

void illegal(void) __attribute__((interrupt_handler));
void illegal(void)
{
}

void zero_divide(void) __attribute__((interrupt_handler));
void zero_divide(void)
{
}

typedef void (*Handler)(void);

static Handler * const g_pVectorTable = 0;

static void install(Handler func, long offset)
{
	g_pVectorTable[offset] = func;
}

int c;
void install_debugger(void)
{
	install(addr_error, 3);
	install(illegal, 4);
	install(zero_divide, 5);

	volatile long a = 0;
	volatile int b = 0;
	c = a / b;
}
