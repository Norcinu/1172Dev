#include "PokerGame.h"
#include <ObjectHandler.h>

void PokerGame::ShuffleCards(void)
{
unsigned char CardsShuffled,i,NextCard,NextPosition,Prev;

	CardPtr = 0;
	for(i=0;i<53;i++)
	{
		TempCards[i]=i+1;
		Cards[i] = 0;
	}
	CardsShuffled = 0;
	do{
		NextCard=GetLocalCharRandomNumber(53);
		do{
			if(!TempCards[NextCard])
			{
				NextCard++;
				if(NextCard > 52)
					NextCard = 0;
			}
			else
				break;
		}while(1);


		NextPosition=GetLocalCharRandomNumber(53);
		Prev = CardsShuffled;
		do{
			if(!Cards[NextPosition])	 	
			{
				Cards[NextPosition] = TempCards[NextCard];
				CardsShuffled++;
				TempCards[NextCard]=0;
			}
			else
			{
				NextPosition++;
				if(NextPosition > 52)
					NextPosition = 0;
			}			 
		}while(CardsShuffled == Prev);
			
	}while(CardsShuffled < 53);
}


void PokerGame::SelectFirstHand(void)
{
 unsigned char i,Card;

	HandPtr = CardPtr = 0;
	for(i=0;i<5;i++)
	{
		Card = Cards[CardPtr++];
		if(!AllowDoubleJokerFlag && Card == 53 && !GetLocalCharRandomNumber(3))
			Card = Cards[CardPtr++];
		DealHand[HandPtr++] = Card;
		if(Card > 53) HandPtr = 0;
	}
}

void PokerGame::DealFirstHand(void)
{
	for(i=0;i<5;i++)
		PutCard(i,DealHand[i]);
}

void PokerGame::SelectSecondHand(void)
{
 unsigned char i,Card;

	for(i=0;i<5;i++)
	{			
		if(!DealHand[i])
		{
			Card = SelectACard(DealHand);
			if(!AllowDoubleJokerFlag &&  Card == 53 && !GetLocalCharRandomNumber(3))			
				Card = SelectACard(DealHand);
			if(Card > 53) Card = 0;
			DrawHand[i] = Card;			
		}	
		else
			DrawHand[i]= DealHand[i];
	}
}

void PokerGame::DealSecondHand(void)
{
	for(i=0;i<5;i++)
	{
		if (!(DealHand[i]&0x40))
		{
			if (InsertDoubleJokerFlag && DrawHand[i] == JOKER_CARD)
				PutCard(i,GOLDEN_JOKER_CARD);
			else
				PutCard(i,DrawHand[i]);
		}		
	}	
}

unsigned char PokerGame::SelectACard(unsigned char *Hand)
{
 unsigned char i;

	for (i=0;i<5;i++)
	{
		if (Hand[i]== Cards[CardPtr])
		{
			CardPtr++;
			ReshuffleCards();
		}
	}
	return(Cards[CardPtr++]);
}

void PokerGame::ReshuffleCards(void)
{
	if (CardPtr > 53)
	{
		ShuffleCards();
		CardPtr = 0;
	}
}

unsigned char PokerGame::FindWinLevel(unsigned int LocalValue)
{
 unsigned char Level;

	for (Level = 0; Level < MAX_AWARDS; Level++)
	{
		if (LocalValue == AwardTable[GameIndex][Level])
			break;
	}
	if(Level == MAX_AWARDS)
		Level = 0;
	return(Level);
}

unsigned char PokerGame::WinInvolvingJoker(unsigned char *DealtHand,unsigned char Value)
{
 unsigned char SC[5], JokerPresent, i;

	JokerPresent = 0;

	if (Value)
	{		
		SC[0] = GetSuit(DealtHand[0]);
		SC[1] = GetSuit(DealtHand[1]);
		SC[2] = GetSuit(DealtHand[2]);
		SC[3] = GetSuit(DealtHand[3]);
		SC[4] = GetSuit(DealtHand[4]);
		
		for (i=0; i<5; i++)
		{
			if (SC[i] == JOKER)
			{
				JokerPresent = 1;	
				break;
			}	
		}
		
		if (JokerPresent && InsertDoubleJokerFlag)
			JokerPresent = 0;
	}
	return(JokerPresent);
}

unsigned char PokerGame::PokerHoldCondition(void)
{
	if (FindWinLevel(Pay) < MAX_LEVEL)
		return(1);
	else
		return(0);
}	

unsigned char PokerGame::PokerHiloCondition(void)
{
	if (Pay && FindWinLevel(Pay) < FunctionMaxLevels())
		return(1);
	else
		return(0);
}	









