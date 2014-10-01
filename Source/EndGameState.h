#ifndef ENDGAMESTATE_H
#define ENDGAMESTATE_H

#include <State.h>

class EndGameState : public State
{
public:
	EndGameState();
	~EndGameState();
	void Enter() override;
	void Update() override;
	void Exit() override;
private:
	float mEndGameDelayTimer;

};

#endif ENDGAMESTATE_H