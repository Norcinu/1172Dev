#include "PokerGame.h"
#include <ObjectHandler.h>

void PokerGame::SetGameStartTime(void)
{
	mGameStartTimer = TheEngine::Instance()->GetSystemTimer().GetRunningTime();
}

void PokerGame::SetGameEndTime(void)
{
	mGameEndTimer = TheEngine::Instance()->GetSystemTimer().GetRunningTime();
}

float PokerGame::GetGameLengthTime(void)
{
	return(mGameEndTimer-mGameStartTimer);
}

float PokerGame::GeneralSetGameDelay(float dValue)
{
float Value;

#ifdef FAST_PLAY
	Value = 0;
#else
	Value = dValue;
#endif

	return(Value);
}

