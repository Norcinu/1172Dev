#include "PokerGame.h"
#include <ObjectHandler.h>

/***************************ROLL OFF TABLES********************************/
/**************************65 PERCENT PAYOUT*******************************/
unsigned char DealChance65[] =  { 55,	/* JACKS OR BETTER */
								  75,	/* TWO PAIRS */
								  75,	/* THREE OF A KIND */								
								  75,	/* STRAIGHT */
								  80,	/* FLUSH */
								  68,	/* FULL HOUSE */
								  80,	/* FOUR OF A KIND */
								  92,	/* STRAIGHT FLUSH */
								  92,	/* FIVE OF A KIND */
								   0};  /* ROYAL FLUSH */

unsigned char DrawChance65[] = { 20,	/* JACKS OR BETTER */
								  22,	/* TWO PAIRS */
								  22,	/* THREE OF A KIND */
								  25,	/* STRAIGHT */
								  30,	/* FLUSH */
								  35,	/* FULL HOUSE */
								  35,	/* FOUR OF A KIND */
								   0,	/* STRAIGHT FLUSH */
						     	   0,	/* FIVE OF A KIND */
								   0};  /* ROYAL FLUSH */

/***************************45 PERCENT PAYOUT******************************/
unsigned char DealChance45[] =  { 55,	/* JACKS OR BETTER */
								  75,	/* TWO PAIRS */
								  75,	/* THREE OF A KIND */								
								  75,	/* STRAIGHT */
								  80,	/* FLUSH */
								  68,	/* FULL HOUSE */
								  80,	/* FOUR OF A KIND */
								  92,	/* STRAIGHT FLUSH */
								  92,	/* FIVE OF A KIND */
								   0};  /* ROYAL FLUSH */

unsigned char DrawChance45[] = { 51,	/* JACKS OR BETTER */
								  50,	/* TWO PAIRS */
								  50,	/* THREE OF A KIND */
								  50,	/* STRAIGHT */
								  60,	/* FLUSH */
								  65,	/* FULL HOUSE */
								  65,	/* FOUR OF A KIND */
								   0,	/* STRAIGHT FLUSH */
						     	   0,	/* FIVE OF A KIND */
								   0};  /* ROYAL FLUSH */

/***************************75 PERCENT PAYOUT******************************/
unsigned char DealChance75[] =  { 55,	/* JACKS OR BETTER */
								   75,	/* TWO PAIRS */
								   75,	/* THREE OF A KIND */								
								   75,	/* STRAIGHT */
								   80,	/* FLUSH */
								   68,	/* FULL HOUSE */
								   80,	/* FOUR OF A KIND */
								   92,	/* STRAIGHT FLUSH */
								   92,	/* FIVE OF A KIND */
								    0}; /* ROYAL FLUSH */

unsigned char DrawChance75[] =  {   6,	/* JACKS OR BETTER */
								    8,	/* TWO PAIRS */
								    8,	/* THREE OF A KIND */
								    8,	/* STRAIGHT */
								   15,	/* FLUSH */
								   20,	/* FULL HOUSE */
								   20,	/* FOUR OF A KIND */
								    0,	/* STRAIGHT FLUSH */
						     	    0,	/* FIVE OF A KIND */
								    0}; /* ROYAL FLUSH */

/******************************100 PERCENT PAYOUT************************/
unsigned char DealChance100[] =  { 0,	/* JACKS OR BETTER */
								   0,	/* TWO PAIRS */
								   0,	/* THREE OF A KIND */								
								   0,	/* STRAIGHT */
								   0,	/* FLUSH */
								   0,	/* FULL HOUSE */
								   0,	/* FOUR OF A KIND */
								   0,	/* STRAIGHT FLUSH */
								   0,	/* FIVE OF A KIND */
								   0};  /* ROYAL FLUSH */

unsigned char DrawChance100[] =  { 0,	/* JACKS OR BETTER */
								   0,	/* TWO PAIRS */
								   0,	/* THREE OF A KIND */
								   0,	/* STRAIGHT */
								   0,	/* FLUSH */
								   0,	/* FULL HOUSE */
								   0,	/* FOUR OF A KIND */
								   0,	/* STRAIGHT FLUSH */
								   0,	/* FIVE OF A KIND */
								   0};  /* ROYAL FLUSH */
/**************************************************************************************/
unsigned char *DealChance[]= {DealChance65,DealChance45,DealChance75,DealChance100};
unsigned char *DrawChance[]= {DrawChance65,DrawChance45,DrawChance75,DrawChance100};
/**************************************************************************************/
void PokerGame::DealRoll(void)
{
 unsigned  int Win;
 unsigned  int TriesCtr;
 unsigned char DoubleJokerFlag=0;

	TriesCtr = 0;
	InitHandsArrays();
	
	ShuffleCards();

	if (AllowHighWinFlag)
	{    	
		ForceWin(AllowedHighWin);
		CreatePack();
		SelectFirstHand();
		Win = Chkwin(DealHand,0);
	}
	else
	{
		SelectFirstHand();
		Win = Chkwin(DealHand,0);

		if (Win)
		{
			if (!AllowWin(Win,DealChance[PercPtr],1,DoubleJokerFlag))
			{
				while(TriesCtr++ < MAX_TRIES)
				{				
					RollOffWin(DealHand,Win,1);
					Win = Chkwin(DealHand,0);
					if (!Win)
						break;
				}
			}

			if (TriesCtr == MAX_TRIES+1)
				Win = Win;
		}

/* DEBUG
		DealHand[0] = 10;
		DealHand[1] = 11;
		DealHand[2] = 12;
		DealHand[3] = 13;
		DealHand[4] = 53;
		//Win = Chkwin(DealHand,0);
*/
	}
}

/* Roll off undesired second hand wins */
void PokerGame::DrawRoll(void)
{
 unsigned int Win;
 unsigned int TriesCtr;
 unsigned char DoubleJokerFlag=0;

	TriesCtr = 0;
	
	if (AllowHighWinFlag)
	{
		SelectSecondHand();
		Win = Chkwin(DrawHand,1);		
	}
	else
	{
		SelectSecondHand();
		Win = Chkwin(DrawHand,1);

		if (Win)
		{
			if(!AllowWin(Win,DrawChance[PercPtr],2,DoubleJokerFlag))
			{
				while(TriesCtr++ < MAX_TRIES)
				{				
					RollOffWin(DrawHand,Win,2);
					Win = Chkwin(DrawHand,1);
					if (!Win)
						break;
				}
			}
		}

	}

	InsertDoubleJokerFlag = GoldenJokerFeature(Win);
}

unsigned char PokerGame::RollOffWin(unsigned char *DealtHand, unsigned int value, unsigned char HandNumber)
{
 unsigned char Element;
 unsigned char SValue;

	Element = MAX_ELEMENT;

	SValue = FindWinLevel(value);

	SValue = ConvertWinLevel2RollOffLevel(SValue);

	switch(SValue)
	{
		case 9: Element = RoyalFlushCard(DealtHand,HandNumber);    break;
		case 8: Element = FiveOfKindCard(DealtHand,HandNumber);   break;
		case 7: Element = StraightFlushCard(DealtHand,HandNumber); break;
		case 6: Element = FourOfKindCard(DealtHand,HandNumber);   break;
		case 5: Element = FullHouseCard(DealtHand,HandNumber);	   break;
		case 4: Element = FlushCard(DealtHand,HandNumber);		   break;
		case 3: Element = StraightCard(DealtHand,HandNumber);	   break;
		case 2: Element = ThreeOfKindCard(DealtHand,HandNumber);  break;
		case 1: Element = TwoPairCard(DealtHand,HandNumber);	   break;
		case 0: Element = JacksOrBetterCard(DealtHand,HandNumber);break;
	}

	if (Element < MAX_ELEMENT)
	{
		DealtHand[Element] = SelectACard(DealtHand);
	}

	return(Element);
}

/* finds an unheld card contributing to a royal flush win */
unsigned char PokerGame::RoyalFlushCard(unsigned char *DealtHand, unsigned char HandNumber)
{
 unsigned char Element,i;

	Element = MAX_ELEMENT;
	for (i=0;i<5;i++)
	{
		if (DealtHand[i] == 53)
		{
			Element = i;
			break;
		}
	}

	for(i=0;i<5 && Element == MAX_ELEMENT ;i++)
	{
		if(!CardsUsedArray[i])
		{
			if (HandNumber==1)
			{
				Element=i;
				break;
			}
			else
			{
				if (!(DealHand[i]&0x40))
				{
					Element = i;
					break;
				}
			}

		 }
	}
	return(Element);

}

/* finds an unheld card contributing to a five of a kind win */
unsigned char PokerGame::FiveOfKindCard(unsigned char *DealtHand, unsigned char HandNumber)
{
 unsigned char Element,i;

	Element = MAX_ELEMENT;
	for (i=0;i<5;i++)
	{
		if (DealtHand[i] == 53)
		{
			Element = i;
			break;
		}
	}

	for(i=0;i<5 && Element == MAX_ELEMENT ;i++)
	{
		if(!CardsUsedArray[i])
		{
			if (HandNumber==1)
			{
				Element=i;
				break;
			}
			else
			{
				if (!(DealHand[i]&0x40))
				{
					Element = i;
					break;
				}
			}

		 }
	}

	return(Element);

}

/* finds an unheld card contributing to a straight flush win */
unsigned char PokerGame::StraightFlushCard(unsigned char *DealtHand, unsigned char HandNumber)
{
 unsigned char Element,i;

	Element = MAX_ELEMENT;
	for (i=0;i<5;i++)
	{
		if (DealtHand[i] == 53)
		{
			Element = i;
			break;
		}
	}
	for(i=0;i<5 && Element == MAX_ELEMENT ;i++)
	{
		if(!CardsUsedArray[i])
		{
			if (HandNumber==1)
			{
				Element=i;
				break;
			}
			else
			{
				if (!(DealHand[i]&0x40))
				{
					Element = i;
					break;
				}
			}

		 }
	}


	return(Element);

}

/* finds an unheld card contributing to a four of a kind win */
unsigned char PokerGame::FourOfKindCard(unsigned char *DealtHand, unsigned char HandNumber)
{
 unsigned char Element, i;

	Element = MAX_ELEMENT;

	for (i=0;i<5;i++)
	{
		if (DealtHand[i] == 53)
		{
			Element = i;
			break;
		}
	}

	for(i=0;i<5 && Element == MAX_ELEMENT ;i++)
	{

		if(!CardsUsedArray[i])
		{
			if(GetRawCard(DealtHand[i])==AKind[4])
			{
				if (HandNumber==1)
				{
					Element=i;
					break;
				}
				else
				{
					if (!(DealHand[i]&0x40))
					{
						Element = i;
						break;
					}
				}

			}
		}

	}

	return(Element);

}

/* finds an unheld card contributing to a full house win */
unsigned char PokerGame::FullHouseCard(unsigned char *DealtHand, unsigned char HandNumber)
{
 unsigned char Element,i;

	Element = MAX_ELEMENT;
	for (i=0;i<5;i++)
	{
		if (DealtHand[i] == 53)
		{
			Element = i;
			break;
		}
	}
	for(i=0;i<5 && Element == MAX_ELEMENT ;i++)
	{
		if(!CardsUsedArray[i])
		{
			if (HandNumber==1)
			{
				Element=i;
				break;
			}
			else
			{
				if (!(DealHand[i]&0x40))
				{
					Element = i;
					break;
				}
			}

		 }
	}

	return(Element);

}

/* finds an unheld card contributing to a flush win */
unsigned char PokerGame::FlushCard(unsigned char *DealtHand, unsigned char HandNumber)
{
 unsigned char Element,i;

	Element = MAX_ELEMENT;
	for (i=0;i<5;i++)
	{
		if (DealtHand[i] == 53)
		{
			Element = i;
			break;
		}
	}

	for(i=0;i<5 && Element == MAX_ELEMENT ;i++)
	{
		if(!CardsUsedArray[i])
		{
			if (HandNumber==1)
			{
				Element=i;
				break;
			}
			else
			{
				if (!(DealHand[i]&0x40))
				{
					Element = i;
					break;
				}
			}

		 }
	}
	return(Element);

}

/* finds an unheld card contributing to a straight win */
unsigned char PokerGame::StraightCard(unsigned char *DealtHand, unsigned char HandNumber)
{
 unsigned char Element, i;

	Element = MAX_ELEMENT;
	for (i=0;i<5;i++)
	{
		if (DealtHand[i] == 53)
		{
			Element = i;
			break;
		}
	}

	for(i=0;i<5 && Element == MAX_ELEMENT ;i++)
	{
		if(!CardsUsedArray[i])
		{
			if (HandNumber==1)
			{
				Element=i;
				break;
			}
			else
			{
				if (!(DealHand[i]&0x40))
				{
					Element = i;
					break;
				}
			}

		 }
	}
	return(Element);

}

/* finds an unheld card contributing to a three of a kind win */
unsigned char PokerGame::ThreeOfKindCard(unsigned char *DealtHand, unsigned char HandNumber)
{
 unsigned char Element, i;

	Element = MAX_ELEMENT;
	for (i=0;i<5;i++)
	{
		if (DealtHand[i] == 53)
		{
			Element = i;
			break;
		}
	}
	for(i=0;i<5 && Element == MAX_ELEMENT;i++)
	{
		if(GetRawCard(DealtHand[i]) == AKind[3] && !CardsUsedArray[i])
		{
			if (HandNumber==1)
			{
				Element=i;
				break;
			}
			else
			{
				if (!(DealHand[i]&0x40))
				{
					Element = i;
					break;
				}
			}
		}
	}

	return(Element);

}

/* finds an unheld card contributing to a two pair win */
unsigned char PokerGame::TwoPairCard(unsigned char *DealtHand, unsigned char HandNumber)
{
 unsigned char Element, i;

	Element = MAX_ELEMENT;

	for (i=0;i<5;i++)
	{
		if (DealtHand[i] == 53)
		{
			Element = i;
			break;
		}
	}

	for(i=0;i<5 && Element == MAX_ELEMENT;i++)
	{
		if(AlikeMask[GetRawCard(DealtHand[i])] == 2 && !CardsUsedArray[i])
		{
			if (HandNumber==1)
			{
				Element=i;
				break;
			}
			else
			{
				if (!(DealHand[i]&0x40))
				{
					Element = i;
					break;
				}
			}
		}
	}

	return(Element);
}

/* finds an unheld card contributing to a jacks or better win */
unsigned char PokerGame::JacksOrBetterCard(unsigned char *DealtHand, unsigned char HandNumber)
{
 unsigned char Element, i;

	Element = MAX_ELEMENT;
	for (i=0;i<5;i++)
	{
		if (DealtHand[i] == 53)
		{
			Element = i;
			break;
		}
	}

	for(i=0;i<5 && Element == MAX_ELEMENT;i++)
	{
		if(GetRawCard(DealtHand[i]) == ValueOfPair && !CardsUsedArray[i])
		{
			if (HandNumber==1)
			{
				Element=i;
				break;
			}
			else
			{
				if (!(DealHand[i]&0x40))
				{
					Element = i;
					break;
				}
			}
		}
	}

	return(Element);
}

unsigned char PokerGame::AllowWin(unsigned int value,unsigned char *chance_table, unsigned char HandNumber, unsigned char flag)
{
 unsigned char Element, allow, rnd, che;

	allow = 0;

	if (flag)
	{
		allow = 1;
	}
	else
	{
		Element = FindWinLevel(value);

		Element = ConvertWinLevel2RollOffLevel(Element);

		if ((rnd = GetLocalCharRandomNumber(100)) >= (che = chance_table[Element]))
		{
			allow = 1;
		}
		if (allow && GetLocalCharRandomNumber(100) < 7)  //Historical B3 Modifier.
		{
			allow = 0;			
		}
	}		
	return(allow);
}

void PokerGame::InitHandsArrays(void)
{
 unsigned char i;

	for(i=0;i<5;i++)
	{
		DealHand[i] = DrawHand[i] = 0;
	}
}

void PokerGame::InsertJoker(unsigned char value, unsigned char *DealtHand)
{
 unsigned char i;	
 unsigned char card_value;

	for (i=0; i<5; i++)
	{
		if(!(DealtHand[i]&0x40))
		{
			card_value = DealtHand[i];
			DealtHand[i] = 53; /* add joker */
			if (value <= Chkwin(DealtHand,0))
				break;
			else
				DealtHand[i] = card_value;
		}
	}
}

unsigned char PokerGame::FindDoubleJokerWins(unsigned char *DealtHand)
{
	unsigned char Ctr = 0;
	for (unsigned char i=0; i<5; i++)
	{
		if((DealtHand[i]&~0x40) == 53)
			Ctr++;
	}
	if (Ctr == 2)
		return(1);
	else
		return(0);
}

unsigned char PokerGame::CheckForJokerCard(unsigned char *DealtHand)
{
	for (unsigned char i=0; i<5; i++)
	{
		if ((DealtHand[i]&~0x40) == JOKER_CARD)
			return(1);
	}
	return(0);
}

void PokerGame::PlaceOneOfDoubleJoker(unsigned char *DealtHand)
{
unsigned char Counter = 0;
unsigned char CardPos,StoreCard;

	for (i = 0; i<5; i++)
	{
		if (GoldenJokerPositions[i] >= 0)
			Counter++;
	}

	if (Counter)
	{
		if (Counter > 1)
			CardPos = GoldenJokerPositions[GetLocalCharRandomNumber(Counter)];
		else
			CardPos = GoldenJokerPositions[0];

		StoreCard = DealtHand[CardPos];
		DealtHand[CardPos] = JOKER_CARD;

		for (i = CardPtr; i < 53; i++)
		{
			if (Cards[i] == JOKER_CARD)
			{
				Cards[i] = StoreCard;
				break;
			}
		}
	}	
}

unsigned char PokerGame::ConvertWinLevel2RollOffLevel(unsigned char Element)
{
	switch(Element)
	{
		case  9: Element = 8; break;
		case 10: Element = 8; break;
		case 11: Element = 8; break;
		case 12: Element = 9; break;
		default:			  break;
	}
	return(Element);
}


