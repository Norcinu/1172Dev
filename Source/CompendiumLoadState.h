#ifndef COMPENDIUM_LOAD_STATE_H
#define COMPENDIUM_LOAD_STATE_H

#include <State.h>

class CompendiumLoadState : public State
{
public:
	CompendiumLoadState(){}
	~CompendiumLoadState(){}
	void Enter();
	void Exit();
	void Update();
private:
	float mTimer;
};

#endif