#include <ObjectHandler.h>
#include <TextureManager.h>
#include "GenericButtons.h"
#include "CollectProcess.h"
#include "Defines.h"
#include "Game.h"
#include "HoldStartState.h"
#include "MaxBankProcess.h"
#include "CollectProcess.h"
#include "PokerGame.h"
#include "AutoHeldsProcess.h"

#include "NMI_log.h"
#include "JackpotProcess.h"

static bool Registered = ENGINE->AddState("HoldStart", new HoldStartState());

unsigned char global_quit = 0;

HoldStartState::HoldStartState()
{
	
}

HoldStartState::~HoldStartState()
{

}

void HoldStartState::Enter()
{
	ThePokerGame::Instance()->InitializeCardButtonArray();
	
	ThePokerGame::Instance()->SetActiveCardButton(true,0);
	ThePokerGame::Instance()->SetActiveCardButton(true,1);
	ThePokerGame::Instance()->SetActiveCardButton(true,2);
	ThePokerGame::Instance()->SetActiveCardButton(true,3);
	ThePokerGame::Instance()->SetActiveCardButton(true,4);
	
	ThePokerGame::Instance()->HoldCalc(ThePokerGame::Instance()->DealHand);

	ENGINE->GetProcessManager()->AddProcessToQueue(new AutoHeldsProcess);
}

void HoldStartState::Exit()
{
	ThePokerGame::Instance()->InitializeCardButtonArray();

	ThePokerGame::Instance()->TagHeldCards();
	
	THE_BUTTONS->SetButtonActivity(false, "FrontStart");
	THE_BUTTONS->SetButtonActivity(false, "Stake");
	THE_BUTTONS->SetButtonActivity(false, "TopStart");
	THE_BUTTONS->SetButtonActivity(false, "Menu");
	THE_BUTTONS->SetButtonActivity(false, "Collect");
	THE_BUTTONS->SetButtonActivity(false, "Transfer");

	THE_BUTTONS->SetOSButtonActivity(false, "CollectButton");
	THE_BUTTONS->SetOSButtonActivity(false, "HoldInfoButton");
	THE_BUTTONS->SetOSButtonActivity(false, "Hold2Button");
	THE_BUTTONS->SetOSButtonActivity(false, "Hold3Button");
	THE_BUTTONS->SetOSButtonActivity(false, "Hold4Button");
	THE_BUTTONS->SetOSButtonActivity(false, "HoldTransferButton");
	THE_BUTTONS->SetOSButtonActivity(false, "DealStart1PndButton");
	THE_BUTTONS->SetOSButtonActivity(false, "DealStart2PndButton");
	
	ThePokerGame::Instance()->SetActiveCardButton(false,0);
	ThePokerGame::Instance()->SetActiveCardButton(false,1);
	ThePokerGame::Instance()->SetActiveCardButton(false,2);
	ThePokerGame::Instance()->SetActiveCardButton(false,3);
	ThePokerGame::Instance()->SetActiveCardButton(false,4);

	if(!THE_GAME->GetAutoplay())	
		THE_BUTTONS->SetButtonActivity(false, "AutoPlay");
	
	OBJECT_HANDLER->GetObject2D("GraphicalButton07")->SetVisible(false);
	OBJECT_HANDLER->GetObject2D("GraphicalButton08")->SetVisible(false);
	OBJECT_HANDLER->GetObject2D("Legend1DealDrawLit")->SetVisible(false);
	OBJECT_HANDLER->GetObject2D("Legend1DealDrawNlit")->SetVisible(false);
	OBJECT_HANDLER->GetObject2D("Legend2DealDrawLit")->SetVisible(false);
	OBJECT_HANDLER->GetObject2D("Legend2DealDrawNlit")->SetVisible(false);
	OBJECT_HANDLER->GetObject2D("HoldStartMsg")->SetVisible(false);
}

#include <sstream>

void HoldStartState::Update()
{		
	PROFILE(__FUNCTION__);	

	if(ENGINE->GetProcessManager()->GetNumQueueProcesses())
	{
		return;
	}

	if(GetCurrentError())
	{
		return;
	}
	
	if(!global_quit)
	{
		OBJECT_HANDLER->GetObject2D("HoldStartMsg")->SetVisible(true);

		THE_BUTTONS->HoldStartButtons();
		ThePokerGame::Instance()->HoldCardButtons();
		ThePokerGame::Instance()->UpdatePokerHelds();
		ThePokerGame::Instance()->SetHoldStartLamps();
			
		bool startGame = false;
		bool stakeChange = false;
		auto stake = 0;

#ifdef SOAK_BUILD
		startGame = true;
#endif
		if (THE_BUTTONS->ButtonPressed("TopStart") || THE_BUTTONS->ButtonPressed("FrontStart"))
			startGame = true;
		
		if ((THE_BUTTONS->ButtonPressed("FrontStart") || 
			 THE_BUTTONS->ButtonPressed("TopStart") ||
			 THE_GAME->GetAutoplay() || 
			 THE_GAME->GetAutoplay() || 
			 startGame))
		{
			ENGINE->StateTransition("DrawHand");
		}
		else if((THE_BUTTONS->ButtonPressed("AutoPlay") || THE_BUTTONS->OSButtonPressed("AutoplayButton")) && 
				    !THE_GAME->GetAutoplay())
		{
			TheAudioManager::Instance()->GetAudioSample("DROP")->Play();
			THE_GAME->SetAutoplay(true);
		}
	}
}
