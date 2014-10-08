#include "SetupState.h"
#include "Game.h"
#include <Engine.h>
#include <AudioManager.h>
#include "bo.h"

static bool Registered = ENGINE->AddState("SetupState", new SetupState);

SetupState::SetupState()
{

}

SetupState::~SetupState()
{

}

void SetupState::Enter()
{

}

void SetupState::Update()
{
	if(GetCredits() + GetBankDeposit() >= THE_GAME->GetStake())
	{
		TheEngine::Instance()->StateTransition("DealStart");
	}
	else
	{
		if(GetTerminalFormat() > 1 && !(GetCredits() + GetBankDeposit()))
		{
			TheEngine::Instance()->StateTransition("CompendiumLoad");
		}
		else
		{
			//TheAudioManager::Instance()->GetAudioSample("SX_NOCREDIT")->Play();
			TheEngine::Instance()->StateTransition("Standby");
		}
	}
}

void SetupState::Exit()
{
	
}