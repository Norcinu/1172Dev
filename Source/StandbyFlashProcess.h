#ifndef STANDBYFLASHPROCESS_H
#define STANDBYFLASHPROCESS_H

#include <GameProcess.h>

class StandbyFlashProcess : public GameProcess
{
public:
	StandbyFlashProcess();
	~StandbyFlashProcess();
	void Complete();
	void Init();
	void Update();
private:
	bool m_alphaSwitch;
	D3DXCOLOR m_standbyColour;
	float m_animationTime;
};

#endif STANDBYFLASHPROCESS_H