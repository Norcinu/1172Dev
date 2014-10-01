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

static bool Registered = TheEngine::Instance()->AddState("HoldStart", new HoldStartState());

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
	TheEngine::Instance()->GetProcessManager()->AddProcessToQueue(new AutoHeldsProcess);
}

void HoldStartState::Exit()
{
	ThePokerGame::Instance()->InitializeCardButtonArray();

	ThePokerGame::Instance()->TagHeldCards();
	
	GET_BUTTONS->SetButtonActivity(false, "FrontStart");
	GET_BUTTONS->SetButtonActivity(false, "Stake");
	GET_BUTTONS->SetButtonActivity(false, "TopStart");
	GET_BUTTONS->SetButtonActivity(false, "Menu");
	GET_BUTTONS->SetButtonActivity(false, "Collect");
	GET_BUTTONS->SetButtonActivity(false, "Transfer");

	GET_BUTTONS->SetOSButtonActivity(false, "CollectButton");
	GET_BUTTONS->SetOSButtonActivity(false, "HoldInfoButton");
	GET_BUTTONS->SetOSButtonActivity(false, "Hold2Button");
	GET_BUTTONS->SetOSButtonActivity(false, "Hold3Button");
	GET_BUTTONS->SetOSButtonActivity(false, "Hold4Button");
	GET_BUTTONS->SetOSButtonActivity(false, "HoldTransferButton");
	GET_BUTTONS->SetOSButtonActivity(false, "DealStart1PndButton");
	GET_BUTTONS->SetOSButtonActivity(false, "DealStart2PndButton");

	ThePokerGame::Instance()->SetActiveCardButton(false,0);
	ThePokerGame::Instance()->SetActiveCardButton(false,1);
	ThePokerGame::Instance()->SetActiveCardButton(false,2);
	ThePokerGame::Instance()->SetActiveCardButton(false,3);
	ThePokerGame::Instance()->SetActiveCardButton(false,4);

	if(!TheGame::Instance()->GetAutoplay())	
		GET_BUTTONS->SetButtonActivity(false, "AutoPlay");
	

	TheObjectHandler::Instance()->GetObject2D("HoldStartMsg")->SetVisible(false);
}

#include <sstream>

void HoldStartState::Update()
{		
	PROFILE(__FUNCTION__);	

	if(TheEngine::Instance()->GetProcessManager()->GetNumQueueProcesses())
	{
		return;
	}

	if(GetCurrentError())
	{
		return;
	}
	
	if(!global_quit)
	{
		TheObjectHandler::Instance()->GetObject2D("HoldStartMsg")->SetVisible(true);

		GET_BUTTONS->HoldStartButtons();
		ThePokerGame::Instance()->HoldCardButtons();
		ThePokerGame::Instance()->UpdatePokerHelds();
		ThePokerGame::Instance()->SetHoldStartLamps();
			
		bool startGame = false;
		
#ifdef SOAK_BUILD
		startGame = true;
#endif
		if (GET_BUTTONS->ButtonPressed("TopStart") || GET_BUTTONS->ButtonPressed("FrontStart") ||
			GET_BUTTONS->OSButtonPressed("DealStart1PndButton") || GET_BUTTONS->OSButtonPressed("DealStart2PndButton"))
			startGame = true;
		
		if ((GET_BUTTONS->ButtonPressed("FrontStart") || 
			 GET_BUTTONS->ButtonPressed("TopStart") ||
			 TheGame::Instance()->GetAutoplay() || 
			 TheGame::Instance()->GetAutoplay() || 
			 startGame))
		{
			TheEngine::Instance()->StateTransition("DrawHand");
		}
		else if((GET_BUTTONS->ButtonPressed("AutoPlay") || GET_BUTTONS->OSButtonPressed("AutoplayButton")) && 
				    !TheGame::Instance()->GetAutoplay())
		{
			TheAudioManager::Instance()->GetAudioSample("DROP")->Play();
			TheGame::Instance()->SetAutoplay(true);
		}
	}
}
