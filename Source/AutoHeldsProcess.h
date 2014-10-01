#ifndef _AUTOHELDSPROCESS_H
#define _AUTOHELDSPROCESS_H

#include <GameProcess.h>

class AutoHeldsProcess : public GameProcess
{
public:
	AutoHeldsProcess();
	~AutoHeldsProcess();
	void Complete();
	void Update();
private:
	int mAutoHeldsIndex;
	float mAutoHeldsDelayTimer;

};

#endif _AUTOHELDSPROCESS_H