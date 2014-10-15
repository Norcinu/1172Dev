#include "DealHandState.h"
#include <ObjectHandler.h>
#include <TextureManager.h>
#include "GenericButtons.h"
#include "Defines.h"
#include "Game.h"
#include "PokerHandProcess.h"
#include "PokerGame.h"

static bool Registered = ENGINE->AddState("DealHand", new DealHandState());

extern unsigned char global_quit;

DealHandState::DealHandState()
{

}

DealHandState::~DealHandState()
{

}

void DealHandState::Enter()
{
	POKER_GAME->PokerHandProcessComplete = false;

	for (int i=0; i<5; i++)
	{
		POKER_GAME->PokerHand[i].id = 0;
		POKER_GAME->PokerHand[i].hold = 0;
	}

	OBJECT_HANDLER->GetObject2D("GraphicalButton07")->SetVisible(false);
	OBJECT_HANDLER->GetObject2D("GraphicalButton08")->SetVisible(false);
/*
	OBJECT_HANDLER->GetObject2D("Legend1DealDrawLit")->SetVisible(false);
	OBJECT_HANDLER->GetObject2D("Legend1DealDrawNlit")->SetVisible(false);
	OBJECT_HANDLER->GetObject2D("Legend2DealDrawLit")->SetVisible(false);
	OBJECT_HANDLER->GetObject2D("Legend2DealDrawNlit")->SetVisible(false);
	OBJECT_HANDLER->GetObject2D("Legend2DealDrawNlit")->SetVisible(false);*/

	POKER_GAME->DealRoll();
	POKER_GAME->DealFirstHand();
	POKER_GAME->Pay = POKER_GAME->Chkwin(POKER_GAME->DealHand,0);		
	POKER_GAME->JokerWin = POKER_GAME->WinInvolvingJoker(POKER_GAME->DealHand,POKER_GAME->Pay);
	POKER_GAME->PokerGameWin = POKER_GAME->Pay;
}

void DealHandState::Exit()
{
	POKER_GAME->PokerHandProcessComplete = false;
}

#include <sstream>

void DealHandState::Update()
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
		if (POKER_GAME->PokerHandProcessComplete)
		{
			if (POKER_GAME->PokerHoldCondition())
				ENGINE->StateTransition("HoldStart");
			else
				ENGINE->StateTransition("Payment");
		}
		else
			ENGINE->GetProcessManager()->AddProcessToQueue(new PokerHandProcess);
	}
}
