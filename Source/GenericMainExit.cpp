#include "Game.h"
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
