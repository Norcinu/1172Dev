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
	POKER_GAME->GoldenJokerHeldProcessEnd = false;
}

GoldenJokerHeldsProcess::~GoldenJokerHeldsProcess()
{
	
}

void GoldenJokerHeldsProcess::Complete()
{
	POKER_GAME->GoldenJokerHeldProcessEnd = true;
}

void GoldenJokerHeldsProcess::Update()
{
	if (ENGINE->GetSystemTimer().GetRunningTime() < mGoldenJokerHeldsDelayTimer)
	{
		return;
	}

	if (mGoldenJokerHeldsIndex < 5)
	{		
		unsigned int HeldId = OBJECT_HANDLER->GetObject2D("Held01")->GetID()+mGoldenJokerHeldsIndex;
		if (POKER_GAME->PokerHand[mGoldenJokerHeldsIndex].hold)		
		{
			TheAudioManager::Instance()->GetAudioSample("CLICK")->Play();
			OBJECT_HANDLER->GetObject2D(HeldId)->SetVisible(true);												
			mGoldenJokerHeldsDelayTimer = ENGINE->GetSystemTimer().GetRunningTime() + POKER_GAME->GeneralSetGameDelay(0.2f);
		}
		else
		{
			if (OBJECT_HANDLER->GetObject2D(HeldId)->GetVisible())
			{
				TheAudioManager::Instance()->GetAudioSample("CLICK")->Play();
				OBJECT_HANDLER->GetObject2D(HeldId)->SetVisible(false);													
				mGoldenJokerHeldsDelayTimer = ENGINE->GetSystemTimer().GetRunningTime() + POKER_GAME->GeneralSetGameDelay(0.2f);
			}
		}
		mGoldenJokerHeldsIndex++;
	}
	else
		SetCompleted();
}