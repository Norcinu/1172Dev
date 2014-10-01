#ifndef HELPSTATE_H
#define HELPSTATE_H

#include <State.h>

class HelpState : public State
{
public:
	HelpState();
	~HelpState();
	void Enter() override;
	void Update() override;
	void Exit() override;
private:
	float m_timeOut;
	bool mTotalWonVisible;//box showing total won is displayed so show it again on exit
	 int mHelpScreenNumber;
	unsigned int mhelpScreenID;
	unsigned int mhelpButtonID;
};

#endif HELPSTATE_H