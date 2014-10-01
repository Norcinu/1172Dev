#ifndef _UPDATEPROCESS_H
#define _UPDATEPROCESS_H

#include <GameProcess.h>

class UpdateProcess : public GameProcess
{
public:
	UpdateProcess();
	~UpdateProcess(){}
	void Update();
};

#endif _UPDATEPROCESS_H