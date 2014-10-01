#ifndef DEALHANDSTATE_H
#define DEALHANDSTATE_H

#include <State.h>

class DealHandState : public State
{
public:
	DealHandState();
	~DealHandState();
	void Enter() override;
	void Update() override;
	void Exit() override;
private:

};

#endif DEALHANDSTATE_H