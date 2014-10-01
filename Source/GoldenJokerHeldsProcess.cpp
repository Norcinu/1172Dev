#include <ObjectHandler.h>
#include "GenericButtons.h"
#include "Game.h"
#include "GoldenJokerHeldsProcess.h"
#include "CollectProcess.h"
#include "PokerGame.h"

GoldenJokerHeldsProcess::GoldenJokerHeldsProcess()
{
	mGoldenJokerHeldsIndex = 0;
	mGoldenJokerHeldsDelayTimer = 0;
	ThePokerGame::Instance()->GoldenJokerHeldProcessEnd = false;
}

GoldenJokerHeldsProcess::~GoldenJokerHeldsProcess()
{
	
}

void GoldenJokerHeldsProcess::Complete()
{
	ThePokerGame::Instance()->GoldenJokerHeldProcessEnd = true;
}

void GoldenJokerHeldsProcess::Update()
{
	if (TheEngine::Instance()->GetSystemTimer().GetRunningTime() < mGoldenJokerHeldsDelayTimer)
	{
		return;
	}

	if (mGoldenJokerHeldsIndex < 5)
	{		
		unsigned int HeldId = TheObjectHandler::Instance()->GetObject2D("Held01")->GetID()+mGoldenJokerHeldsIndex;
		if (ThePokerGame::Instance()->PokerHand[mGoldenJokerHeldsIndex].hold)		
		{
			TheAudioManager::Instance()->GetAudioSample("CLICK")->Play();
			TheObjectHandler::Instance()->GetObject2D(HeldId)->SetVisible(true);												
			mGoldenJokerHeldsDelayTimer = TheEngine::Instance()->GetSystemTimer().GetRunningTime() + ThePokerGame::Instance()->GeneralSetGameDelay(0.2f);
		}
		else
		{
			if (TheObjectHandler::Instance()->GetObject2D(HeldId)->GetVisible())
			{
				TheAudioManager::Instance()->GetAudioSample("CLICK")->Play();
				TheObjectHandler::Instance()->GetObject2D(HeldId)->SetVisible(false);													
				mGoldenJokerHeldsDelayTimer = TheEngine::Instance()->GetSystemTimer().GetRunningTime() + ThePokerGame::Instance()->GeneralSetGameDelay(0.2f);
			}
		}
		mGoldenJokerHeldsIndex++;
	}
	else
		SetCompleted();
}