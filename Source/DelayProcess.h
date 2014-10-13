#ifndef DELAY_PROCESS_H
#define DELAY_PROCESS_H

#include <GameProcess.h>

class DelayProcess : public GameProcess
{
public:
	DelayProcess(float holdTime);
	~DelayProcess(){}
	void Init();
	void Update();
	void Abort(){}
private:
	float mTimer;
	float mHoldTime;
};

#endif