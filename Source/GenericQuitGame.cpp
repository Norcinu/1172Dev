#include "Game.h"

extern unsigned char global_quit;

void Game::QuitToMainMenu()
{
	global_quit = true;

	StoreData();

	TheAudioManager::Instance()->GetAudioSample("DROP")->Play();
	THE_BUTTONS->LampsOff();

	if(!GetSwitchStatus(REFILL_KEY))
	{
		SetCurrentState(0);
	}

	SendHeaderOnly(EXIT_CODE1, 1);

	ENGINE->Shutdown();
}