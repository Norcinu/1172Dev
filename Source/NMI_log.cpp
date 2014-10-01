#include "Game.h"
#include <bo.h>
#include "Defines.h"

#include "NMI_log.h" //gaming house defines



		unsigned long NumberOfGamesPlayed = 0;
		unsigned long CreditBefore = 0;	//very ambigious way of describing credits+bank+stake deduction
		unsigned long CreditAfter  = 0;
		unsigned long GameCounter  = 0;
		unsigned int  FeatBonusWin1= 0;
		unsigned int  FeatBonusWin2= 0;
		unsigned int  FeatBonusWin3= 0;
		unsigned int  FSpinWin	   = 0;
				 int  ChangedPop   = 0; 

				 int  SymbolsOnView[15]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

/******************************************************************************/

