
#include "Game.h"
#include "Defines.h"
#include "bo.h"

bool Game::CollectFromHopper(unsigned char type)
{	
	bool collectComplete = false;
	char PayoutState = StartPayoutProcess(0, false, MODEL_NUMBER, 0, type);  //NB: Innocore always stage payouts

	if(PayoutState == ERR_NOCOLLECT) //set critical errors
	{
		//SOUND
	}
	else if (PayoutState == HOPPER_COLL)
	{
		if(GetRequestHopperPayout())
		{
			OBJECT_HANDLER->GetText("ErrorText")->SetMessage("Cashing Out");
			OBJECT_HANDLER->GetText("ErrorText")->SetVisible(true);
		}
		else
		{
			return true;
		}
	}
	else if(PayoutState == HANDPAY_COLL )
	{
		OBJECT_HANDLER->GetText("ErrorText")->SetMessage("Hand Pay Required. Please Call Attendant");
		OBJECT_HANDLER->GetText("ErrorText")->SetVisible(true);
		SetWarningError(ERR_PRINTER_NO_PAPER);
		if(GetSwitchStatus(REFILL_KEY))
		{			
			ENGINE->Shutdown();
			return true;
		}
		
	}
	else if(PayoutState == PRINTER_COLL)
	{
		if(GetTicketPrinting())
		{
			OBJECT_HANDLER->GetText("ErrorText")->SetMessage("Printing Ticket");
		}
		else
		{
			return true;
		}
	}
	else if (PayoutState == NOCOLLECT)
	{
		//resets bank and credits!!
		return true;
	}

	return false;
}