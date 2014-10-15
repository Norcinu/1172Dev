#include <ObjectHandler.h>
#include <TextureManager.h>
#include "GenericButtons.h"
#include "CollectProcess.h"
#include "Defines.h"
#include "Game.h"
#include "PaymentState.h"
#include "PokerGame.h"

static bool Registered = ENGINE->AddState("Payment", new PaymentState());

extern unsigned char global_quit;

PaymentState::PaymentState()
{

}

PaymentState::~PaymentState()
{
	
}

void PaymentState::Enter()
{
	POKER_GAME->PayOutStage = 1;
}

void PaymentState::Exit()
{
	POKER_GAME->PayOutStage = 0;
}

#include <sstream>

void PaymentState::Update()
{	
	
	PROFILE(__FUNCTION__);	

	if(ENGINE->GetProcessManager()->GetNumQueueProcesses())
	{
		return;
	}

	if(GetCurrentError())
	{

	}

	if(!global_quit)
	{		
		if (POKER_GAME->PayOutStage > 4)
		{
			ENGINE->StateTransition("EndGame");
		}
		else
		{
			POKER_GAME->PayOut();			
		}
	}
}
