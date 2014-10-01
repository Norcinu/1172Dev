#ifndef WINVALUEPROCESS_H
#define WINVALUEPROCESS_H

#include <GameProcess.h>

class WinValueProcess : public GameProcess
{
public:
	WinValueProcess(float winValue);
	~WinValueProcess();
	void Complete();
	void Init();
	void Update();
private:
	float m_animationTimer;
	float m_winValue;
	unsigned int m_counter;
	unsigned int m_winlineNumber;
};

#endif WINVALUEPROCESS_H