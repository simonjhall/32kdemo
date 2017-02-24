
#include "MiniGame.h"

#include "Cube.h"

class Test3D :
	public MiniGame
{
public:
	Test3D();

	virtual void Update(unsigned char pad, unsigned long frame);
	virtual void Render(void);
	virtual void Clear(void);

private:
	RnD::Cube m_cube;
	float m_angleX, m_angleY;
};
