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
	
	THE_BUTTONS->SetOSButtonActivity(false, "HoldInfoButton");
	THE_BUTTONS->SetOSButtonActivity(false, "Hold2Button");
	THE_BUTTONS->SetOSButtonActivity(false, "Hold3Button");
	THE_BUTTONS->SetOSButtonActivity(false, "Hold4Button");	
	THE_BUTTONS->SetOSButtonActivity(false, "DealStart1PndButton");
	THE_BUTTONS->SetOSButtonActivity(false, "DealStart2PndButton");

	OBJECT_HANDLER->GetObject2D("RGMaxBank")->SetVisible(true);	
	THE_BUTTONS->SetButtonActivity(true, "Collect", LAMP_FLASH);
	//THE_BUTTONS->SetOSButtonActivity(true, "Hold1Button",LAMP_FLASH);
	if(GetCredits() < GetVariableValue(MAX_CREDITS))
	{
		THE_BUTTONS->SetButtonActivity(true, "Transfer", LAMP_FLASH);
		//THE_BUTTONS->SetOSButtonActivity(true, "Hold5Button", LAMP_FLASH);
	}
	else
	{		
		THE_BUTTONS->SetButtonActivity(false, "Transfer");
		//THE_BUTTONS->SetOSButtonActivity(false, "Hold5Button");
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
	//THE_BUTTONS->SetOSButtonActivity(false, "Hold5Button");
	//THE_BUTTONS->SetOSButtonActivity(false, "Hold1Button");
}

void MaxBankProcess::Update()
{
	if (!ThePokerGame::Instance()->GetTransferBanktoCreditTimeElaspsed())
		return;

	OBJECT_HANDLER->GetObject2D("RGMaxBank")->SetVisible(true);
	THE_BUTTONS->SetButtonActivity(true, "Collect", LAMP_FLASH);
	//THE_BUTTONS->SetOSButtonActivity(true, "Hold1Button",LAMP_FLASH);
	if(GetCredits() < GetVariableValue(MAX_CREDITS))
	{
		THE_BUTTONS->SetButtonActivity(true, "Transfer", LAMP_FLASH);		
		//THE_BUTTONS->SetOSButtonActivity(true, "Hold5Button", LAMP_FLASH);
	}
	else
	{	
		THE_BUTTONS->SetButtonActivity(false, "Transfer");
		//THE_BUTTONS->SetOSButtonActivity(false, "Hold5Button");
	}
	
	if(THE_BUTTONS->ButtonPressed("Transfer"))/* ||
	   THE_BUTTONS->OSButtonPressed("Hold5Button"))*/
	{
		ThePokerGame::Instance()->SetTransferBankToCredits(true);		
	}
	else if(THE_BUTTONS->ButtonPressed("Collect"))// ||
		    //THE_BUTTONS->OSButtonPressed("Hold1Button"))
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