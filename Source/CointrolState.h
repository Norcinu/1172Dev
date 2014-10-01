#ifndef COINTROLSTATE_H
#define COINTROLSTATE_H

#include <State.h>

class CointrolState : public State
{
public:
	CointrolState();
	~CointrolState();
	void Enter() override;
	void Update() override;
	void Exit() override;
private:

};

#endif COINTROLSTATE_H