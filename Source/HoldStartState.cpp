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
	
	TheButtons::Instance()->SetButtonActivity(false, "Start");
	TheButtons::Instance()->SetButtonActivity(false, "OnePound");
	TheButtons::Instance()->SetButtonActivity(false, "TwoPound");
	TheButtons::Instance()->SetButtonActivity(false, "Menu");
	TheButtons::Instance()->SetButtonActivity(false, "Collect");
	TheButtons::Instance()->SetButtonActivity(false, "Transfer");

	TheButtons::Instance()->SetOSButtonActivity(false, "CollectButton");
	TheButtons::Instance()->SetOSButtonActivity(false, "HoldInfoButton");
	TheButtons::Instance()->SetOSButtonActivity(false, "Hold2Button");
	TheButtons::Instance()->SetOSButtonActivity(false, "Hold3Button");
	TheButtons::Instance()->SetOSButtonActivity(false, "Hold4Button");
	TheButtons::Instance()->SetOSButtonActivity(false, "HoldTransferButton");
	TheButtons::Instance()->SetOSButtonActivity(false, "DealStart1PndButton");
	TheButtons::Instance()->SetOSButtonActivity(false, "DealStart2PndButton");

	ThePokerGame::Instance()->SetActiveCardButton(false,0);
	ThePokerGame::Instance()->SetActiveCardButton(false,1);
	ThePokerGame::Instance()->SetActiveCardButton(false,2);
	ThePokerGame::Instance()->SetActiveCardButton(false,3);
	ThePokerGame::Instance()->SetActiveCardButton(false,4);

	if(!TheGame::Instance()->GetAutoplay())	
		TheButtons::Instance()->SetButtonActivity(false, "AutoPlay");
	

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

		TheButtons::Instance()->HoldStartButtons();
		ThePokerGame::Instance()->HoldCardButtons();
		ThePokerGame::Instance()->UpdatePokerHelds();
		ThePokerGame::Instance()->SetHoldStartLamps();
			
		bool startGame = false;

#ifdef SOAK_BUILD
		startGame = true;
#endif
		if(TheGame::Instance()->GetStake() == MINIMUM_BET)
		{			
			if(TheButtons::Instance()->ButtonPressed("OnePound") ||
				TheButtons::Instance()->OSButtonPressed("DealStart1PndButton"))							
			{
				startGame = true;
			}				
		}
		else
		{
			if(TheButtons::Instance()->ButtonPressed("TwoPound") ||
				TheButtons::Instance()->OSButtonPressed("DealStart2PndButton"))
			{
				startGame = true;
			}
		}

		if((TheButtons::Instance()->ButtonPressed("Start") || TheGame::Instance()->GetAutoplay() || 
			TheGame::Instance()->GetAutoplay() || startGame))
		{
			TheEngine::Instance()->StateTransition("DrawHand");
		}
		else if((TheButtons::Instance()->ButtonPressed("AutoPlay") || TheButtons::Instance()->OSButtonPressed("AutoplayButton")) && 
				    !TheGame::Instance()->GetAutoplay())
		{
			TheAudioManager::Instance()->GetAudioSample("DROP")->Play();
			TheGame::Instance()->SetAutoplay(true);
		}
	}
	
}
