#include "PokerGame.h"
#include <ObjectHandler.h>

unsigned int PokerGame::Chkwin(unsigned char * dealt_hand, unsigned char hand_type)
{
 unsigned char JkrAvail;

	RoyalFlushWinFlag=0;

	RoutCond = 0;
	ChkWinPay = 0;

	RC1 = GetRawCard(dealt_hand[0]); 
	RC2 = GetRawCard(dealt_hand[1]);
	RC3 = GetRawCard(dealt_hand[2]);
	RC4 = GetRawCard(dealt_hand[3]);
	RC5 = GetRawCard(dealt_hand[4]);

	SC1 = GetSuit(dealt_hand[0]);
	SC2 = GetSuit(dealt_hand[1]);
	SC3 = GetSuit(dealt_hand[2]);
	SC4 = GetSuit(dealt_hand[3]);
	SC5 = GetSuit(dealt_hand[4]);

	JokerMem = 0;

	for(i=0;i<5;i++)
	{
		SuitCardMask[i] = 0;
		ValueCardMask[i] = 0;
		RunOf[i][1][1]=0;
		CardsUsedArray[i]=0;
	}
	for(i=0;i<10;i++)
	{
		RunOf[i][1][0]=0;
		RunOf[i][1][1]=0;
	}

	if(SC1 == 4)/*joker*/
	{
		SuitCardMask[0] = 0x1f;
		JokerMem++;
		CardsUsedArray[0]=dealt_hand[0];
	}
	else
	{
		SuitCardMask[0] = (1<<SC1);
		ValueCardMask[0] = (1<<RC1);
		if(RC1==1)
			ValueCardMask[0] |= (0x4000);
	}

	if(SC2 == 4)/*joker*/
	{
		JokerMem++;
		SuitCardMask[1] = 0x1f;
		CardsUsedArray[1]=dealt_hand[1];
	}
	else
	{
		SuitCardMask[1] = (1<<SC2);
		ValueCardMask[1] = (1<<RC2);
		if(RC2==1)
			ValueCardMask[1] |= (0x4000);
	}

	if(SC3 == 4)/*joker*/
	{
		JokerMem++;
		SuitCardMask[2] = 0x1f;
		CardsUsedArray[2]=dealt_hand[2];
	}
	else
	{
		SuitCardMask[2] = (1<<SC3);
		ValueCardMask[2] = (1<<RC3);
		if(RC3==1)
			ValueCardMask[2] |= (0x4000);
	}

	if(SC4 == 4)/*joker*/
	{
		JokerMem++;
		SuitCardMask[3] = 0x1f;
		CardsUsedArray[3]=dealt_hand[3];
	}
	else
	{
		SuitCardMask[3] = (1<<SC4);
		ValueCardMask[3] = (1<<RC4);
		if(RC4==1)
			ValueCardMask[3] |= (0x4000);
	}

	if(SC5 == 4)/*joker*/
	{
		JokerMem++;
		SuitCardMask[4] = 0x1f;
		CardsUsedArray[4]=dealt_hand[4];
	}
	else
	{
		SuitCardMask[4] = (1<<SC5);
		ValueCardMask[4] = (1<<RC5);
		if(RC5==1)
			ValueCardMask[4] |= (0x4000);
	}

/*	check for flush */


	if(SuitCardMask[0]&SuitCardMask[1]&
	   SuitCardMask[2]&SuitCardMask[3]&SuitCardMask[4])
	{
		Flush = 1;
		MaxFlush = 5;
	}
	else
	{
		/* add code to give max number of same suit */
		Flush = 0;
		for(i=0;i<5;i++)
			FlushCount[i]=0;
		for(i=0;i<5;i++)
			for(j=0;j<5;j++)
			{
				if(SuitCardMask[j]&(1<<i))
					FlushCount[i]++;
			}
		MaxFlush = 0;
		for(i=0;i<5;i++)
		{
			if(FlushCount[i] > MaxFlush)
			{
				MaxFlush = FlushCount[i];
				FlushSuit = i;
			}
		}
	}

/*check for x alike*/
	for(i=0;i<15;i++)
		AlikeMask[i] = 0;

	for(i=0;i<6;i++)
		AKind[i] = 0;

	for(j=0;j<5;j++)
		for(i=1;i<15;i++)
		{
			if(ValueCardMask[j]&(1<<i))
				AlikeMask[i]++;
		}

	for(i=3;i<6;i++)
	{
		for(j=1;j<14;j++)
			if(AlikeMask[j]==i)
				AKind[i]=j;
	}
	JokerPrime = JokerMem;
	AKind2=AKind3=AKind4=AKind5=NumberPairs = ValueOfPair = 0;

	do
	{
		if(AKind[4])
		{
			if(JokerPrime)
			{						
				JokerPrime--;		 //RJ 5/4/09 second joker was joker_prime = 0
				AKind5 = 1;
				AKind[5] = AKind[4]; //RJ 5/4/09 second joker
				AKind[4] = 0;		 //RJ 5/4/09 second joker
			}
			else
			{
				AKind4 = 1;
			}
		}
		else if(AKind[3])
		{
			if(JokerPrime)
			{
				JokerPrime--;			//RJ 5/4/09 second joker was joker_prime = 0
				AKind4 = 1;
				AKind[4] = AKind[3];
				AKind[3] = 0;			//RJ 5/4/09 second joker
			}
			else
			{
				AKind3 = 1;
			}

		}
		else 
			break;
	}while(JokerPrime); //RJ 5/4/09 loop added, second joker

	/* check 2 alike or 3 alike with joker*/
	j=0;
	for(i=14;i>1;i--)
	{
		if(AlikeMask[i]==2)
		{
			if(JokerPrime)
			{
				JokerPrime--;			//RJ 5/4/09 second joker was joker_prime = 0
				AKind3 = 1;
				/* added 14.08.01 RJ */				
				/*remove RL prevents full house if AA1010jkr reported AM 26/08/01*/
/*				if (i==14)
					i = 1; */
				AKind[3]=i;
				//RJ 20/10/04 added for second joker
                if (JokerPrime)
                {
                	JokerPrime--;
                    AKind4 = 1;
                    AKind[4] = i;
                    AKind3 = AKind[3] = 0;
                }
			}
			else
			{
				if(!(AKind3&&i==1))//added RL to achieve RG mod 
				{
					AKind2++;
//TRY					if(i==1||i >= 11)
						ValueOfPair = i;
					j++;
				}
			}
		}
	}
	if(JokerPrime)
	{
		if(/*AlikeMask[1]==1*/0)
		{
			j++;			
			JokerPrime = 0;
			ValueOfPair = 1;
		}
		else
		{
			for(i=14;i>1;i--)
			{
				if(AlikeMask[i]==1)
				{
					if(JokerPrime)
					{
						JokerPrime--;
                        //RJ 5/4/09 added for second joker
                        if (JokerPrime)
                        {
                        	JokerPrime--;
                            AKind3 = 1;
                            AKind[3] = i;
                        }
						else
						{
							j++;						
							ValueOfPair = i;
						}
					}
				}
			}

		}
	}
	NumberPairs = j;

/*check for run of 2 3 4 or 5*/
	WinPtr=0;
	RunCtr=0;
	HighestRun = 0;
	JkrUsed = 0;
	JkrAvail = JokerMem;
	for(i=1;i<18;i++)
	{
		//RJ 5/04/09 second Joker.
		if((AlikeMask[i]
		||(JokerMem && (RunCtr>1||(RunCtr&&(AlikeMask[i+1]||(JkrAvail==2 ? AlikeMask[i+2] : 0))))&& (JkrUsed < JokerMem)))
			&& i < 17 )//should be 16 to allow for JQKA + JOKER
		{
			RunCtr++;
			if(!AlikeMask[i])
			{
				JkrUsed++;
				if (JkrUsed < 2)
					JkrAvail--;
			}
		}
		else
		{
			if(RunCtr > 1)
			{
					RunOf[WinPtr][1][0] = RunCtr; /*number in straight*/
					RunOf[WinPtr][1][1] = i-RunCtr; /*start 0f straight*/
					WinPtr++;
			}

			if(RunCtr > HighestRun)
			{
				HighestRun = RunCtr;
				StartOfStraight = i-RunCtr;
			}

			RunCtr=0;
		}

	}
	/* print runs of*/
	MaxRun = 0;
	for(i=0;i<5;i++)
	{
		if(RunOf[i][1][0])
		{
			if(RunOf[i][1][0]>MaxRun)
			{
				MaxRun = RunOf[i][1][0];
				RunPtr = i;
			}
		}
	}

/*rank wins */
	if(ValueOfPair == 14)//only 13 cards position 14 used to rank ace highest
		ValueOfPair = 1;

	if(/*MaxRun*/HighestRun==5&&MaxFlush==5)
	{
		/* add code for royal/straight flush */
		if(StartOfStraight == 10||StartOfStraight == 11 )//11 if joker == 10 
		{
			ChkWinPay = get_pay(ROYAL_FLUSH);
			RoutCond = 1;
			RoyalFlushWinFlag = 1;
		}
		else
		{
			ChkWinPay = get_pay(STRAIGHT_FLUSH);
			RoutCond = 3;
		}
	}
	else if(AKind5)
	{
		if (HigherWins(dealt_hand))
			ChkWinPay = HigherWins(dealt_hand);
		else
			ChkWinPay = get_pay(AKIND5);
		RoutCond = 2;
	}
	else if(AKind4)
	{
		ChkWinPay = get_pay(AKIND4);
		RoutCond = 4;
	}
	else if(AKind3 && AKind2)
	{
		ChkWinPay = get_pay(FULL_HOUSE);
		RoutCond = 5;
	}
	else if(MaxFlush == 5)
	{
		ChkWinPay = get_pay(FLUSH);
		RoutCond = 6;
	}
	else if(MaxRun == 5)
	{
		ChkWinPay = get_pay(STRAIGHT);
		RoutCond = 7;
	}
	else if(AKind3)
	{
		ChkWinPay = get_pay(AKIND3);
		RoutCond = 8;
	}
	else if(NumberPairs==2)
	{
		ChkWinPay = get_pay(TWO_PAIR);
		RoutCond = 9;
	}
/** No Jacks or Better in this game
	else if ((NumberPairs==1) && (ValueOfPair >= 11 || ValueOfPair == 1))
	{
		#ifdef PRINT
		printf("1 Pair of %02d\n",ValueOfPair);
		#endif
		ChkWinPay = get_pay(ONE_PAIR);
		RoutCond = 10;
	}
*/
	return(ChkWinPay);
}
/*********************************************************************/
/*returns value 1-13*/
//returns the numerical value of a card. This function is to be used
//when not drawing a card on a screen.
unsigned char PokerGame::GetRawCard(unsigned char CardValue)
{
	if (CardValue&0x40)
		CardValue &= ~0x40;

	do{
		if(CardValue > 13)
			CardValue -=13;
	}while(CardValue > 13);
	return(CardValue);
}
/*********************************************************************/
/*returns value 0 - 4 Club,Heart,Spade,Diamond,Joker */

unsigned char PokerGame::GetSuit(unsigned CardValue)
{
unsigned char suit;

	if (CardValue&0x40)
		CardValue &= ~0x40;

	suit = 0;
	do{
		if(CardValue > 13)
		{
			CardValue -=13;
			suit++;
		}
	}while(CardValue > 13);

	return(suit);
}
/*********************************************************************/
unsigned int PokerGame::get_pay(unsigned char win_type)
{
 unsigned int AwardValue;

	AwardValue = AwardTable[GameIndex][win_type-1];
	return(AwardValue);
}
/*********************************************************************/
unsigned int PokerGame::HigherWins(unsigned char *CardHand)
{
unsigned char i;

	for (i=0;i<5;i++)
	{
		if ((CardHand[i]&0x3f) == 13)
			return(AwardTable[GameIndex][11]);
		else if ((CardHand[i]&0x3f) == 12)
			return(AwardTable[GameIndex][10]);
		else if ((CardHand[i]&0x3f) == 11)
			return(AwardTable[GameIndex][9]);
	}
	return(0);
}
/*********************************************************************/