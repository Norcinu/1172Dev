#ifndef DEALSTARTSTATE_H
#define DEALSTARTSTATE_H

#include <State.h>

class DealStartState : public State
{
public:
	DealStartState();
	~DealStartState();
	void Enter() override;
	void Update() override;
	void Exit() override;
private:
	unsigned int lCredit; //Local credit for autoplay cancelling
	float mHelpActiveDelayTimer;
};

#endif DEALSTARTSTATE_H