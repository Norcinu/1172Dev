#include <ObjectHandler.h>
#include "GenericButtons.h"
#include "Game.h"
#include "PayTransferProcess.h"
#include "CollectProcess.h"
#include "PokerGame.h"

PayTransferProcess::PayTransferProcess()
{
	mPayTransferDelayTimer = 0;
	ThePokerGame::Instance()->FinalWinValue = 0;
	mAmountToDeduct = ThePokerGame::Instance()->Pay/10;
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
		 					
	if (ThePokerGame::Instance()->Pay)								  
	{						
		TheAudioManager::Instance()->GetAudioSample("WIN10")->Play();
		if (ThePokerGame::Instance()->Pay < mAmountToDeduct) mAmountToDeduct = ThePokerGame::Instance()->Pay;		
		ThePokerGame::Instance()->Pay -= mAmountToDeduct;
        ThePokerGame::Instance()->FinalWinValue += mAmountToDeduct;
		AddWinToBank(mAmountToDeduct,MODEL_NUMBER);	
		THE_GAME->UpdateDigits();
		mPayTransferDelayTimer = ENGINE->GetSystemTimer().GetRunningTime() + ThePokerGame::Instance()->GeneralSetGameDelay(0.1f);
	}
	else	
		SetCompleted();
}