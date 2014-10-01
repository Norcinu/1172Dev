#include <ObjectHandler.h>
#include <TextureManager.h>
#include "GenericButtons.h"
#include "Defines.h"
#include "Game.h"
#include "DrawHandState.h"
#include "PokerHandProcess.h"
#include "PokerGame.h"

static bool Registered = TheEngine::Instance()->AddState("DrawHand", new DrawHandState());

extern unsigned char global_quit;

DrawHandState::DrawHandState()
{

}

DrawHandState::~DrawHandState()
{

}

void DrawHandState::Enter()
{
	ThePokerGame::Instance()->PokerHandProcessComplete = false;

	ThePokerGame::Instance()->DrawRoll();
	ThePokerGame::Instance()->DealSecondHand();
	ThePokerGame::Instance()->Pay = ThePokerGame::Instance()->Chkwin(ThePokerGame::Instance()->DrawHand,0);		
	ThePokerGame::Instance()->JokerWin = ThePokerGame::Instance()->WinInvolvingJoker(ThePokerGame::Instance()->DrawHand,ThePokerGame::Instance()->Pay);
	ThePokerGame::Instance()->PokerGameWin = ThePokerGame::Instance()->Pay;
}

void DrawHandState::Exit()
{
	ThePokerGame::Instance()->PokerHandProcessComplete = false;
}

#include <sstream>

void DrawHandState::Update()
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
			if (ThePokerGame::Instance()->InsertDoubleJokerFlag)
				TheEngine::Instance()->StateTransition("GoldenJoker");
			else if (ThePokerGame::Instance()->PokerHiloCondition())
				TheEngine::Instance()->StateTransition("HiloGamble");
			else
				TheEngine::Instance()->StateTransition("Payment");
		}
		else
			TheEngine::Instance()->GetProcessManager()->AddProcessToQueue(new PokerHandProcess);
	}
}
