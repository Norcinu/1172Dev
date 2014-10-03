#include "PokerGame.h"
#include <ObjectHandler.h>
#include "PayTransferProcess.h"
#include "JackpotProcess.h"

void PokerGame::PayOut(void)
{
	switch(PayOutStage)
	{
		case 1: if (PayOutEnterJackpotFanfare())
					PayOutStage = 2;
				else
					PayOutStage = 3;
				break;
		case 2: PayOutDoJackpotFanfare();
				PayOutStage = 3;
				break;
		case 3: PayOutReflex();
				PayOutStage = 4;
				break;
		case 4: //PayOutPaytransfer(); // DEBUG DEBUG
				PayOutStage = 5;
				break;
		default:
				break;
	}
}

bool PokerGame::PayOutEnterJackpotFanfare(void)
{
	bool Status = false;

	if(FindWinLevel(Pay) == FunctionMaxLevels())
	{
		if (RoyalFlushWinFlag && GetGameIndex() == Game200p)
			SetAwardValueLitState(FunctionMaxLevels()+1,MLAMP_ON);
		else
			SetAwardValueLitState(FunctionMaxLevels(),MLAMP_ON);
		for (int i=0; i<2; i++)
		{
			TheAudioManager::Instance()->GetAudioSample("WOLF_SND")->Play();		
		}		

		Status = true;
	}
	
	return(Status);
}

bool PokerGame::PayOutDoJackpotFanfare(void)
{
	bool Status = false;

	ENGINE->GetProcessManager()->AddProcessToQueue(new JackpotProcess);
	return(Status);
}

bool PokerGame::PayOutReflex(void)
{
	bool Status = false;

	ReflexOut();
	return(Status);
}

bool PokerGame::PayOutPaytransfer(void)
{
	bool Status = false;
	
	if (!GetDoorStatus())
		SendGameOutcome2Server(WageredAmount,0,PokerGameWin,((MODEL_NUMBER * 1000) + RELEASE_NUMBER * 10),Pay);

	ENGINE->GetProcessManager()->AddProcessToQueue(new PayTransferProcess);
	return(Status);
}




