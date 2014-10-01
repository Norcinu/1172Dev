#include <ObjectHandler.h>
#include <TextureManager.h>
#include "GenericButtons.h"
#include "CollectProcess.h"
#include "Defines.h"
#include "Game.h"
#include "StandbyState.h"
#include <bo.h>

static bool Registered = TheEngine::Instance()->AddState("Standby", new StandbyState());

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
	TheEngine::Instance()->GetProcessManager()->AddProcessToList(m_standbyFlash);	
	m_standbyTimer = TheEngine::Instance()->GetSystemTimer().GetRunningTime() + STANDBY_TIME;
}

void StandbyState::Exit()
{
	m_standbyFlash->SetCompleted();
	GET_BUTTONS->SetOSButtonActivity(false, "HoldInfoButton");
}

void StandbyState::Update()
{
	if(TheEngine::Instance()->GetProcessManager()->GetNumQueueProcesses())
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
		GET_BUTTONS->StandbyButtons();

		TheGame::Instance()->UpdateDigits();

		if(GetSwitchStatus(REFILL_KEY))
		{
			TheGame::Instance()->QuitToMainMenu();
			return;
		}

		if(TheEngine::Instance()->GetSystemTimer().GetRunningTime() > m_standbyTimer)
		{
			if((GetTerminalFormat() > 1) && (!GetDynamicDoNotQuitGame(MODEL_NUMBER)))
				TheGame::Instance()->QuitToMainMenu();
		}

		if(GetCredits()+GetBankDeposit() >= MINIMUM_BET)
		{
			TheEngine::Instance()->StateTransition("DealStart");
		}

		if(GET_BUTTONS->OSButtonPressed("HoldInfoButton"))
		{
			TheEngine::Instance()->StateTransition("Help");
			return;
		}
		else if(GET_BUTTONS->ButtonPressed("Menu"))
		{
			TheGame::Instance()->QuitToMainMenu();
		}

		if((GetCredits()+GetBankDeposit()>0))
		{
			if(GET_BUTTONS->ButtonPressed("Collect") || GET_BUTTONS->OSButtonPressed("CollectButton"))
			{
				TheEngine::Instance()->GetProcessManager()->AddProcessToQueue(new CollectProcess);	
			}
		}
	}
}