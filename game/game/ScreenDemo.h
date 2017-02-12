#pragma once
#include "MiniGame.h"

class ScreenDemo : public MiniGame
{
public:
	ScreenDemo();

	virtual void Update(unsigned char pad, unsigned long frame);
	virtual void Render(void);
	virtual void Clear(void);

private:
	short m_x = 128 - 25;
	short m_y = 128 - 25;
};

