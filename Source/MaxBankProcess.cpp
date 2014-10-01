#include <ObjectHandler.h>
#include "GenericButtons.h"
#include "Game.h"
#include "MaxBankProcess.h"
#include "CollectProcess.h"
#include "PokerGame.h"

MaxBankProcess::MaxBankProcess()
{	
	TheButtons::Instance()->SetButtonActivity(false, "Start");
	TheButtons::Instance()->SetButtonActivity(false, "OnePound");
	TheButtons::Instance()->SetButtonActivity(false, "TwoPound");
	TheButtons::Instance()->SetButtonActivity(false, "Menu");
	
	TheButtons::Instance()->SetOSButtonActivity(false, "HoldInfoButton");
	TheButtons::Instance()->SetOSButtonActivity(false, "Hold2Button");
	TheButtons::Instance()->SetOSButtonActivity(false, "Hold3Button");
	TheButtons::Instance()->SetOSButtonActivity(false, "Hold4Button");	
	TheButtons::Instance()->SetOSButtonActivity(false, "DealStart1PndButton");
	TheButtons::Instance()->SetOSButtonActivity(false, "DealStart2PndButton");

	TheObjectHandler::Instance()->GetObject2D("RGMaxBank")->SetVisible(true);	
	TheButtons::Instance()->SetButtonActivity(true, "Collect", LAMP_FLASH);
	TheButtons::Instance()->SetOSButtonActivity(true, "CollectButton",LAMP_FLASH);
	if(GetCredits() < GetVariableValue(MAX_CREDITS))
	{
		TheButtons::Instance()->SetButtonActivity(true, "Transfer", LAMP_FLASH);
		TheButtons::Instance()->SetOSButtonActivity(true, "HoldTransferButton", LAMP_FLASH);
	}
	else
	{		
		TheButtons::Instance()->SetButtonActivity(false, "Transfer");
		TheButtons::Instance()->SetOSButtonActivity(false, "HoldTransferButton");
	}
}

MaxBankProcess::~MaxBankProcess()
{
	
}

void MaxBankProcess::Complete()
{
	TheObjectHandler::Instance()->GetObject2D("RGMaxBank")->SetVisible(false);	
	TheButtons::Instance()->SetButtonActivity(false, "Transfer");
	TheButtons::Instance()->SetButtonActivity(false, "Collect");	
	TheButtons::Instance()->SetOSButtonActivity(false, "HoldTransferButton");
	TheButtons::Instance()->SetOSButtonActivity(false, "CollectButton");
}

void MaxBankProcess::Update()
{
	if (!ThePokerGame::Instance()->GetTransferBanktoCreditTimeElaspsed())
		return;

	TheObjectHandler::Instance()->GetObject2D("RGMaxBank")->SetVisible(true);
	TheButtons::Instance()->SetButtonActivity(true, "Collect", LAMP_FLASH);
	TheButtons::Instance()->SetOSButtonActivity(true, "CollectButton",LAMP_FLASH);
	if(GetCredits() < GetVariableValue(MAX_CREDITS))
	{
		TheButtons::Instance()->SetButtonActivity(true, "Transfer", LAMP_FLASH);		
		TheButtons::Instance()->SetOSButtonActivity(true, "HoldTransferButton", LAMP_FLASH);
	}
	else
	{	
		TheButtons::Instance()->SetButtonActivity(false, "Transfer");
		TheButtons::Instance()->SetOSButtonActivity(false, "HoldTransferButton");
	}
	
	if(TheButtons::Instance()->ButtonPressed("Transfer") ||
	   TheButtons::Instance()->OSButtonPressed("HoldTransferButton"))
	{
		ThePokerGame::Instance()->SetTransferBankToCredits(true);		
	}
	else if(TheButtons::Instance()->ButtonPressed("Collect") ||
		    TheButtons::Instance()->OSButtonPressed("CollectButton"))
	{
		TheEngine::Instance()->GetProcessManager()->AddProcessToQueue(new CollectProcess);
		SetCompleted();
		return;
	}

	TheGame::Instance()->UpdateDigits();

	if(GetBankDeposit()<GetVariableValue(MAX_WIN_BANK))
	{
		SetCompleted();
	}
}