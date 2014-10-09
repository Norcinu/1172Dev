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
	ThePokerGame::Instance()->SoakStoreThisGame();
	ThePokerGame::Instance()->SetGameEndTime();

	if (ThePokerGame::Instance()->GetGameLengthTime() < MIN_GAME_TIME)
		RemainingTime = MIN_GAME_TIME-ThePokerGame::Instance()->GetGameLengthTime();	

	//if (RemainingTime > 1.0f)
	//	mEndGameDelayTimer = ENGINE->GetSystemTimer().GetRunningTime() + ThePokerGame::Instance()->GeneralSetGameDelay(RemainingTime);
	//else
	if (THE_GAME->GetAutoplay())
		mEndGameDelayTimer = ENGINE->GetSystemTimer().GetRunningTime() + ThePokerGame::Instance()->GeneralSetGameDelay(RemainingTime);
	else
		mEndGameDelayTimer = ENGINE->GetSystemTimer().GetRunningTime() + ThePokerGame::Instance()->GeneralSetGameDelay(0.25f);	//1.0f
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

	auto timerrrrr = ENGINE->GetSystemTimer().GetRunningTime();
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
