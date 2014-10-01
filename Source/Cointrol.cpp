#include "PokerGame.h"
#include <ObjectHandler.h>

void PokerGame::Cointrol(void)
{
	SubStakeFromCredits(WageredAmount,MODEL_NUMBER);	
	ReflexIn();	
}
