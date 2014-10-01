#include "PokerGame.h"
#include <ObjectHandler.h>

/*                             HIGHER         1  2  3  4  5  6  7  8  9 10 11 12 13 */
const 	unsigned char WwHiChance[]       = {0,10,10,10, 4, 4, 4, 4, 4,13,12,12,13,EOT};
const 	unsigned char WlHiChance[]       = {0, 0, 5,10,14,14,14,14,14,10, 5, 0, 0,EOT};
const   unsigned char LoseHiChance[]     = {0, 9, 9, 9, 9, 8, 8, 8, 8, 8, 8, 8, 8,EOT};


/*                             LOWER         1  2  3  4  5  6  7  8  9 10 11 12 13 */
const 	unsigned char WwLoChance[]=      {13,12,12,13, 4, 4, 4, 4, 4,10,10,10, 0,EOT};
const 	unsigned char WlLoChance[]=      { 0, 0, 5,10,14,14,14,14,14,10, 5, 0, 0,EOT};
const   unsigned char LoseLoChance[]=    { 0, 8, 8, 8, 8, 8, 8, 8, 8, 9, 9, 9, 9,EOT};

const 	unsigned char HiloThrowTable[]=  { 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13};

/***					                   1  2  3  4  5  6  7  8  9 10 11 12 13 */
const   unsigned char WinSwapChance[] = {8, 8, 8, 8, 8, 5, 5, 5, 5,10,10,10,10};
const   unsigned char LoseSwapChance[]= {0, 0, 8, 8, 9,13,12,12,13, 9, 8, 8, 0};

unsigned char PokerGame::DecHiWinWin(unsigned char CardValue)
{
 unsigned char NewValue, TryCtr;

	TryCtr = 0;
	do
	{
		NewValue = HiloThrowTable[GetChoice(WwHiChance)];
		TryCtr++;
	}while(NewValue <= CardValue && TryCtr < MAX_TRIES);
	if (CardValue == NewValue)
	{
		NewValue++;
		if (NewValue > 13)
			NewValue -= 13;
	}
	return(NewValue);

}

unsigned char PokerGame::DecLoWinWin(unsigned char CardValue)
{
 unsigned char NewValue, TryCtr;

	TryCtr = 0;
	do
	{
		NewValue = HiloThrowTable[GetChoice(WwLoChance)];
		TryCtr++;
	}while(NewValue >= CardValue && TryCtr < MAX_TRIES);
	if (CardValue == NewValue)
		NewValue--;
	return(NewValue);

}

unsigned char PokerGame::DecHiWinLose(unsigned char CardValue)
{
 unsigned char NewValue, TryCtr;

	TryCtr = 0;
	do
	{
		NewValue = HiloThrowTable[GetChoice(WlHiChance)];
		TryCtr++;
	}while(NewValue <= CardValue && TryCtr < MAX_TRIES);
	if (CardValue == NewValue)
	{
		if (NewValue < 11)
			NewValue++;
	       else
			NewValue--;
	}
	return(NewValue);

}

unsigned char PokerGame::DecLoWinLose(unsigned char CardValue)
{
 unsigned char NewValue, TryCtr;

	TryCtr = 0;
	do
	{
		NewValue = HiloThrowTable[GetChoice(WlLoChance)];
		TryCtr++;
	}while(NewValue >= CardValue && TryCtr < MAX_TRIES);
	if (CardValue == NewValue)
	{
		if (NewValue > 3)
			NewValue--;
		else
		{
			NewValue++;
			if (NewValue > 13)
				NewValue -= 13;
		}
	}
	return(NewValue);

}

unsigned char PokerGame::DecLoseHi(unsigned char CardValue)
{
 unsigned char NewValue, TryCtr;

	TryCtr = 0;
	do
	{
		NewValue = HiloThrowTable[GetChoice(LoseHiChance)];
		TryCtr++;
	}while(NewValue >= CardValue && TryCtr < MAX_TRIES);
	if (NewValue == CardValue)
	{
		if (NewValue > 3)
			NewValue--;
		else
		{
			NewValue++;
			if (NewValue > 13)
				NewValue -= 13;
		}
	}
	return(NewValue);

}

unsigned char PokerGame::DecLoseLo(unsigned char CardValue)
{
 unsigned char NewValue, TryCtr;

	TryCtr = 0;
	do
	{
		NewValue = HiloThrowTable[GetChoice(LoseLoChance)];
		TryCtr++;
	}while(NewValue <= CardValue && TryCtr < MAX_TRIES);
	if (NewValue == CardValue)
	{
		NewValue++;
		if (NewValue > 13)
			NewValue -= 13;
	}
	return(NewValue);

}

unsigned char PokerGame::DecWinSwap(unsigned char CardValue)
{
 unsigned char NewValue;
	do
	{
		NewValue = HiloThrowTable[GetChoice(WinSwapChance)];
	}while(NewValue == CardValue);
	return(NewValue);
}

unsigned char PokerGame::DecLoseSwap(unsigned char CardValue)
{
 unsigned char NewValue;
	do
	{
		NewValue = HiloThrowTable[GetChoice(LoseSwapChance)];
	}while(NewValue == CardValue);
	return(NewValue);
}

unsigned char PokerGame::DecInitialCardValue(void)
{
 unsigned char Value;


	if (HiloRepeatTab[0] && HiloRepeatTab[1])
	{
		if (GetLocalCharRandomNumber(2))
			Value =  HiloThrowTable[GetChoice(WwHiChance)];
		else
			Value =  HiloThrowTable[GetChoice(WwLoChance)];
	}
	else
	{
		if (GetLocalCharRandomNumber(2))
			Value =  HiloThrowTable[GetChoice(WlHiChance)];
		else
			Value =  HiloThrowTable[GetChoice(WlLoChance)];
	}	
	return(Value);
}
/*****************************************************************************
 Pass this routine a table that is terminated with an EOT marker.
 The values in the table before the EOT must add up to EXACTLY 100.
 Each value is the % chance of that result being returned by this routine.
*/
unsigned char PokerGame::GetChoice(const unsigned char * tab)
{
unsigned char	i,TabNumber,RNV,RN;
unsigned char	Choice;

#ifdef MODEL_ACTIVE
unsigned char sum_total;

	sum_total = 0;
	for(i=0;*(tab+i) != EOT;i++)
	{
		sum_total += *(tab+i);
	}
	if(sum_total != 100)
	{
#ifndef SOAK
		printf("\nERROR in GetChoice(). Table with following values:\n");
		for(i=0;*(tab+i) != EOT;i++)
		{
			printf("%d ",*(tab+i));
		}
		printf("\nAdding up to %d\n ",sum_total);
#endif
	}
#endif

	Choice = 0;
	RNV = 100;
	for(i=0; *(tab+i) != EOT; i++)
	{
		TabNumber = *(tab+i);		// % chance for this item in the table
		RN = GetLocalCharRandomNumber(RNV);
		if(RN < TabNumber)
		{
			Choice = i;				// return a number 0 to 1 less than the EOT
			break;
		}
		RNV -= TabNumber;
	}

	return (Choice);
}
