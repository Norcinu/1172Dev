#include <ObjectHandler.h>
#include <TextureManager.h>
#include "GenericButtons.h"
#include "Defines.h"
#include "Game.h"
#include "EndGameState.h"
#include "PokerGame.h"

static bool Registered = ENGINE->AddState("EndGame", new EndGameState());

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

	OBJECT_HANDLER->GetObject2D("GameOverMsg")->SetVisible(true);
	ClearGameInProgress();	
	THE_GAME->StoreData();
	POKER_GAME->SoakStoreThisGame();
	POKER_GAME->SetGameEndTime();

	if (POKER_GAME->GetGameLengthTime() < MIN_GAME_TIME)
		RemainingTime = MIN_GAME_TIME-POKER_GAME->GetGameLengthTime();

//	if (THE_GAME->GetAutoplay())
//		mEndGameDelayTimer = ENGINE->GetSystemTimer().GetRunningTime() + POKER_GAME->GeneralSetGameDelay(RemainingTime);
//	else
		mEndGameDelayTimer = ENGINE->GetSystemTimer().GetRunningTime() + POKER_GAME->GeneralSetGameDelay(0.3f);	//1.0f
}

void EndGameState::Exit()
{
	OBJECT_HANDLER->GetObject2D("GameOverMsg")->SetVisible(false);		
}

#include <sstream>

void EndGameState::Update()
{		
	PROFILE(__FUNCTION__);	

	if(ENGINE->GetProcessManager()->GetNumQueueProcesses())
	{
		return;
	}

	if (ENGINE->GetSystemTimer().GetRunningTime() < mEndGameDelayTimer)
	{
		return;
	}

	if(GetCurrentError())
	{
		return;
	}

	if(!global_quit)
	{						
		ENGINE->StateTransition("DealStart");		
	}
}
