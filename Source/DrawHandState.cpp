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
	POKER_GAME->PokerHandProcessComplete = false;

	POKER_GAME->DrawRoll();
	POKER_GAME->DealSecondHand();
	POKER_GAME->Pay = POKER_GAME->Chkwin(POKER_GAME->DrawHand,0);		
	POKER_GAME->JokerWin = POKER_GAME->WinInvolvingJoker(POKER_GAME->DrawHand,POKER_GAME->Pay);
	POKER_GAME->PokerGameWin = POKER_GAME->Pay;
}

void DrawHandState::Exit()
{
	POKER_GAME->PokerHandProcessComplete = false;
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
		OBJECT_HANDLER->GetObject2D("HoldStartMsg")->SetVisible(false);

		if (!ENGINE->GetProcessManager()->GetNumQueueProcesses())
		{
			if (!m_sleep)
			{
				if (THE_GAME->GetAutoplay() && m_firstHand)
				{
					ENGINE->GetProcessManager()->AddProcessToQueue(new DelayProcess(0.75f));
					m_firstHand = false;
				}
				else if (THE_GAME->GetAutoplay())
					ENGINE->GetProcessManager()->AddProcessToQueue(new DelayProcess(0.35f));
				m_sleep = true;
			}
			else
			{
				m_sleep = false;
				if (POKER_GAME->PokerHandProcessComplete)
				{
					if (POKER_GAME->InsertDoubleJokerFlag)
						ENGINE->StateTransition("GoldenJoker");
					else if (POKER_GAME->PokerHiloCondition()) 
					{
					if (THE_GAME->GetAutoplay())
						{
							THE_GAME->SetAutoplay(false);
							POKER_GAME->SetEnteredOnAutoplay(true);
						}
						ENGINE->StateTransition("HiloGamble");
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
