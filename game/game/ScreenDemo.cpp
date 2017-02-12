#include "ScreenDemo.h"

#include "common.h"
#include "screen.h"
#include "pad.h"
#include "text.h"


ScreenDemo::ScreenDemo()
{
}

void ScreenDemo::Update(unsigned char pad, unsigned long frame)
{
	if (pad & PAD_UP_SH)
		m_y--;
	if (pad & PAD_DOWN_SH)
		m_y++;
	if (pad & PAD_LEFT_SH)
		m_x--;
	if (pad & PAD_RIGHT_SH)
		m_x++;
}

void ScreenDemo::Render(void)
{
	draw_square(m_x, m_y, 50, 50, 1);
	draw_string(m_x & ~7, m_y - 16, "the legend is true", 18);
}

void ScreenDemo::Clear(void)
{
	clear_string(m_x & ~7, m_y - 16, "the legend is true", 18);
	draw_square(m_x, m_y, 50, 50, 0);

}

