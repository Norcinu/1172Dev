#include "PokerGame.h"
#include <ObjectHandler.h>

const signed long HiloStoreMaxValue[MaxSelectableGames]		    =  { 400000,  800000};
	  signed long HiloStoreMinValue[MaxSelectableGames]			=  {-400000, -800000};
      signed long StandardStoreLimitValue[MaxSelectableGames]   =  { 180000,  360000};
const signed long ModeStoreOffset[MaxSelectableGames]			=  {    140,     280};

const signed long HiloStoreInitValue[MaxSelectableGames]		 = {-300000, -600000};
const signed long StandardStoreInitValue[MaxSelectableGames]	 = {-200000, -400000};
const signed long ModeStoreInitValue[MaxSelectableGames]		 = {      0,       0};
const signed long HighStoreInitValue[MaxSelectableGames]		 = {      0,       0};

unsigned char IgnoreModeStore[MaxSelectableGames];
unsigned int  ModeGameInCtr[MaxSelectableGames] ={0,0};
unsigned int  MaxModeGameInCtr[MaxSelectableGames] = {10,10};

void PokerGame::ReflexIn(void)
{
	Rin2();
}

void PokerGame::ReflexOut(void)
{
 unsigned int AmountToDeduct = 0;

	if (AllowHighWinFlag)  //Tkae from High Win Store
	{
		HighStore[GameIndex] -= Pay*10;  
	}
	else if (ModeGameIn[GameIndex])	    //needs to be able to go negative.
	{
		ModeStore[GameIndex] -= Pay*10;   //In this mode all wins taken from tc_store
	}
	else if(GambleReflexValue)  // new gamble only deducts start gamble entry value
	{
		AmountToDeduct = GambleReflexValue * 10;
		if(HiloStore[GameIndex] != HiloStore2[GameIndex])
			HiloStore[GameIndex] = HiloStore2[GameIndex] = HiloStoreInitValue[GameIndex];
		HiloStore[GameIndex] += GambleReflexValue*10;
		HiloStore2[GameIndex] += GambleReflexValue*10;

		if(Pay)
		{
			HiloStore[GameIndex] -= Pay*10;
			HiloStore2[GameIndex] -= Pay*10;
		}
	}
	else
	{
		AmountToDeduct = Pay * 10;//was 100 but Pay now in 1p units not 10p
	}

	if (!ModeGameIn[GameIndex])
		StandardStore[GameIndex] -= AmountToDeduct;

	GambleReflexValue = 0;
	
}

void PokerGame::Rin1(unsigned int Pcent)
{
 unsigned int CashAdd;

	CashAdd = Pcent;			      /*total to add for this game */

	if (GetLocalCharRandomNumber(100) < 5)
	{
		HighStore[GameIndex] += CashAdd;
		CashAdd = 0;
	}
	else if (GetLocalCharRandomNumber(100) < 10 && !IgnoreModeStore[GameIndex]) /* 10% of money in goes to tc_store */
	{
		ModeStore[GameIndex] += CashAdd;
		CashAdd = 0;
	}
	else if ((HiloStore[GameIndex] < HiloStoreMinValue[GameIndex]))
	{
		if (GetLocalCharRandomNumber(100) < 30)         /* 24% of money in goes to HiloStore */
		{
			HiloStore[GameIndex] += CashAdd;
			HiloStore2[GameIndex] += CashAdd;
			CashAdd = 0;
		}
	}
	else if ((HiloStore[GameIndex] < HiloStoreMaxValue[GameIndex]))
	{
		if (GetLocalCharRandomNumber(100) < 11)          /* 14% of money in goes to HiloStore */
		{
			HiloStore[GameIndex] += CashAdd;
			HiloStore2[GameIndex] += CashAdd;
			CashAdd = 0;
		}
	}

	if (CashAdd)
		StandardStore[GameIndex] += CashAdd;		      /* remainder goes to ordinary store */

}

void PokerGame::Rin2(void)
{
	float reflex;
#ifdef _BBOX
	reflex = (((float)GetTargetPercentage() - (float)GetBonusBoxPercentage()) / 100)*WageredAmount;
	Rin1((unsigned int)(reflex*10));
#else
	reflex = ((float)GetTargetPercentage()/(float)100)*WageredAmount;
	Rin1((unsigned int)(reflex*10));		
#endif
}

void PokerGame::Rinit(void)
{
	for (int i=0; i < MaxSelectableGames ; i++)
	{
		IgnoreModeStore[i] = 0;
		ModeGameIn[i] = STANDARD01_MODE;	
	}
    InitStats();
	InitialModeTriggerStore();		
}

void PokerGame::InitStats(void)
{
	for (int i=0; i < MaxSelectableGames ; i++)
	{
		ModeStore[i]	 = ModeStoreInitValue[i];
		StandardStore[i] = StandardStoreInitValue[i];
		HiloStore[i]     = HiloStore2[i] = HiloStoreInitValue[i];
		HighStore[i]     = HighStoreInitValue[i];
		GeneralStore[i]  = 0;
	}
}

unsigned char PokerGame::Rlinks(void)
{
	return(0);
}

unsigned int PokerGame::GetPc(void)
{
unsigned int Pcent;
	
	Pcent = GetTargetPercentage();
	return(Pcent);
}

unsigned char PokerGame::SetChanceTablePtrs(void)
{
 unsigned char ChanceTablePtr;
 signed long UpperLimit, LowerLimit;

	/* In Pence */
	UpperLimit = + StandardStoreLimitValue[GameIndex];
	LowerLimit = - StandardStoreLimitValue[GameIndex];

	ChanceTablePtr = 0;     //86% tables

	if (ModeGameIn[GameIndex])
		ChanceTablePtr = 3; // tables

	else if (StandardStore[GameIndex] < LowerLimit)
		ChanceTablePtr = 1; //60% tables

	else if (StandardStore[GameIndex] > UpperLimit)
		ChanceTablePtr = 2; //100% tables

	return(ChanceTablePtr);
}

void PokerGame::SelectModeGameIn(void)
{
	if (!ModeGameIn[GameIndex])
	{
		if (AllowFeatureModeEntry())
		{
			ModeGameIn[GameIndex] = STANDARD02_MODE;
			IgnoreModeStore[GameIndex] = 1;
		}
	}
	else
	{
		if (AllowFeatureModeExit())
		{
			ReallocateModeMoney();
			ModeGameIn[GameIndex] = IgnoreModeStore[GameIndex] = ModeGameInCtr[GameIndex] = 0;
			SetModeTriggerStore();
		}
	}	
}

void PokerGame::CountHyperModeGames(void)
{
	if (ModeGameIn[GameIndex])
		ModeGameInCtr[GameIndex]++;		
}

void PokerGame::SetPointerAndMode(void)
{	
	SelectModeGameIn();
	CountHyperModeGames();
	PercPtr = SetChanceTablePtrs();
}

void PokerGame::SetModeTriggerStore(void)
{	
	ModeStoreTrigger[GameIndex] = (ModeStoreOffset[GameIndex] + (GetLocalIntRandomNumber(10)+1))* 1000;
	ModeStoreExit[GameIndex]    = 0;
}

void PokerGame::InitialModeTriggerStore(void)
{
	for (int i=0; i < MaxSelectableGames ; i++)
	{
		ModeStoreTrigger[i] = (ModeStoreOffset[i] + (GetLocalIntRandomNumber(10)+1))* 1000;
		ModeStoreExit[i]    = 0;
	}
}

unsigned char PokerGame::AllowFeatureModeEntry(void)
{
unsigned char Allow = 0;

	if (ModeStore[GameIndex] >= ModeStoreTrigger[GameIndex])
		Allow = 1;
	return(Allow);
}

unsigned char PokerGame::AllowFeatureModeExit(void)
{
unsigned char Allow = 0;

	if (ModeStore[GameIndex] <= ModeStoreExit[GameIndex] || ModeGameInCtr[GameIndex] >= MaxModeGameInCtr[GameIndex])
		Allow = 1;
	return(Allow);
}

void PokerGame::ReallocateModeMoney(void)
{
	if (ModeStore[GameIndex] > 0)
	{
		StandardStore[GameIndex] += ModeStore[GameIndex];
		ModeStore[GameIndex] = 0;
	}
}		