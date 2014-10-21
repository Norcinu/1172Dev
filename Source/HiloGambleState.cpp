#include <ObjectHandler.h>
#include <TextureManager.h>
#include "GenericButtons.h"
#include "Defines.h"
#include "Game.h"
#include "HiloGambleState.h"
#include "PokerHandProcess.h"
#include "PokerGame.h"
#include "DelayProcess.h"

static bool Registered = ENGINE->AddState("HiloGamble", new HiloGambleState());

extern unsigned char global_quit;

HiloGambleState::HiloGambleState()
{

}

HiloGambleState::~HiloGambleState()
{

}

void HiloGambleState::Enter()
{
	THE_BUTTONS->SetButtonActivity(false, "FrontStart");
	THE_BUTTONS->SetButtonActivity(false, "Stake");	
	THE_BUTTONS->SetButtonActivity(false, "TopStart");	
	THE_BUTTONS->SetButtonActivity(false, "Collect");

	POKER_GAME->HiloGambleStage = 1;
	POKER_GAME->HiloDelayTimer = 0;
	
	POKER_GAME->SetHoldHiloGraphics();
}

void HiloGambleState::Exit()
{
	POKER_GAME->HiloGambleStage = 0;
	POKER_GAME->ActivateHiloGambleGraphics = 0;
	THE_BUTTONS->SetOSButtonActivity(false, "LoButton");
	THE_BUTTONS->SetOSButtonActivity(false, "HiButton");
	
	OBJECT_HANDLER->GetObject2D("Legend1SwopLit")->SetVisible(false);
	OBJECT_HANDLER->GetObject2D("Legend1SwopNlit")->SetVisible(false);

	if (POKER_GAME->GetEnteredOnAutoplay())
	{
		THE_GAME->SetAutoplay(true);
		POKER_GAME->SetEnteredOnAutoplay(false);
		ENGINE->GetProcessManager()->AddProcessToQueue(new DelayProcess(1.f));
	}
}

#include <sstream>

void HiloGambleState::Update()
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
		POKER_GAME->HiloGamble(POKER_GAME->JokerWin);
		
		if (POKER_GAME->HiloGambleStage > 9)
		{
			if (POKER_GAME->GetEnteredOnAutoplay())
				THE_GAME->SetAutoplay(true);
			ENGINE->StateTransition("Payment");
		}
	}
}
