#ifndef VARIABLESOUNDPROCESS_H
#define VARIABLEPROCESSPROCESS_H

#include <GameProcess.h>

class VariableSoundProcess : public GameProcess
{
public:
	VariableSoundProcess(char *name, int nameIndex, int RepeatNumber);
	~VariableSoundProcess();
	void Complete();	
	void Update();
private:
	float mVariableSoundDelayTimer;	
	int   mRepeatNumber;
	char *mSoundName;
	int   mSoundNameIndex;
};

#endif VARIABLEPROCESSPROCESS_H