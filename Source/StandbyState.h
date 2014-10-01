#ifndef STANDBYSTATE_H
#define STANDBYSTATE_H

#include <State.h>
#include "StandbyFlashProcess.h"

#define STANDBY_TIME (300.0f) //Time in Seconds - 5 Minutes

class StandbyState : public State
{
public:
	StandbyState();
	~StandbyState();
	void Enter() override;
	void Update() override;
	void Exit() override;
private:
	float m_standbyTimer;
	StandbyFlashProcess* m_standbyFlash;
	unsigned int m_previousCredits;
};

#endif STANDBYSTATE_H