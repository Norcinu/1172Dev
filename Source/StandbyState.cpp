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

#ifdef SOAK_BUILD
extern unsigned long add_cdeposit(unsigned long value);
#endif

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
	THE_BUTTONS->SetOSButtonActivity(false, "ChangeStake");
	THE_BUTTONS->SetOSButtonActivity(false, "PopButtons");

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
		add_cdeposit(2000);
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
			if(THE_BUTTONS->ButtonPressed("Collect"))
			{
				ENGINE->GetProcessManager()->AddProcessToQueue(new CollectProcess);	
			}
		}
	}
}
