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
	POKER_GAME->GoldenJokerStage = 1;
	POKER_GAME->GoldenJokerHeldProcessEnd = false;

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

	POKER_GAME->SetActiveCardButton(false,0);
	POKER_GAME->SetActiveCardButton(false,1);
	POKER_GAME->SetActiveCardButton(false,2);
	POKER_GAME->SetActiveCardButton(false,3);
	POKER_GAME->SetActiveCardButton(false,4);
}

void GoldenJokerState::Exit()
{	
	THE_BUTTONS->SetOSButtonActivity(false, "HoldInfoButton");
	THE_BUTTONS->SetOSButtonActivity(false, "Hold2Button");
	THE_BUTTONS->SetOSButtonActivity(false, "Hold3Button");
	THE_BUTTONS->SetOSButtonActivity(false, "Hold4Button");
	THE_BUTTONS->SetOSButtonActivity(false, "Hold5Button");
	
	POKER_GAME->SetActiveCardButton(false,0);
	POKER_GAME->SetActiveCardButton(false,1);
	POKER_GAME->SetActiveCardButton(false,2);
	POKER_GAME->SetActiveCardButton(false,3);
	POKER_GAME->SetActiveCardButton(false,4);
	
	POKER_GAME->PokerGameWin = POKER_GAME->Pay;
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
		POKER_GAME->GoldenJokerUpdateDrawHand();

		if (POKER_GAME->GoldenJokerStage == 3)
		{
			if (!POKER_GAME->GoldenJokerHeldProcessEnd)
				ENGINE->GetProcessManager()->AddProcessToQueue(new GoldenJokerHeldsProcess);
			else
				OBJECT_HANDLER->GetObject2D("GoldenJokerMsg")->SetVisible(true);	
		}

		if (POKER_GAME->GoldenJokerStage == 4)
			ENGINE->GetProcessManager()->AddProcessToQueue(new GoldenJokerCardProcess);

		if (POKER_GAME->GoldenJokerStage == 5)
		{
			OBJECT_HANDLER->GetObject2D("GoldenJokerMsg")->SetVisible(false);
			ENGINE->StateTransition("HiloGamble");
		}
	}
}
