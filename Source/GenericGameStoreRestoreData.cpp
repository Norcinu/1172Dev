#include <GenericDefines.h>
#include "Game.h"
#include "PokerGame.h"

extern unsigned int  ModeGameInCtr[MaxSelectableGames];

void Game::StoreData()
{
	if(GetDoorStatus())  //Don't store if door Open.
		return; 
	
	if(mStoringThreadActive)
		return;

	mStoringThreadActive = 1;
	_beginthread(&Game::BeginStoreDataThread, 0, static_cast<void*>(this));
}

void Game::BeginStoreDataThread(void* Param)
{
	static_cast<Game*>(Param)->StoreDataThread();
}

void Game::StoreDataThread()
{
	int status;
	int value;
	int file;

	SetFileAction();

	std::string gameControlPath = ENGINE->GetWriteablePath();
	gameControlPath.append("\\1172G.dat");

	file = _open(gameControlPath.c_str(), FILE_WRITE, _S_IREAD | _S_IWRITE);
	
	if(file == NULL)
	{
		ClearFileAction();
		mStoringThreadActive = 0;
		return;
	}

	value   = POKER_GAME->WageredAmount;_write(file,&value,4);
	status  = value;

	value   = POKER_GAME->FinalWinValue;_write(file,&value,4);
	status += value;

	for (int i=0; i < MaxSelectableGames; i++)
	{

		value   = POKER_GAME->StandardStore[i];_write(file,&value,4);
		status += POKER_GAME->StandardStore[i];

		value   = POKER_GAME->HiloStore[i];_write(file,&value,4);
		status += POKER_GAME->HiloStore[i];

		value   = POKER_GAME->HiloStore2[i];_write(file,&value,4);
		status += POKER_GAME->HiloStore2[i];

		value   = POKER_GAME->ModeStore[i];_write(file,&value,4);
		status += POKER_GAME->ModeStore[i];
		
		value   = POKER_GAME->ModeStoreTrigger[i];_write(file,&value,4);
		status += POKER_GAME->ModeStoreTrigger[i];

		value   = POKER_GAME->ModeStoreExit[i];_write(file,&value,4);
		status += POKER_GAME->ModeStoreExit[i];

		value   = POKER_GAME->ModeGameIn[i];_write(file,&value,4);
		status += POKER_GAME->ModeGameIn[i];  

		value   = ModeGameInCtr[i];_write(file,&value,4);
		status += ModeGameInCtr[i]; 
		
		value   = POKER_GAME->HighStore[i];_write(file,&value,4);
		status += POKER_GAME->HighStore[i]; 
	}
			
	value = status;_write(file,&value,4);

	_close(file);
	ClearFileAction();
	mStoringThreadActive = 0;
}

void Game::RestoreData()
{
	int loopCounter = 0;
	while(mStoringThreadActive && loopCounter < 500)
	{
		Sleep(2);
		loopCounter++;
	}
	if(loopCounter >499)
	{
		mStoringThreadActive = 0;
	}
	
	int LiveCsum = 0;
	int StoredCsum = 0;
	int file = 0;
	int value = 0;
	float fvalue = 0.0f;

	int PrivateKey = 0;

	SetFileAction();

	std::string gameControlPath = ENGINE->GetWriteablePath();
	gameControlPath.append("\\1172G.dat");

	file = _open(gameControlPath.c_str(), FILE_READ, _S_IREAD | _S_IWRITE);

	if(file == NULL)
	{
		ClearFileAction();
		return;
	}
	
	if(_read(file, &value, 4) > 0) 
		POKER_GAME->WageredAmount = value; 
	LiveCsum += value;

	if(_read(file, &value, 4) > 0) 
		POKER_GAME->FinalWinValue = value; 
	LiveCsum += value;


	for (int i=0; i < MaxSelectableGames; i++)
	{		
		if(_read(file, &value, 4) > 0) 
			POKER_GAME->StandardStore[i] = value;
		LiveCsum += value; 
		
		if(_read(file, &value, 4) > 0)
			POKER_GAME->HiloStore[i] = value;
		LiveCsum += value; 

		if(_read(file, &value, 4) > 0)
			POKER_GAME->HiloStore2[i] = value;
		LiveCsum += value; 
		
		if(_read(file, &value, 4) > 0) 
			POKER_GAME->ModeStore[i] = value;	
		LiveCsum += value; 
		
		if(_read(file, &value, 4) > 0) 
			POKER_GAME->ModeStoreTrigger[i] = value;	
		LiveCsum += value; 

		if(_read(file, &value, 4) > 0) 
			POKER_GAME->ModeStoreExit[i] = value;	
		LiveCsum += value; 

		if(_read(file, &value, 4) > 0) 
			POKER_GAME->ModeGameIn[i] = value;	
		LiveCsum += value; 

		if(_read(file, &value, 4) > 0) 
			ModeGameInCtr[i] = value;	
		LiveCsum += value; 

		if(_read(file, &value, 4) > 0) 
			POKER_GAME->HighStore[i] = value;	
		LiveCsum += value; 
	}
	
	if(_read(file, &value, 4) > 0) 
		StoredCsum = value; //check sum
	
	_close(file);
	ClearFileAction();
	
	if((LiveCsum != StoredCsum)||!StoredCsum)
	{
		mStake = 0;		
		mGameCounter = 0;
		POKER_GAME->Rinit();
		StoreData();
		if(LiveCsum != StoredCsum)
		{
			SetCriticalError(ERR_GAME_DATA_RESET);
		}
	}
}