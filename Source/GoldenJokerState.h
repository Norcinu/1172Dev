#ifndef GOLDENJOKERSTATE_H
#define GOLDENJOKERSTATE_H

#include <State.h>

class GoldenJokerState : public State
{
public:
	GoldenJokerState();
	~GoldenJokerState();
	void Enter() override;
	void Update() override;
	void Exit() override;
private:

};

#endif GOLDENJOKERSTATE_H