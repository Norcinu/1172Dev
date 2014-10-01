#ifndef BUTTONPROCESS_H
#define BUTTONPROCESS_H

#include <GameProcess.h>

class HardwareButton;

class ButtonProcess : public GameProcess
{
public:
	ButtonProcess(HardwareButton* button);
	~ButtonProcess();
	void Complete();
	void Update();
private:
	HardwareButton* mButton;
};

#endif BUTTONPROCESS_H