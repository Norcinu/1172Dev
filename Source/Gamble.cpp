#include "PokerGame.h"
#include <ObjectHandler.h>

const	unsigned char	GambleWinPercentageVlowTable[MaxSelectableGames][16] = {{43,45,50,45,40,24,42,45,40,45,40,33,30,30,30,30},
																				{43,45,50,45,40,24,42,45,40,45,40,33,30,30,30,30}};

const	unsigned char	GambleWinPercentageLowTable[MaxSelectableGames][16]  = {{43,59,66,59,53,35,55,59,53,50,50,43,40,40,40,40},
																				{43,59,66,59,53,35,55,59,53,50,50,43,40,40,40,40}};

const	unsigned char	GambleWinPercentageTable[MaxSelectableGames][16]     = {{50,66,75,66,60,40,63,66,60,66,60,50,50,50,50,50},
																				{50,66,75,66,60,40,63,66,60,66,60,50,50,50,50,50}};

extern  signed long HiloStore[MaxSelectableGames],HiloStore2[MaxSelectableGames],HighStore[MaxSelectableGames],StandardStore[MaxSelectableGames];
extern  signed long HiloStoreMinValue[MaxSelectableGames];

unsigned char PokerGame::GetWinPerc(unsigned char Level)
{	
	if(HiloStore[GameIndex] < HiloStoreMinValue[GameIndex])
		return(GambleWinPercentageVlowTable[GameIndex][Level]);
	else if(HiloStore[GameIndex] < (HiloStoreMinValue[GameIndex]/2))
		return(GambleWinPercentageLowTable[GameIndex][Level]);
	else
		return(GambleWinPercentageTable[GameIndex][Level]);	
}