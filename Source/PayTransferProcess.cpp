#include <ObjectHandler.h>
#include "GenericButtons.h"
#include "Game.h"
#include "PayTransferProcess.h"
#include "CollectProcess.h"
#include "PokerGame.h"

PayTransferProcess::PayTransferProcess()
{
	mPayTransferDelayTimer = 0;
	POKER_GAME->FinalWinValue = 0;
	mAmountToDeduct = POKER_GAME->Pay/10;
}

PayTransferProcess::~PayTransferProcess()
{
	
}

void PayTransferProcess::Complete()
{
	mPayTransferDelayTimer = 0;
}

void PayTransferProcess::Update()
{

	if (ENGINE->GetSystemTimer().GetRunningTime() < mPayTransferDelayTimer)
	{
		return;
	}
		 					
	if (POKER_GAME->Pay)								  
	{						
		TheAudioManager::Instance()->GetAudioSample("WIN10")->Play();
		if (POKER_GAME->Pay < mAmountToDeduct) mAmountToDeduct = POKER_GAME->Pay;		
		POKER_GAME->Pay -= mAmountToDeduct;
        POKER_GAME->FinalWinValue += mAmountToDeduct;
		AddWinToBank(mAmountToDeduct,MODEL_NUMBER);	
		THE_GAME->UpdateDigits();
		mPayTransferDelayTimer = ENGINE->GetSystemTimer().GetRunningTime() + POKER_GAME->GeneralSetGameDelay(0.1f);
	}
	else	
		SetCompleted();
}