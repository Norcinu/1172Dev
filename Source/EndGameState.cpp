#include <ObjectHandler.h>
#include <TextureManager.h>
#include "GenericButtons.h"
#include "Defines.h"
#include "Game.h"
#include "EndGameState.h"
#include "PokerGame.h"

static bool Registered = TheEngine::Instance()->AddState("EndGame", new EndGameState());

extern unsigned char global_quit;

EndGameState::EndGameState()
{
	mEndGameDelayTimer = 0;
}

EndGameState::~EndGameState()
{

}

void EndGameState::Enter()
{
	float RemainingTime = 0;

	TheObjectHandler::Instance()->GetObject2D("GameOverMsg")->SetVisible(true);	
	ClearGameInProgress();	
	TheGame::Instance()->StoreData();
	ThePokerGame::Instance()->SoakStoreThisGame();
	ThePokerGame::Instance()->SetGameEndTime();

	if (ThePokerGame::Instance()->GetGameLengthTime() < MIN_GAME_TIME)
		RemainingTime = MIN_GAME_TIME-ThePokerGame::Instance()->GetGameLengthTime();	

	if (RemainingTime > 1.0f)
		mEndGameDelayTimer = TheEngine::Instance()->GetSystemTimer().GetRunningTime() + ThePokerGame::Instance()->GeneralSetGameDelay(RemainingTime);
	else
		mEndGameDelayTimer = TheEngine::Instance()->GetSystemTimer().GetRunningTime() + ThePokerGame::Instance()->GeneralSetGameDelay(1.0f);	
}

void EndGameState::Exit()
{
	TheObjectHandler::Instance()->GetObject2D("GameOverMsg")->SetVisible(false);		
}

#include <sstream>

void EndGameState::Update()
{		
	PROFILE(__FUNCTION__);	

	if(TheEngine::Instance()->GetProcessManager()->GetNumQueueProcesses())
	{
		return;
	}

	if (TheEngine::Instance()->GetSystemTimer().GetRunningTime() < mEndGameDelayTimer)
	{
		return;
	}

	if(GetCurrentError())
	{
		return;
	}

	if(!global_quit)
	{						
		TheEngine::Instance()->StateTransition("DealStart");		
	}
}
