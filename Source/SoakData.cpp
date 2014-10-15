#include "PokerGame.h"
#include <ObjectHandler.h>

void PokerGame::SoakStoreThisGame(void)
{
#ifdef SOAK_BUILD
	FILE	      *fplog;
	static long GameNo = 0;
	static int TotalWageredAmount = 0;
	static int TotalWonAmount = 0;
	static char FirstPass = 0;

	GameNo++;
	TotalWageredAmount += WageredAmount;
	TotalWonAmount += FinalWinValue;

	float GamePercentage = ((float)TotalWonAmount/(float)TotalWageredAmount)*100;

	if(!(fplog=fopen("d:\\1172\\1172FPlay.log","a")) == NULL)
	{		
		if(!FirstPass)
		{
			fprintf(fplog,"G.Number\t Bet\t Win\t T.Bet\t T.win\t P.Payout\t D.Joker\n\n");
			FirstPass = 1;
		}

		if (TotalWonAmount)
			fprintf(fplog,"%05d\t %05d\t %05d\t %05d\t %05d\t %05f\t %05d\n",GameNo,WageredAmount,FinalWinValue,TotalWageredAmount,TotalWonAmount,GamePercentage,SoakDoubleJokerFlag);
		else
			fprintf(fplog,"%05d\t %05d\t %05d\t %05d\t %05d\t %05f\t %05d\n",GameNo,WageredAmount,FinalWinValue,TotalWageredAmount,TotalWonAmount,0.0,SoakDoubleJokerFlag);
						
		fclose(fplog);
	}
	
	SoakDoubleJokerFlag = 0;
#endif
}