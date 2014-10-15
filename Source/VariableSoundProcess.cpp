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

	if (ENGINE->GetSystemTimer().GetRunningTime() < mVariableSoundDelayTimer)
	{
		return;
	}

	if (mRepeatNumber)
	{
		TheAudioManager::Instance()->GetAudioSample(mSoundName)->Play();
		mVariableSoundDelayTimer = ENGINE->GetSystemTimer().GetRunningTime() + POKER_GAME->GeneralSetGameDelay(POKER_GAME->GetSoundDelay(mSoundNameIndex,0));
		mRepeatNumber--;
	}
	else		 			
		SetCompleted();
}