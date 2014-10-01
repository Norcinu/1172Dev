#include <ObjectHandler.h>
#include <TextureManager.h>
#include "GenericButtons.h"
#include "Defines.h"
#include "Game.h"
#include "GoldenJokerState.h"
#include "GoldenJokerHeldsProcess.h"
#include "GoldenJokerCardProcess.h"
#include "PokerGame.h"

static bool Registered = TheEngine::Instance()->AddState("GoldenJoker", new GoldenJokerState());

extern unsigned char global_quit;

GoldenJokerState::GoldenJokerState()
{

}

GoldenJokerState::~GoldenJokerState()
{

}

void GoldenJokerState::Enter()
{
	ThePokerGame::Instance()->GoldenJokerStage = 1;
	ThePokerGame::Instance()->GoldenJokerHeldProcessEnd = false;

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
}

void GoldenJokerState::Exit()
{	
	TheButtons::Instance()->SetOSButtonActivity(false, "HoldInfoButton");
	TheButtons::Instance()->SetOSButtonActivity(false, "Hold2Button");
	TheButtons::Instance()->SetOSButtonActivity(false, "Hold3Button");
	TheButtons::Instance()->SetOSButtonActivity(false, "Hold4Button");
	TheButtons::Instance()->SetOSButtonActivity(false, "HoldTransferButton");

	ThePokerGame::Instance()->SetActiveCardButton(false,0);
	ThePokerGame::Instance()->SetActiveCardButton(false,1);
	ThePokerGame::Instance()->SetActiveCardButton(false,2);
	ThePokerGame::Instance()->SetActiveCardButton(false,3);
	ThePokerGame::Instance()->SetActiveCardButton(false,4);

	ThePokerGame::Instance()->PokerGameWin = ThePokerGame::Instance()->Pay;
}

#include <sstream>

void GoldenJokerState::Update()
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
		ThePokerGame::Instance()->GoldenJokerUpdateDrawHand();

		if (ThePokerGame::Instance()->GoldenJokerStage == 3)
		{
			if (!ThePokerGame::Instance()->GoldenJokerHeldProcessEnd)
				TheEngine::Instance()->GetProcessManager()->AddProcessToQueue(new GoldenJokerHeldsProcess);
			else
				TheObjectHandler::Instance()->GetObject2D("GoldenJokerMsg")->SetVisible(true);	
		}

		if (ThePokerGame::Instance()->GoldenJokerStage == 4)
			TheEngine::Instance()->GetProcessManager()->AddProcessToQueue(new GoldenJokerCardProcess);

		if (ThePokerGame::Instance()->GoldenJokerStage == 5)
		{
			TheObjectHandler::Instance()->GetObject2D("GoldenJokerMsg")->SetVisible(false);
			TheEngine::Instance()->StateTransition("HiloGamble");
		}
	}
}
