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
	//LightsOff();

	SendHeaderOnly(EXIT_CODE1, 1);

	Sleep(1000);

	//while(StoringControlThreadActive)
		msg_service();

	global_quit = 1;
}