#pragma once
#include "MiniGame.h"




class PacmanGame : public MiniGame
{
public:
	PacmanGame();

	virtual void Update(unsigned char pad, unsigned long frame);
	virtual void Render(void);
	virtual void Clear(void);
};

