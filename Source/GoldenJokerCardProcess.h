#ifndef _GOLDENJOKERCARDPROCESS_H
#define _GOLDENJOKERCARDPROCESS_H

#include <GameProcess.h>

class GoldenJokerCardProcess : public GameProcess
{
public:
	GoldenJokerCardProcess();
	~GoldenJokerCardProcess();
	void Complete();
	void Update();
private:
	int mCardIndex;
	float mCardDelayTimer;

};

#endif _GOLDENJOKERCARDPROCESS_H