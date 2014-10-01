#include <ObjectHandler.h>
#include "GenericButtons.h"
#include "Game.h"
#include "MaxBankProcess.h"
#include "CollectProcess.h"
#include "PokerGame.h"

MaxBankProcess::MaxBankProcess()
{	
	GET_BUTTONS->SetButtonActivity(false, "FrontStart");
	GET_BUTTONS->SetButtonActivity(false, "Stake");
	GET_BUTTONS->SetButtonActivity(false, "TopStart");
	GET_BUTTONS->SetButtonActivity(false, "Menu");
	
	GET_BUTTONS->SetOSButtonActivity(false, "HoldInfoButton");
	GET_BUTTONS->SetOSButtonActivity(false, "Hold2Button");
	GET_BUTTONS->SetOSButtonActivity(false, "Hold3Button");
	GET_BUTTONS->SetOSButtonActivity(false, "Hold4Button");	
	GET_BUTTONS->SetOSButtonActivity(false, "DealStart1PndButton");
	GET_BUTTONS->SetOSButtonActivity(false, "DealStart2PndButton");

	TheObjectHandler::Instance()->GetObject2D("RGMaxBank")->SetVisible(true);	
	GET_BUTTONS->SetButtonActivity(true, "Collect", LAMP_FLASH);
	GET_BUTTONS->SetOSButtonActivity(true, "CollectButton",LAMP_FLASH);
	if(GetCredits() < GetVariableValue(MAX_CREDITS))
	{
		GET_BUTTONS->SetButtonActivity(true, "Transfer", LAMP_FLASH);
		GET_BUTTONS->SetOSButtonActivity(true, "HoldTransferButton", LAMP_FLASH);
	}
	else
	{		
		GET_BUTTONS->SetButtonActivity(false, "Transfer");
		GET_BUTTONS->SetOSButtonActivity(false, "HoldTransferButton");
	}
}

MaxBankProcess::~MaxBankProcess()
{
	
}

void MaxBankProcess::Complete()
{
	TheObjectHandler::Instance()->GetObject2D("RGMaxBank")->SetVisible(false);	
	GET_BUTTONS->SetButtonActivity(false, "Transfer");
	GET_BUTTONS->SetButtonActivity(false, "Collect");	
	GET_BUTTONS->SetOSButtonActivity(false, "HoldTransferButton");
	GET_BUTTONS->SetOSButtonActivity(false, "CollectButton");
}

void MaxBankProcess::Update()
{
	if (!ThePokerGame::Instance()->GetTransferBanktoCreditTimeElaspsed())
		return;

	TheObjectHandler::Instance()->GetObject2D("RGMaxBank")->SetVisible(true);
	GET_BUTTONS->SetButtonActivity(true, "Collect", LAMP_FLASH);
	GET_BUTTONS->SetOSButtonActivity(true, "CollectButton",LAMP_FLASH);
	if(GetCredits() < GetVariableValue(MAX_CREDITS))
	{
		GET_BUTTONS->SetButtonActivity(true, "Transfer", LAMP_FLASH);		
		GET_BUTTONS->SetOSButtonActivity(true, "HoldTransferButton", LAMP_FLASH);
	}
	else
	{	
		GET_BUTTONS->SetButtonActivity(false, "Transfer");
		GET_BUTTONS->SetOSButtonActivity(false, "HoldTransferButton");
	}
	
	if(GET_BUTTONS->ButtonPressed("Transfer") ||
	   GET_BUTTONS->OSButtonPressed("HoldTransferButton"))
	{
		ThePokerGame::Instance()->SetTransferBankToCredits(true);		
	}
	else if(GET_BUTTONS->ButtonPressed("Collect") ||
		    GET_BUTTONS->OSButtonPressed("CollectButton"))
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