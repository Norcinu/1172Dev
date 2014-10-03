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
	gameControlPath.append("\\1170G.dat");

	file = _open(gameControlPath.c_str(), FILE_WRITE, _S_IREAD | _S_IWRITE);
	
	if(file == NULL)
	{
		ClearFileAction();
		mStoringThreadActive = 0;
		return;
	}

	value   = ThePokerGame::Instance()->WageredAmount;_write(file,&value,4);
	status  = value;

	value   = ThePokerGame::Instance()->FinalWinValue;_write(file,&value,4);
	status += value;

	for (int i=0; i < MaxSelectableGames; i++)
	{

		value   = ThePokerGame::Instance()->StandardStore[i];_write(file,&value,4);
		status += ThePokerGame::Instance()->StandardStore[i];

		value   = ThePokerGame::Instance()->HiloStore[i];_write(file,&value,4);
		status += ThePokerGame::Instance()->HiloStore[i];

		value   = ThePokerGame::Instance()->HiloStore2[i];_write(file,&value,4);
		status += ThePokerGame::Instance()->HiloStore2[i];

		value   = ThePokerGame::Instance()->ModeStore[i];_write(file,&value,4);
		status += ThePokerGame::Instance()->ModeStore[i];
		
		value   = ThePokerGame::Instance()->ModeStoreTrigger[i];_write(file,&value,4);
		status += ThePokerGame::Instance()->ModeStoreTrigger[i];

		value   = ThePokerGame::Instance()->ModeStoreExit[i];_write(file,&value,4);
		status += ThePokerGame::Instance()->ModeStoreExit[i];

		value   = ThePokerGame::Instance()->ModeGameIn[i];_write(file,&value,4);
		status += ThePokerGame::Instance()->ModeGameIn[i];  

		value   = ModeGameInCtr[i];_write(file,&value,4);
		status += ModeGameInCtr[i]; 
		
		value   = ThePokerGame::Instance()->HighStore[i];_write(file,&value,4);
		status += ThePokerGame::Instance()->HighStore[i]; 
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
	gameControlPath.append("\\1170G.dat");

	file = _open(gameControlPath.c_str(), FILE_READ, _S_IREAD | _S_IWRITE);

	if(file == NULL)
	{
		ClearFileAction();
		return;
	}
	
	if(_read(file, &value, 4) > 0) 
		ThePokerGame::Instance()->WageredAmount = value; 
	LiveCsum += value;

	if(_read(file, &value, 4) > 0) 
		ThePokerGame::Instance()->FinalWinValue = value; 
	LiveCsum += value;


	for (int i=0; i < MaxSelectableGames; i++)
	{		
		if(_read(file, &value, 4) > 0) 
			ThePokerGame::Instance()->StandardStore[i] = value;
		LiveCsum += value; 
		
		if(_read(file, &value, 4) > 0)
			ThePokerGame::Instance()->HiloStore[i] = value;
		LiveCsum += value; 

		if(_read(file, &value, 4) > 0)
			ThePokerGame::Instance()->HiloStore2[i] = value;
		LiveCsum += value; 
		
		if(_read(file, &value, 4) > 0) 
			ThePokerGame::Instance()->ModeStore[i] = value;	
		LiveCsum += value; 
		
		if(_read(file, &value, 4) > 0) 
			ThePokerGame::Instance()->ModeStoreTrigger[i] = value;	
		LiveCsum += value; 

		if(_read(file, &value, 4) > 0) 
			ThePokerGame::Instance()->ModeStoreExit[i] = value;	
		LiveCsum += value; 

		if(_read(file, &value, 4) > 0) 
			ThePokerGame::Instance()->ModeGameIn[i] = value;	
		LiveCsum += value; 

		if(_read(file, &value, 4) > 0) 
			ModeGameInCtr[i] = value;	
		LiveCsum += value; 

		if(_read(file, &value, 4) > 0) 
			ThePokerGame::Instance()->HighStore[i] = value;	
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
		ThePokerGame::Instance()->Rinit();
		StoreData();
		if(LiveCsum != StoredCsum)
		{
			SetCriticalError(ERR_GAME_DATA_RESET);
		}
	}
}