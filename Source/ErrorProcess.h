#ifndef ERRORPROCESS_H
#define ERRORPROCESS_H

#include <GameProcess.h>

class ErrorProcess : public GameProcess
{
public:
	ErrorProcess();
	~ErrorProcess();
	void Complete();
	void Init();
	void Update();
private:
	bool mHadError;
	long m_currentError;
};

#endif ERRORPROCESS_H