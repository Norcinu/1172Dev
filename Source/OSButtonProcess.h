#ifndef OSBUTTONPROCESS_H
#define OSBUTTONPROCESS_H

#include <GameProcess.h>

class OSButton;
class Object2D;

class OSButtonProcess : public GameProcess
{
public:
	OSButtonProcess(OSButton* button);
	~OSButtonProcess();
	void Complete();
	void Update();
private:
	OSButton* mButton;
	Object2D* mButtonObject;
};

#endif OSBUTTONPROCESS_H