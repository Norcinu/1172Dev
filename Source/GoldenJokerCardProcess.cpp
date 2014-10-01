#include <ObjectHandler.h>
#include "GenericButtons.h"
#include "Game.h"
#include "GoldenJokerCardProcess.h"
#include "CollectProcess.h"
#include "PokerGame.h"

GoldenJokerCardProcess::GoldenJokerCardProcess()
{
	mCardIndex = 0;
	mCardDelayTimer = 0;	
}

GoldenJokerCardProcess::~GoldenJokerCardProcess()
{
	
}

void GoldenJokerCardProcess::Complete()
{
	
}

void GoldenJokerCardProcess::Update()
{
	if (TheEngine::Instance()->GetSystemTimer().GetRunningTime() < mCardDelayTimer)
	{
		return;
	}

	if (mCardIndex < 5)
	{
		if (ThePokerGame::Instance()->PokerHand[mCardIndex].id == JOKER_CARD)
		{			
			Object2D* allCards = TheObjectHandler::Instance()->GetObject2D("Cards");			
			TheAudioManager::Instance()->GetAudioSample("CFLIP")->Play();						
			allCards->GetInstance(mCardIndex)->SetVisible(true);
			allCards->GetInstance(mCardIndex)->SetCurrentSprite(ThePokerGame::Instance()->PokerHand[mCardIndex].id - 1);											
			SetCompleted();
		}
		else
			mCardDelayTimer = TheEngine::Instance()->GetSystemTimer().GetRunningTime() + ThePokerGame::Instance()->GeneralSetGameDelay(0.1f);
		mCardIndex++;
	}		
}