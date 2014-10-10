#include <ObjectHandler.h>
#include <TextureManager.h>
#include "GenericButtons.h"
#include "Defines.h"
#include "Game.h"
#include "DrawHandState.h"
#include "PokerHandProcess.h"
#include "PokerGame.h"
#include "DelayProcess.h"

static bool Registered = ENGINE->AddState("DrawHand", new DrawHandState());

extern unsigned char global_quit;

DrawHandState::DrawHandState()
{

}

DrawHandState::~DrawHandState()
{

}

void DrawHandState::Enter()
{
	m_sleep = false;
	m_firstHand = true;
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
		if (!ENGINE->GetProcessManager()->GetNumQueueProcesses())
		{
			if (!m_sleep)
			{
				if (THE_GAME->GetAutoplay() && m_firstHand)
				{
					ENGINE->GetProcessManager()->AddProcessToQueue(new DelayProcess(0.75f));
					m_firstHand = false;
				}
				else
					ENGINE->GetProcessManager()->AddProcessToQueue(new DelayProcess(0.35f));
				m_sleep = true;
			}
			else
			{
				m_sleep = false;
				if (ThePokerGame::Instance()->PokerHandProcessComplete)
				{
					if (ThePokerGame::Instance()->InsertDoubleJokerFlag)
						ENGINE->StateTransition("GoldenJoker");
					else if (ThePokerGame::Instance()->PokerHiloCondition()) 
					{
						ENGINE->StateTransition("HiloGamble");
						THE_GAME->SetAutoplay(false);
					}
					else
						ENGINE->StateTransition("Payment");
				}
				else
					ENGINE->GetProcessManager()->AddProcessToQueue(new PokerHandProcess);
			}
		}
	}
}
