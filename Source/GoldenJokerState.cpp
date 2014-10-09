#include <ObjectHandler.h>
#include <TextureManager.h>
#include "GenericButtons.h"
#include "Defines.h"
#include "Game.h"
#include "GoldenJokerState.h"
#include "GoldenJokerHeldsProcess.h"
#include "GoldenJokerCardProcess.h"
#include "PokerGame.h"

static bool Registered = ENGINE->AddState("GoldenJoker", new GoldenJokerState());

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

	THE_BUTTONS->SetButtonActivity(false, "FrontStart");
	THE_BUTTONS->SetButtonActivity(false, "Stake");
	THE_BUTTONS->SetButtonActivity(false, "TopStart");
	THE_BUTTONS->SetButtonActivity(false, "Menu");
	THE_BUTTONS->SetButtonActivity(false, "Collect");
	THE_BUTTONS->SetButtonActivity(false, "Transfer");

	THE_BUTTONS->SetOSButtonActivity(false, "Hold1Button");
	THE_BUTTONS->SetOSButtonActivity(false, "HoldInfoButton");
	THE_BUTTONS->SetOSButtonActivity(false, "Hold2Button");
	THE_BUTTONS->SetOSButtonActivity(false, "Hold3Button");
	THE_BUTTONS->SetOSButtonActivity(false, "Hold4Button");
	THE_BUTTONS->SetOSButtonActivity(false, "Hold5Button");
	//THE_BUTTONS->SetOSButtonActivity(false, "DealStart1PndButton");
	//THE_BUTTONS->SetOSButtonActivity(false, "DealStart2PndButton");

	ThePokerGame::Instance()->SetActiveCardButton(false,0);
	ThePokerGame::Instance()->SetActiveCardButton(false,1);
	ThePokerGame::Instance()->SetActiveCardButton(false,2);
	ThePokerGame::Instance()->SetActiveCardButton(false,3);
	ThePokerGame::Instance()->SetActiveCardButton(false,4);
}

void GoldenJokerState::Exit()
{	
	THE_BUTTONS->SetOSButtonActivity(false, "HoldInfoButton");
	THE_BUTTONS->SetOSButtonActivity(false, "Hold2Button");
	THE_BUTTONS->SetOSButtonActivity(false, "Hold3Button");
	THE_BUTTONS->SetOSButtonActivity(false, "Hold4Button");
	THE_BUTTONS->SetOSButtonActivity(false, "Hold5Button");
	
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
		ThePokerGame::Instance()->GoldenJokerUpdateDrawHand();

		if (ThePokerGame::Instance()->GoldenJokerStage == 3)
		{
			if (!ThePokerGame::Instance()->GoldenJokerHeldProcessEnd)
				ENGINE->GetProcessManager()->AddProcessToQueue(new GoldenJokerHeldsProcess);
			else
				OBJECT_HANDLER->GetObject2D("GoldenJokerMsg")->SetVisible(true);	
		}

		if (ThePokerGame::Instance()->GoldenJokerStage == 4)
			ENGINE->GetProcessManager()->AddProcessToQueue(new GoldenJokerCardProcess);

		if (ThePokerGame::Instance()->GoldenJokerStage == 5)
		{
			OBJECT_HANDLER->GetObject2D("GoldenJokerMsg")->SetVisible(false);
			ENGINE->StateTransition("HiloGamble");
		}
	}
}
