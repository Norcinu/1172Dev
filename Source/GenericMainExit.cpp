#include "Game.h"
#include "bo.h"

extern unsigned char global_quit;

void Game::GameShutDown(void)
{
	int loopCounter = 0;
	bool threadsRunning = false;
	do
	{
		threadsRunning = false;

		if(mStoringThreadActive)
		{
			threadsRunning = true;
		}
		Sleep(2);
		loopCounter++;
	}
	while(threadsRunning && loopCounter < 500);
	
	CloseSharedMemory();
}

void Game::QuitToMorningReboot()
{
	global_quit = 1;

	StoreData();

//	TheAudioManager::Instance()->GetAudioSample("SX_INFO")->Play();
//	TheAudioManager::Instance()->ProcessSound("SX_AMBIENT", new FadeOutProcess(0.5f));
	TheButtons::Instance()->LampsOff();

	if(!GetSwitchStatus(REFILL_KEY))
	{
		SetCurrentState(0);
	}

	SendHeaderOnly(EXIT_CODE1, 1);

	TheEngine::Instance()->Shutdown();
}