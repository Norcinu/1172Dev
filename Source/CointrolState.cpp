#include <ObjectHandler.h>
#include "Defines.h"
#include "Game.h"
#include "CointrolState.h"
#include "PokerGame.h"


static bool Registered = TheEngine::Instance()->AddState("Cointrol", new CointrolState());

extern unsigned char global_quit;

CointrolState::CointrolState()
{
	
}

CointrolState::~CointrolState()
{

}

void CointrolState::Enter()
{
	
}

void CointrolState::Exit()
{
	
}

#include <sstream>

void CointrolState::Update()
{	
	
	PROFILE(__FUNCTION__);	

	if(TheEngine::Instance()->GetProcessManager()->GetNumQueueProcesses())
	{
		return;
	}

	if(!global_quit)
	{
		ThePokerGame::Instance()->Cointrol();		
		TheGame::Instance()->UpdateDigits();
		TheEngine::Instance()->StateTransition("DealHand");
	}
}
