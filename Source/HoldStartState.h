#ifndef HOLDSTARTSTATE_H
#define HOLDSTARTSTATE_H

#include <State.h>

class HoldStartState : public State
{
public:
	HoldStartState();
	~HoldStartState();
	void Enter() override;
	void Update() override;
	void Exit() override;
private:
	unsigned int lCredit; //Local credit for autoplay cancelling
	float mHelpActiveDelayTimer;
};

#endif HOLDSTARTSTATE_H