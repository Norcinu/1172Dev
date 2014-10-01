#ifndef _COLLECTPROCESS_H
#define _COLLECTPROCESS_H

#include <GameProcess.h>

class CollectProcess : public GameProcess
{
public:
	CollectProcess();
	~CollectProcess();
	void Complete();
	void Init();
	void Update();
	bool Payout();
private:
	bool mPayoutConfirmed;
	bool mPayoutStarted;
	unsigned char mType;
	unsigned char mPayoutState;

};

#endif _COLLECTPROCESS_H