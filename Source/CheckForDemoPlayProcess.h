#ifndef CHECKFORDEMOPLAYPROCESS_H
#define CHECKFORDEMOPLAYPROCESS_H

#include <GameProcess.h>

class CheckForDemoPlayProcess : public GameProcess
{
public:
	CheckForDemoPlayProcess();
	~CheckForDemoPlayProcess();
	virtual void Init() override;
	void Update() override;
	void Complete() override;
	void AddDemoModeCredits();
private:
	bool mNoteValidatorFlag;
	bool mFinishedDemoGame;
};

#endif