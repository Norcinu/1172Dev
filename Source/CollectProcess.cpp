#include <ObjectHandler.h>
#include "Game.h"
#include "Defines.h"
#include "CollectProcess.h"
#include "PokerGame.h"

CollectProcess::CollectProcess()
{
	mPayoutConfirmed = false;
	mPayoutStarted = false;
	mType = HOPPER;
	mPayoutState = ERR_NOCOLLECT;
}

CollectProcess::~CollectProcess()
{

}

void CollectProcess::Complete()
{
	POKER_GAME->SetActiveCollectBoxButton(false,COLLECT_COINS_PB);
	POKER_GAME->SetActiveCollectBoxButton(false,COLLECT_TICKET_PB);
	OBJECT_HANDLER->GetObject2D("RGCollectOrPrintTicket")->SetVisible(false);	
}

void CollectProcess::Init()
{
	THE_BUTTONS->DisableHWButtons();
	
	if (GetCredits() + GetBankDeposit() < GetPrintPayThreshold() && 
		GetCredits() + GetBankDeposit() >= GetMinPayoutValue())
	{	
		POKER_GAME->SetActiveCollectBoxButton(true,COLLECT_COINS_PB);
		POKER_GAME->SetActiveCollectBoxButton(true,COLLECT_TICKET_PB);
		OBJECT_HANDLER->GetObject2D("RGCollectOrPrintTicket")->SetVisible(true);
	}
	else if((GetCredits() + GetBankDeposit() < GetPrintPayThreshold() && 
			 GetCredits() +	GetBankDeposit() < GetMinPayoutValue()) || 
			 GetCredits() + GetBankDeposit() >= GetPrintPayThreshold())
	{
		mPayoutConfirmed = true;
		mType = PRINTER;		
	}	
}

void CollectProcess::Update()
{	
	if(!mPayoutConfirmed)
	{
		if(POKER_GAME->CollectBoxPb(COLLECT_COINS_PB))
		{
			mType = HOPPER;
			mPayoutConfirmed = true;
		}
		else if(POKER_GAME->CollectBoxPb(COLLECT_TICKET_PB))
		{
			mType = PRINTER;
			mPayoutConfirmed = true;
		}
	}
	else
	{
		if(!mPayoutStarted)
		{
			mPayoutState = StartPayoutProcess(0, false, MODEL_NUMBER, 0, mType);  //NB: Innocore always stage payouts
			mPayoutStarted = true;
		}
		else
		{
			if(Payout())
			{
				SetCompleted();
			}
		}
	}

	if(GetCurrentError())
	{
		SetCompleted();
	}
}

bool CollectProcess::Payout()
{
	if(mPayoutState == ERR_NOCOLLECT) //set critical errors
	{
		//SOUND
	}
	else if (mPayoutState == HOPPER_COLL)
	{
		if(GetRequestHopperPayout())
		{
			
		}
		else
		{
			return true;
		}
	}
	else if(mPayoutState == HANDPAY_COLL )
	{	
		OBJECT_HANDLER->GetObject2D("RGHandPay")->SetVisible(true);
		SetWarningError(ERR_PRINTER_NO_PAPER);
		if(GetSwitchStatus(REFILL_KEY))
		{			
			ENGINE->Shutdown();
			return true;
		}
		
	}
	else if(mPayoutState == PRINTER_COLL)
	{
		if(GetTicketPrinting())
		{
			
		}
		else
		{
			return true;
		}
	}
	else if (mPayoutState == NOCOLLECT)	
	{
		//resets bank and credits!!
		return true;
	}

	return false;
}