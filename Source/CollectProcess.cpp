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
	ThePokerGame::Instance()->SetActiveCollectBoxButton(false,COLLECT_COINS_PB);
	ThePokerGame::Instance()->SetActiveCollectBoxButton(false,COLLECT_TICKET_PB);
	OBJECT_HANDLER->GetObject2D("RGCollectOrPrintTicket")->SetVisible(false);	
}

void CollectProcess::Init()
{
	THE_BUTTONS->DisableHWButtons();	

	THE_BUTTONS->SetOSButtonActivity(false, "CollectButton");
	THE_BUTTONS->SetOSButtonActivity(false, "HoldInfoButton");
	THE_BUTTONS->SetOSButtonActivity(false, "Hold2Button");
	THE_BUTTONS->SetOSButtonActivity(false, "Hold3Button");
	THE_BUTTONS->SetOSButtonActivity(false, "Hold4Button");
	THE_BUTTONS->SetOSButtonActivity(false, "HoldTransferButton");
	THE_BUTTONS->SetOSButtonActivity(false, "DealStart1PndButton");
	THE_BUTTONS->SetOSButtonActivity(false, "DealStart2PndButton");
	
	if(GetCredits() + GetBankDeposit() < GetPrintPayThreshold() 
		&& GetCredits() + GetBankDeposit() >= GetMinPayoutValue())
	{	
		ThePokerGame::Instance()->SetActiveCollectBoxButton(true,COLLECT_COINS_PB);
		ThePokerGame::Instance()->SetActiveCollectBoxButton(true,COLLECT_TICKET_PB);
		OBJECT_HANDLER->GetObject2D("RGCollectOrPrintTicket")->SetVisible(true);
	}
	else if((GetCredits() + GetBankDeposit() < GetPrintPayThreshold() 
		&& GetCredits() + 	GetBankDeposit() < GetMinPayoutValue())
		|| GetCredits() + GetBankDeposit() >= GetPrintPayThreshold())
	{
		mPayoutConfirmed = true;
		mType = PRINTER;		
	}	
}

void CollectProcess::Update()
{	
	if(!mPayoutConfirmed)
	{
		if(ThePokerGame::Instance()->CollectBoxPb(COLLECT_COINS_PB))
		{
			mType = HOPPER;
			mPayoutConfirmed = true;
		}
		else if(ThePokerGame::Instance()->CollectBoxPb(COLLECT_TICKET_PB))
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
			TheEngine::Instance()->Shutdown();
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