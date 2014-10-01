#ifndef PAYTRANSFERPROCESS_H
#define PAYTRANSFERPROCESS_H

#include <GameProcess.h>

class PayTransferProcess : public GameProcess
{
public:
	PayTransferProcess();
	~PayTransferProcess();
	void Complete();	
	void Update();
private:
	float mPayTransferDelayTimer;
	unsigned int mAmountToDeduct;
};

#endif PAYTRANSFERPROCESS_H