#include <ObjectHandler.h>
#include <TextureManager.h>
#include "GenericButtons.h"
#include "CollectProcess.h"
#include "Defines.h"
#include "Game.h"
#include "PaymentState.h"
#include "PokerGame.h"

static bool Registered = TheEngine::Instance()->AddState("Payment", new PaymentState());

extern unsigned char global_quit;

PaymentState::PaymentState()
{
	
}

PaymentState::~PaymentState()
{

}

void PaymentState::Enter()
{
	ThePokerGame::Instance()->PayOutStage = 1;
}

void PaymentState::Exit()
{
	ThePokerGame::Instance()->PayOutStage = 0;
}

#include <sstream>

void PaymentState::Update()
{	
	
	PROFILE(__FUNCTION__);	

	if(TheEngine::Instance()->GetProcessManager()->GetNumQueueProcesses())
	{
		return;
	}

	if(GetCurrentError())
	{

	}

	if(!global_quit)
	{		
		if (ThePokerGame::Instance()->PayOutStage > 4)
			TheEngine::Instance()->StateTransition("EndGame");
		else
		{
			ThePokerGame::Instance()->PayOut();			
		}
	}
}
