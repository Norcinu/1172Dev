#include "PokerGame.h"
#include <ObjectHandler.h>

/* GENERATE FIVE OF A KIND HAND */
void PokerGame::Shuffle5KCards(unsigned char WinType)
{
unsigned char Kind,Suite1,Suite2,Suite3,Suite4;

	if (WinType == J_AKIND5)
		Kind = 11;
	else if (WinType == Q_AKIND5)
		Kind = 12;
	else if (WinType == K_AKIND5)
		Kind = 13;

	Suite1 = GetLocalCharRandomNumber(4);
	do
	{
		Suite2 = GetLocalCharRandomNumber(4);
	}while(Suite1 == Suite2);

	do
	{
		Suite3 = GetLocalCharRandomNumber(4);
	}while(Suite1 == Suite3 || Suite2 == Suite3);

    do
    {
    	Suite4 = GetLocalCharRandomNumber(4);
    }while(Suite4 == Suite1 || Suite4 == Suite2 || Suite4 == Suite3);

	TempCards[0] = Kind+(13*Suite1);
	TempCards[1] = Kind+(13*Suite2);
	TempCards[2] = Kind+(13*Suite3);
	TempCards[3] = Kind+(13*Suite4);
	TempCards[4] = 53;
    ShuffleTstCards(TempCards);
}

void PokerGame::ShuffleTstCards(unsigned char *TempCards)
{
unsigned char CardsShuffled,i,NextCard,NextPosition,Prev;

	for (i=0;i<5;i++)
		Tcards[i] = 0;
	CardsShuffled = 0;
	do{
		NextCard=GetLocalCharRandomNumber(5);
		do{
			if(!TempCards[NextCard])
			{
				NextCard++;
				if(NextCard > 4)
					NextCard = 0;
			}
			else
				break;
		}while(1);


		NextPosition=GetLocalCharRandomNumber(5);
		Prev = CardsShuffled;
		do{
			if(!Tcards[NextPosition])
			{
				Tcards[NextPosition] = TempCards[NextCard];
				CardsShuffled++;
				TempCards[NextCard]=0;
			}
			else
			{
				NextPosition++;
				if(NextPosition > 4)
					NextPosition = 0;
			}
		}while(CardsShuffled == Prev);

	}while(CardsShuffled < 5);
}

void PokerGame::ForceWin(unsigned char WinType)
{    	
	Shuffle5KCards(WinType);
}

void PokerGame::CreatePack(void)
{
unsigned char i,j;

	//store pack of cards
	for (i=0;i<53;i++)
    	Xcards[i] = Cards[i];

    //remove cards which make up the winning hand
	for (i=0;i<53;i++)
    {
    	for (j=0;j<5;j++)
        {
    		if (Xcards[i] == Tcards[j])
            {
            	Xcards[i] = 0;
                break;
            }
        }
    }

    //create new pack of cards, leaving first five cards blank
    j=5;
    for (i=0;i<53;i++)
    {
        if (Xcards[i])
        {
    		Cards[j] = Xcards[i];
        	j++;
        }
    }

    //replace blank cards with the winning hand
    for (i=0;i<5;i++)
    	Cards[i] = Tcards[i];

	Nswop = NumToSwop();

    SelectCardsToSwop();

    ExecuteSwop();
}

unsigned char PokerGame::NumToSwop(void)
{
	return(GetLocalCharRandomNumber(3));
}

void PokerGame::SelectCardsToSwop(void)
{
 unsigned char Numb;

    Numb = Nswop;
	SwopPos[0] = SwopPos[1] = SwopPos[2] = 255;

	if (Numb > 0)
		SwopPos[0] = GetLocalCharRandomNumber(5);

	if (Numb > 1)
	{
		do
		{
    		SwopPos[1] = GetLocalCharRandomNumber(5);
		}while(SwopPos[1] == SwopPos[0]);
	}

    if (Numb > 2)
    {
    	do
    	{
    		SwopPos[2] = GetLocalCharRandomNumber(5);
    	}while(SwopPos[2] == SwopPos[1] || SwopPos[2] == SwopPos[0]);
    }

}

void PokerGame::ExecuteSwop(void)
{
unsigned char sp1, sp2, i;

	if (Nswop)
	{
		for (i=0;i<Nswop;i++)
		{
    		sp1 = Cards[SwopPos[i]];
			sp2 = Cards[5+i];
			Cards[SwopPos[i]] = sp2;
			Cards[5+i] = sp1;
		}			
	}
}

unsigned char PokerGame::FindHoldPos(unsigned char Ele)
{
 unsigned char i, rtn;

	rtn = 1;
	for (i=0;i<3;i++)
	{
		if (SwopPos[i] == Ele)
			rtn = 0;
	}
	return(rtn);
}

unsigned char PokerGame::SelectWinType(void)
{
	if (GetLocalCharRandomNumber(100) < 80)
		return(J_AKIND5);
	else if (GetLocalCharRandomNumber(100) < 50)
		return(Q_AKIND5);
	else
		return(K_AKIND5);
}
