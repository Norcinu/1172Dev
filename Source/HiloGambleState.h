#ifndef HILOGAMBLESTATE_H
#define HILOGAMBLESTATE_H

#include <State.h>

class HiloGambleState : public State
{
public:
	HiloGambleState();
	~HiloGambleState();
	void Enter() override;
	void Update() override;
	void Exit() override;
private:

};

#endif HILOGAMBLESTATE_H