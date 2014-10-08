#include "CompendiumLoadState.h"
#include "Game.h"
#include <Engine.h>
#include <AudioManager.h>
#include <bo.h>

static const float WAIT_TIME = 30.0f;

void CompendiumLoadState::Enter()
{
	mTimer = TheEngine::Instance()->GetSystemTimer().GetRunningTime() + WAIT_TIME;
	//TheButtons::Instance()->SetButtonState("Menu", true, LAMP_FLASH);
}

void CompendiumLoadState::Exit()
{

}

void CompendiumLoadState::Update()
{
	/**
	* Return for any queue processes such as TiTo so game doesn't quit
	*/
	if(TheEngine::Instance()->GetProcessManager()->GetNumQueueProcesses())
	{
		return;
	}
	//TheButtons::Instance()->SetButtonState("Menu", true, LAMP_FLASH);
	TheButtons::Instance()->SetButtonActivity(true, "Menu", LAMP_FLASH);
	/**
	* If timer has passed, refill key is turned, menu is pressed or player inserts money then quit to the home page
	*/
	if(mTimer < TheEngine::Instance()->GetSystemTimer().GetRunningTime() ||
		(GetBankDeposit() + GetCredits()) ||
		GetSwitchStatus(REFILL_KEY) ||
		TheButtons::Instance()->ButtonPressed("Menu"))
	{
		TheGame::Instance()->QuitToMainMenu();
		return;
	}
		
	if(GetRebootRequired())
	{
		THE_GAME->QuitToMainMenu();
		//TheGame::Instance()->QuitToMorningReboot();
		return;
	}

}