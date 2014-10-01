#ifndef PAYMENTSTATE_H
#define PAYMENTSTATE_H

#include <State.h>

class PaymentState : public State
{
public:
	PaymentState();
	~PaymentState();
	void Enter() override;
	void Update() override;
	void Exit() override;
private:

};

#endif PAYMENTSTATE_H