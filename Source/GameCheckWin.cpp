#include "GenericReelControl.h"
#include "Game.h"

unsigned int SymbolWinValueTable3_100[]={ 0, 500, 2000, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
unsigned int SymbolWinValueTable4_100[]={ 0,1000, 2500, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
unsigned int SymbolWinValueTable5_100[]={ 0,1500, 5000, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

unsigned int SymbolWinValueTable3_200[]={ 0,1000, 4000, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
unsigned int SymbolWinValueTable4_200[]={ 0,2000, 5000, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
unsigned int SymbolWinValueTable5_200[]={ 0,3000, 5000, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};




unsigned int GenericReelControl::Check4Win(unsigned char r1, unsigned char r2, unsigned char r3, unsigned char r4, unsigned char r5)
{
	unsigned int WinValue = 0;
	if(TheGame::Instance()->GetStake() == 200)
	{
		SymbolTablePtr3 = &SymbolWinValueTable3_200[0];
		SymbolTablePtr4 = &SymbolWinValueTable4_200[0];
		SymbolTablePtr5 = &SymbolWinValueTable5_200[0];
	}
	else //lower stakes use same win tables
	{
		SymbolTablePtr3 = &SymbolWinValueTable3_100[0];
		SymbolTablePtr4 = &SymbolWinValueTable4_100[0];
		SymbolTablePtr5 = &SymbolWinValueTable5_100[0];
	}
		
	line[0] = mHexReelLayout[mReelLayout[5 * r1]];
	line[1] = mHexReelLayout[mReelLayout[1 + (5 * r2)]];
	line[2] = mHexReelLayout[mReelLayout[2 + (5 * r3)]];
	line[3] = mHexReelLayout[mReelLayout[3 + (5 * r4)]];
	line[4] = mHexReelLayout[mReelLayout[4 + (5 * r5)]];

	mGoldenSpinSymbol = line[2]; //GV: moved in here to see if FreeSpins Symbol in win combo

	//Win 5
	int Ptr2WinValue = (line[0] | line[1] | line[2] | line[3] | line[4]);

	WinValue = SymbolTablePtr5[mWinningSymbol[Ptr2WinValue]];
	if(WinValue)
	{
		mSymsToAnimate[0] = 5 * r1;
		mSymsToAnimate[1] = 1 + (5 * r2);
		mSymsToAnimate[2] = 2 + (5 * r3);
		mSymsToAnimate[3] = 3 + (5 * r4);
		mSymsToAnimate[4] = 4 + (5 * r5);
		return (WinValue);
	}

	//4 from left
	Ptr2WinValue = (line[0] | line[1] | line[2] | line[3]);

	WinValue = SymbolTablePtr4[mWinningSymbol[Ptr2WinValue]];
	if(WinValue)
	{
		mSymsToAnimate[0] = 5 * r1;
		mSymsToAnimate[1] = 1 + (5 * r2);
		mSymsToAnimate[2] = 2 + (5 * r3);
		mSymsToAnimate[3] = 3 + (5 * r4);
		mSymsToAnimate[4] = -1;
		return (WinValue);
	}

	//4 from right
	Ptr2WinValue = (line[1] | line[2] | line[3] | line[4]);

	WinValue = SymbolTablePtr4[mWinningSymbol[Ptr2WinValue]];
	if(WinValue)
	{
		mSymsToAnimate[0] = -1;
		mSymsToAnimate[1] = 1 + (5 * r2);
		mSymsToAnimate[2] = 2 + (5 * r3);
		mSymsToAnimate[3] = 3 + (5 * r4);
		mSymsToAnimate[4] = 4 + (5 * r5);
		return (WinValue);
	}

	//3 from left
	Ptr2WinValue = (line[0] | line[1] | line[2]);

	WinValue = SymbolTablePtr3[mWinningSymbol[Ptr2WinValue]];
	if(WinValue)
	{
		mSymsToAnimate[0] = 5 * r1;
		mSymsToAnimate[1] = 1 + (5 * r2);
		mSymsToAnimate[2] = 2 + (5 * r3);
		mSymsToAnimate[3] = -1;
		mSymsToAnimate[4] = -1;
		return (WinValue);
	}

	//3 from right
	Ptr2WinValue = (line[2] | line[3] | line[4]);

	WinValue = SymbolTablePtr3[mWinningSymbol[Ptr2WinValue]];
	if(WinValue)
	{
		mSymsToAnimate[0] = -1;
		mSymsToAnimate[1] = -1;
		mSymsToAnimate[2] = 2 + (5 * r3);
		mSymsToAnimate[3] = 3 + (5 * r4);
		mSymsToAnimate[4] = 4 + (5 * r5);
		return (WinValue);
	}

	//3 from middle
	Ptr2WinValue = (line[1] | line[2] | line[3]);

	WinValue = SymbolTablePtr3[mWinningSymbol[Ptr2WinValue]];

	if(WinValue)
	{
		mSymsToAnimate[0] = -1;
		mSymsToAnimate[1] = 1 + (5 * r2);
		mSymsToAnimate[2] = 2 + (5 * r3);
		mSymsToAnimate[3] = 3 + (5 * r4);
		mSymsToAnimate[4] = -1;
	}
	return (WinValue);
}