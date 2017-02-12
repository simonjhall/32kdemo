#pragma once
class MiniGame
{
public:
	virtual void Update(unsigned char pad, unsigned long frame);
	virtual void Render(void);
	virtual void Clear(void);

protected:
	inline MiniGame(void) {};
};

