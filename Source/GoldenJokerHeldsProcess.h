#ifndef _GOLDENJOKERHELDSPROCESS_H
#define _GOLDENJOKERHELDSPROCESS_H

#include <GameProcess.h>

class GoldenJokerHeldsProcess : public GameProcess
{
public:
	GoldenJokerHeldsProcess();
	~GoldenJokerHeldsProcess();
	void Complete();
	void Update();
private:
	int mGoldenJokerHeldsIndex;
	float mGoldenJokerHeldsDelayTimer;

};

#endif _GOLDENJOKERHELDSPROCESS_H