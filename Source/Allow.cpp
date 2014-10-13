#include "PokerGame.h"
#include <ObjectHandler.h>

const unsigned char SuccessRateTab[] = {
											53, // JACKS OR BETTER 
										    70, // TWO PAIR 
					                        70, // THREE OF A KIND 
					                        70, // STRAIGHT 
					                        63, // FLUSH 
					                        42, // FULL HOUSE 
					                        60, // FOUR OF A KIND 
					                        60, // STRAIGHT FLUSH 
					                        60, // FIVE OF A KIND 
					                        66, // FOUR JACKS & JOKER
											60, // FOUR QUEENS & JOKER
											50, // FOUR KINGS & JOKER
											50,	// ROYAL FLUSH 
											50};										
											

void PokerGame::InitialHiloRptTab(unsigned char HigherChance)
{
 unsigned char SuccessRate,StartLevel;
 unsigned char Award, Level;

	SuccessRate = 90;

	Award = FunctionMaxAwards();
	Level = FunctionMaxLevels();
	

	for (i=0; i<Award; i++)
		HiloRepeatTab[i] = 0;

	StartLevel = FindWinLevel(Pay);
		
	if(HigherChance)
	{
		for (i=0; i<(Level - StartLevel); i++)
		{

			if (ModeGameIn[GameIndex])
			{
				if (GetLocalCharRandomNumber(100) < SuccessRate)
					HiloRepeatTab[i] = 1;
				else
					break;
			}
			else
			{
				if (GetLocalCharRandomNumber(100) < SuccessRateTab[StartLevel+i])
					HiloRepeatTab[i] = 1;
				else
					break;
			}
		}
	}
	else // set for no extra cost hilo
	{
		for (i=0; i < Level-StartLevel; i++)
		{
			if (GetLocalCharRandomNumber(100) < GetWinPerc(i+StartLevel))
			{
				HiloRepeatTab[i] = 1;
			}
			else
				break;
		}
	}

/*DEBUG
	for (i=0; i<Level-StartLevel; i++)
		HiloRepeatTab[i] = 1;*/
}

unsigned char PokerGame::AllowGambleReflexValue(unsigned char JokerWin)
{
unsigned char Allow = 0;

	if (!ModeGameIn[GameIndex])
	{
		if (!JokerWin)
			Allow = 1;
	}
	return(Allow);
}	


unsigned char PokerGame::AllowInsertDoubleJoker(unsigned int Value,unsigned char Hand)
{
unsigned char Allow = 0;

	if (Value && GetLocalCharRandomNumber(100) < 2)
	{
		if (Hand == 1)
			Allow = GetLocalCharRandomNumber(3)+1;
		else
			Allow = 3;
	}
	else if (Value)
		Allow = 4;		//Failed Attempt

	return(Allow);
}
/************************************************************************/
unsigned char PokerGame::AllowHighWins(void)
{
unsigned char Allow;

	if (!ModeGameIn[GameIndex] && (HighStore[GameIndex] > HIGH_STORE_MIN_VALUE) && !GetLocalCharRandomNumber(30))  
		Allow = 1;
	else
		Allow = 0;
	return(Allow);
}
