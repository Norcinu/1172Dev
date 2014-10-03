#ifndef SETUPSTATE_H
#define SETUPSTATE_H

#include <State.h>

class SetupState : public State
{
public:
	SetupState();
	~SetupState();

	void Enter() override;
	void Update() override;
	void Exit() override;
};

#endif
