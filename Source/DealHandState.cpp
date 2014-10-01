#include <ObjectHandler.h>
#include <TextureManager.h>
#include "GenericButtons.h"
#include "Defines.h"
#include "Game.h"
#include "DealHandState.h"
#include "PokerHandProcess.h"
#include "PokerGame.h"

static bool Registered = TheEngine::Instance()->AddState("DealHand", new DealHandState());

extern unsigned char global_quit;

DealHandState::DealHandState()
{

}

DealHandState::~DealHandState()
{

}

void DealHandState::Enter()
{
	ThePokerGame::Instance()->PokerHandProcessComplete = false;

	for (int i=0; i<5; i++)
	{
		ThePokerGame::Instance()->PokerHand[i].id = 0;
		ThePokerGame::Instance()->PokerHand[i].hold = 0;
	}

	ThePokerGame::Instance()->DealRoll();
	ThePokerGame::Instance()->DealFirstHand();
	ThePokerGame::Instance()->Pay = ThePokerGame::Instance()->Chkwin(ThePokerGame::Instance()->DealHand,0);		
	ThePokerGame::Instance()->JokerWin = ThePokerGame::Instance()->WinInvolvingJoker(ThePokerGame::Instance()->DealHand,ThePokerGame::Instance()->Pay);
	ThePokerGame::Instance()->PokerGameWin = ThePokerGame::Instance()->Pay;
}

void DealHandState::Exit()
{
	ThePokerGame::Instance()->PokerHandProcessComplete = false;
}

#include <sstream>

void DealHandState::Update()
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
		if (ThePokerGame::Instance()->PokerHandProcessComplete)
		{
			if (ThePokerGame::Instance()->PokerHoldCondition())
				TheEngine::Instance()->StateTransition("HoldStart");
			else
				TheEngine::Instance()->StateTransition("Payment");
		}
		else
			TheEngine::Instance()->GetProcessManager()->AddProcessToQueue(new PokerHandProcess);
	}
}
