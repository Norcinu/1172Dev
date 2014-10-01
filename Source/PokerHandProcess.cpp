#include <ObjectHandler.h>
#include "GenericButtons.h"
#include "Game.h"
#include "PokerHandProcess.h"
#include "CollectProcess.h"
#include "PokerGame.h"

PokerHandProcess::PokerHandProcess()
{
	mCardIndex = 0;
	mCardDelayTimer = 0;

	if(TheEngine::Instance()->GetCurrentState()->GetName() == "DrawHand")
	{
		mCardIndex = mCardIndex;
	}
	else
	{
		Object2D* allCards = TheObjectHandler::Instance()->GetObject2D("Cards");
		allCards->GetInstance(0)->SetVisible(false);
		allCards->GetInstance(1)->SetVisible(false);
		allCards->GetInstance(2)->SetVisible(false);
		allCards->GetInstance(3)->SetVisible(false);
		allCards->GetInstance(4)->SetVisible(false);

		TheObjectHandler::Instance()->GetObject2D("Held01")->SetVisible(false);
		TheObjectHandler::Instance()->GetObject2D("Held02")->SetVisible(false);
		TheObjectHandler::Instance()->GetObject2D("Held03")->SetVisible(false);
		TheObjectHandler::Instance()->GetObject2D("Held04")->SetVisible(false);
		TheObjectHandler::Instance()->GetObject2D("Held05")->SetVisible(false);

		TheObjectHandler::Instance()->GetObject2D("HI01")->SetVisible(false);
		TheObjectHandler::Instance()->GetObject2D("HI02")->SetVisible(false);
		TheObjectHandler::Instance()->GetObject2D("HI03")->SetVisible(false);
		TheObjectHandler::Instance()->GetObject2D("HI04")->SetVisible(false);
		TheObjectHandler::Instance()->GetObject2D("HI05")->SetVisible(false);

		TheObjectHandler::Instance()->GetObject2D("LO01")->SetVisible(false);
		TheObjectHandler::Instance()->GetObject2D("LO02")->SetVisible(false);
		TheObjectHandler::Instance()->GetObject2D("LO03")->SetVisible(false);
		TheObjectHandler::Instance()->GetObject2D("LO04")->SetVisible(false);
		TheObjectHandler::Instance()->GetObject2D("LO05")->SetVisible(false);
	}
}

PokerHandProcess::~PokerHandProcess()
{
	
}

void PokerHandProcess::Complete()
{
	ThePokerGame::Instance()->PokerHandProcessComplete = true;		
}

void PokerHandProcess::Update()
{
	if (TheEngine::Instance()->GetSystemTimer().GetRunningTime() < mCardDelayTimer)
	{
		return;
	}

	if (mCardIndex < 5)
	{
		if (ThePokerGame::Instance()->PokerHand[mCardIndex].id)
		{
			if (!ThePokerGame::Instance()->PokerHand[mCardIndex].hold)
			{
				Object2D* allCards = TheObjectHandler::Instance()->GetObject2D("Cards");			
				TheAudioManager::Instance()->GetAudioSample("CFLIP")->Play();						
				allCards->GetInstance(mCardIndex)->SetVisible(true);
				allCards->GetInstance(mCardIndex)->SetCurrentSprite(ThePokerGame::Instance()->PokerHand[mCardIndex].id - 1);				
				mCardDelayTimer = TheEngine::Instance()->GetSystemTimer().GetRunningTime() + ThePokerGame::Instance()->GeneralSetGameDelay(ThePokerGame::Instance()->GetSoundDelay(CFLIP,50));
			}
		}
		mCardIndex++;
	}
	else
	{
		SetCompleted();
	}	
}