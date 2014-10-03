#include <ObjectHandler.h>
#include <TextureManager.h>
#include "GenericButtons.h"
#include "CollectProcess.h"
#include "Defines.h"
#include "Game.h"
#include "StandbyState.h"
#include <bo.h>

static bool Registered = ENGINE->AddState("Standby", new StandbyState());

extern unsigned char global_quit;

StandbyState::StandbyState()
{
	m_standbyFlash = 0;
	m_standbyTimer = 0.0f;
	m_previousCredits = 0;
}

StandbyState::~StandbyState()
{

}

void StandbyState::Enter()
{
	m_standbyFlash = new StandbyFlashProcess();
	ENGINE->GetProcessManager()->AddProcessToList(m_standbyFlash);
	m_standbyTimer = ENGINE->GetSystemTimer().GetRunningTime() + STANDBY_TIME;
}

void StandbyState::Exit()
{
	m_standbyFlash->SetCompleted();
	THE_BUTTONS->SetOSButtonActivity(false, "HoldInfoButton");
}

void StandbyState::Update()
{
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
#ifdef SOAK_BUILD
		SetAutoCreditFlag();
#endif
		THE_BUTTONS->StandbyButtons();

		THE_GAME->UpdateDigits();
		
		if(GetSwitchStatus(REFILL_KEY))
		{
			THE_GAME->QuitToMainMenu();
			return;
		}

		if(ENGINE->GetSystemTimer().GetRunningTime() > m_standbyTimer)
		{
			if((GetTerminalFormat() > 1) && (!GetDynamicDoNotQuitGame(MODEL_NUMBER)))
				THE_GAME->QuitToMainMenu();
		}

		if(GetCredits()+GetBankDeposit() >= MINIMUM_BET)
		{
			ENGINE->StateTransition("DealStart");
		}

		if(THE_BUTTONS->OSButtonPressed("HoldInfoButton"))
		{
			ENGINE->StateTransition("Help");
			return;
		}
		else if(THE_BUTTONS->ButtonPressed("Menu"))
		{
			THE_GAME->QuitToMainMenu();
		}

		if((GetCredits()+GetBankDeposit() > 0))
		{
			if(THE_BUTTONS->ButtonPressed("Collect") || THE_BUTTONS->OSButtonPressed("CollectButton"))
			{
				ENGINE->GetProcessManager()->AddProcessToQueue(new CollectProcess);	
			}
		}
	}
}
