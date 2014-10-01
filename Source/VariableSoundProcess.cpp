#include <ObjectHandler.h>
#include "Game.h"
#include "VariableSoundProcess.h"
#include "PokerGame.h"

VariableSoundProcess::VariableSoundProcess(char *name, int nameIndex, int RepeatNumber)
{
	mVariableSoundDelayTimer = 0;	
	mSoundNameIndex			 = nameIndex;
	mRepeatNumber			 = RepeatNumber;
	mSoundName				 = name;
}

VariableSoundProcess::~VariableSoundProcess()
{
	
}

void VariableSoundProcess::Complete()
{
	mVariableSoundDelayTimer = 0;
}

void VariableSoundProcess::Update()
{

	if (TheEngine::Instance()->GetSystemTimer().GetRunningTime() < mVariableSoundDelayTimer)
	{
		return;
	}

	if (mRepeatNumber)
	{
		TheAudioManager::Instance()->GetAudioSample(mSoundName)->Play();
		mVariableSoundDelayTimer = TheEngine::Instance()->GetSystemTimer().GetRunningTime() + ThePokerGame::Instance()->GeneralSetGameDelay(ThePokerGame::Instance()->GetSoundDelay(mSoundNameIndex,0));
		mRepeatNumber--;
	}
	else		 			
		SetCompleted();
}