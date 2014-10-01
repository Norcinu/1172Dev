#include "PokerGame.h"
#include <ObjectHandler.h>


unsigned int PokerGame::SoakBuildPressButton(bool (PokerGame::*ReadButton)(),unsigned int StakeValue,unsigned int GameIndexValue)
{
unsigned int Allow=0;

	if (GetGameIndex() == GameIndexValue)
	{
		if (!GetLocalCharRandomNumber(5)) 
			Allow = 1;
		else if (AutoPlayFlag && GetCredits() >= StakeValue) 
			Allow = 1;
	}	
	return(Allow);
}