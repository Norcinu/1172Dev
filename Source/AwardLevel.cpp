#include "PokerGame.h"
#include <ObjectHandler.h>

unsigned int PokerGame::FunctionMaxLevels(void)
{
	if (GetGameIndex() == Game100p)	
		return(MAX_LEVEL);
	else
		return(MAX_LEVEL-1);

}

unsigned int PokerGame::FunctionMaxAwards(void)
{
	if (GetGameIndex() == Game100p)	
		return(MAX_AWARDS);
	else
		return(MAX_AWARDS-1);

}

unsigned int PokerGame::FunctionLevel(void)
{
	if (GetGameIndex() == Game100p)	
		return(LEVEL);
	else
		return(LEVEL-1);
}