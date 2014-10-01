#include <ObjectHandler.h>
#include "GenericButtons.h"
#include "Game.h"
#include "AutoHeldsProcess.h"
#include "CollectProcess.h"
#include "PokerGame.h"

AutoHeldsProcess::AutoHeldsProcess()
{
	mAutoHeldsIndex = 0;
	mAutoHeldsDelayTimer = 0;
}

AutoHeldsProcess::~AutoHeldsProcess()
{
	
}

void AutoHeldsProcess::Complete()
{
	
}

void AutoHeldsProcess::Update()
{
	if (TheEngine::Instance()->GetSystemTimer().GetRunningTime() < mAutoHeldsDelayTimer)
	{
		return;
	}

	if (mAutoHeldsIndex < 5)
	{
		if (ThePokerGame::Instance()->PokerHand[mAutoHeldsIndex].hold)
		{
			TheAudioManager::Instance()->GetAudioSample("CLICK")->Play();
			unsigned int HeldId = TheObjectHandler::Instance()->GetObject2D("Held01")->GetID()+mAutoHeldsIndex;
			TheObjectHandler::Instance()->GetObject2D(HeldId)->SetVisible(true);
			mAutoHeldsDelayTimer = TheEngine::Instance()->GetSystemTimer().GetRunningTime() + ThePokerGame::Instance()->GeneralSetGameDelay(0.15f);
		}
		mAutoHeldsIndex++;
	}
	else
		SetCompleted();

}