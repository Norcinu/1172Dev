#ifndef _POKERHANDPROCESS_H
#define _POKERHANDPROCESS_H

#include <GameProcess.h>

class PokerHandProcess : public GameProcess
{
public:
	PokerHandProcess();
	~PokerHandProcess();
	void Complete();
	void Update();
private:
	int mCardIndex;
	float mCardDelayTimer;

};

#endif _POKERHANDPROCESS_H