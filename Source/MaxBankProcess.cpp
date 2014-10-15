#include <ObjectHandler.h>
#include "GenericButtons.h"
#include "Game.h"
#include "MaxBankProcess.h"
#include "CollectProcess.h"
#include "PokerGame.h"

MaxBankProcess::MaxBankProcess()
{	
	THE_BUTTONS->SetButtonActivity(false, "FrontStart");
	THE_BUTTONS->SetButtonActivity(false, "Stake");
	THE_BUTTONS->SetButtonActivity(false, "TopStart");
	THE_BUTTONS->SetButtonActivity(false, "Menu");
	
	OBJECT_HANDLER->GetObject2D("RGMaxBank")->SetVisible(true);	
	THE_BUTTONS->SetButtonActivity(true, "Collect", LAMP_FLASH);
	if(GetCredits() < GetVariableValue(MAX_CREDITS))
	{
		THE_BUTTONS->SetButtonActivity(true, "Transfer", LAMP_FLASH);
	}
	else
	{		
		THE_BUTTONS->SetButtonActivity(false, "Transfer");
	}
}

MaxBankProcess::~MaxBankProcess()
{
	
}

void MaxBankProcess::Complete()
{
	OBJECT_HANDLER->GetObject2D("RGMaxBank")->SetVisible(false);
	THE_BUTTONS->SetButtonActivity(false, "Transfer");
	THE_BUTTONS->SetButtonActivity(false, "Collect");
}

void MaxBankProcess::Update()
{
	if (!POKER_GAME->GetTransferBanktoCreditTimeElaspsed())
		return;

	OBJECT_HANDLER->GetObject2D("RGMaxBank")->SetVisible(true);
	THE_BUTTONS->SetButtonActivity(true, "Collect", LAMP_FLASH);
	if(GetCredits() < GetVariableValue(MAX_CREDITS))
	{
		THE_BUTTONS->SetButtonActivity(true, "Transfer", LAMP_FLASH);		
	}
	else
	{	
		THE_BUTTONS->SetButtonActivity(false, "Transfer");
	}
	
	if(THE_BUTTONS->ButtonPressed("Transfer"))
	{
		POKER_GAME->SetTransferBankToCredits(true);		
	}
	else if(THE_BUTTONS->ButtonPressed("Collect"))
	{
		ENGINE->GetProcessManager()->AddProcessToQueue(new CollectProcess);
		SetCompleted();
		return;
	}

	THE_GAME->UpdateDigits();

	if(GetBankDeposit()<GetVariableValue(MAX_WIN_BANK))
	{
		SetCompleted();
	}
}