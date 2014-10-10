#ifndef DRAWHANDSTATE_H
#define DRAWHANDSTATE_H

#include <State.h>

class DrawHandState : public State
{
public:
	DrawHandState();
	~DrawHandState();
	void Enter() override;
	void Update() override;
	void Exit() override;
private:
	bool m_sleep;
	bool m_firstHand;
};

#endif DRAWHANDSTATE_H