#include "PokerGame.h"
#include <ObjectHandler.h>

void PokerGame::HoldCardButtons(void)
{

	if (GET_BUTTONS->OSButtonPressed("HoldInfoButton") || ReadCardHoldPb(0))
	{
		if (!PokerHand[0].hold)
			PokerHand[0].hold = 1;
		else
			PokerHand[0].hold = 0;		
	}
	else if (GET_BUTTONS->OSButtonPressed("Hold2Button") || ReadCardHoldPb(1))
	{
		if (!PokerHand[1].hold)
			PokerHand[1].hold = 1;
		else
			PokerHand[1].hold = 0;
	}
	else if (GET_BUTTONS->OSButtonPressed("Hold3Button") || ReadCardHoldPb(2))
	{
		if (!PokerHand[2].hold)
			PokerHand[2].hold = 1;
		else
			PokerHand[2].hold = 0;
	}
	else if (GET_BUTTONS->OSButtonPressed("Hold4Button") || ReadCardHoldPb(3))
	{		
		if (!PokerHand[3].hold)
			PokerHand[3].hold = 1;
		else
			PokerHand[3].hold = 0;
	}
	else if (GET_BUTTONS->OSButtonPressed("HoldTransferButton") || ReadCardHoldPb(4))
	{		
		if (!PokerHand[4].hold)
			PokerHand[4].hold = 1;
		else
			PokerHand[4].hold = 0;
	}	
}

void PokerGame::HoldCalc(unsigned char *DealtHand)
{
unsigned char HighestCard,Match;
unsigned char MatchCard;//RL1208

	PotentialWin = 0;
	if(ValueOfPair == 14)//only 13 cards position 14 used to rank ace highest
		ValueOfPair = 1;

	/* ROYAL OR STRAIGHT FLUSH */
	if(MaxRun==5&&MaxFlush==5)
	{
		/* add code for royal/straight flush */
		if(StartOfStraight == 10)
		{
			for (i=0; i < 5; i++)
				SetRHS(i);
			PotentialWin = ROYAL_FLUSH;
		}
		else
		{
			for (i=0; i < 5; i++)
				SetRHS(i);
			PotentialWin = STRAIGHT_FLUSH;
		}
	}
	/* FIVE OF A KIND */
	else if(AKind5)
	{
		for (i=0; i < 5; i++)
			SetRHS(i);
		PotentialWin = AKIND5;
	}
	/* FOUR OF A KIND */
	else if(AKind4)
	{
		for(i=0;i<5;i++)
		{
			if(GetRawCard(DealtHand[i])!=AKind[4] && !CardsUsedArray[i])
			;//	DealtHand[i]=0;
			else
			{
				SetRHS(i);
				PotentialWin = AKIND4;
			}
		}
	}
	/* FULL HOUSE */
	else if(AKind3 && AKind2)
	{
		for (i=0; i < 5; i++)
			SetRHS(i);
		PotentialWin = FULL_HOUSE;
	}
	/* FLUSH */
	else if(MaxFlush == 5)
	{
		for (i=0; i < 5; i++)
			SetRHS(i);
		PotentialWin = FLUSH;
	}
	/* STRAIGHT */
	else if(MaxRun == 5)
	{
		for (i=0; i < 5; i++)
			SetRHS(i);
		PotentialWin = STRAIGHT;
	}
	/* THREE OF A KIND */
	else if(AKind3)
	{
		/*save win hand reset non used cards */
		for(i=0;i<5;i++)
		{
			//MAKE SURE ACE CODE == 1 AND NOT 14//R:1208
			if(AKind[3]==14)		//RL1208
				MatchCard = 1;
			else
				MatchCard = AKind[3];

			if(GetRawCard(DealtHand[i])!=MatchCard && !CardsUsedArray[i])
				;//	DealtHand[i]=0;
			else
			{
				SetRHS(i);
				PotentialWin = AKIND3;
			}
		}
	}
	/* TWO PAIRS */
	else if(NumberPairs==2)
	{
		for(i=0;i<5;i++)
		{
			if(AlikeMask[GetRawCard(DealtHand[i])]!=2 && !CardsUsedArray[i] )
			;//	DealtHand[i]=0;
			else
			{
				SetRHS(i);
				PotentialWin = TWO_PAIR;
			}
		}
	}
	else if(NumberPairs==1 && (ValueOfPair >= 11 || ValueOfPair == 1))
	{
		for(i=0;i<5;i++)
		{
			if(GetRawCard(DealtHand[i]) != ValueOfPair && !CardsUsedArray[i])
			{
			;//	DealtHand[i]=0;
			}
			else
			{
				SetRHS(i);
				//PotentialWin = ONE_PAIR;
			}
		}
	}
	else
	{
		if(MaxRun == 4)
		{
			unsigned char found,pair_card_used;
			pair_card_used = 0;
			for(i=0;i<5;i++)
			{
				found = 0;
				for(j=0;j<RunOf[RunPtr][1][0];j++)
				{
					Match = GetRawCard(DealtHand[i]) ;
					if(RunOf[RunPtr][1][1] == 11 && Match == 1)//A is high for JQKA
						Match = 14;
					if(Match  == RunOf[RunPtr][1][1]+j)
						found = 1;
				}
				if(Match == ValueOfPair)	//cant hold_low pair
				{
					pair_card_used++;
					if(pair_card_used > 1)
						found = 0;
				}

				if(!found && !CardsUsedArray[i])
				;//	DealtHand[i]=0;
				else
				{
					SetRHS(i);
					PotentialWin = STRAIGHT;
				}
			}
		}
		else if(MaxFlush==4)
		{
			for(i=0;i<5;i++)
			{
				if(GetSuit(DealtHand[i]) != FlushSuit && !CardsUsedArray[i])
				{
				;//	DealtHand[i]=0;
				}
				else
				{
					SetRHS(i);
					PotentialWin = FLUSH;
				}
			}
		}
		else if(NumberPairs==1)//hold any pair
		{
			for(i=0;i<5;i++)
			{
				if(AlikeMask[GetRawCard(DealtHand[i])]!=2 && !CardsUsedArray[i])
				{
				;//	DealtHand[i]=0;
				}
				else
				{
					SetRHS(i);
					PotentialWin =  AKIND3;
				}

			}
		}
		else if (0) // takes away hold highest
		{
			if(NumberPairs == 0)
			{
				/* cant be a pair so find highest card and hold it */
				HighestCard = 0;
				for(i=0;i<5;i++)
				{
					if(!CardsUsedArray[i])
					{
						if((GetRawCard(DealtHand[i])>HighestCard||GetRawCard(DealtHand[i])==1)&&HighestCard != 1)
							HighestCard = GetRawCard(DealtHand[i]);
					}
				}
				for(i=0;i<5;i++)
				{
					if((GetRawCard(DealtHand[i])!=1&&GetRawCard(DealtHand[i])<11 && !CardsUsedArray[i]) || GetRawCard(DealtHand[i])!= HighestCard)
					;//	DealtHand[i] = 0;
					else
					{
						SetRHS(i);
						//PotentialWin = ONE_PAIR;
					}
				}
			}
		}
	}
}

void PokerGame::SetRHS(unsigned char i)
{
	PokerHand[i].hold = 1;
}

void PokerGame::TagHeldCards(void)
{
	if (PokerHand[0].hold)
		DealHand[0] |= 0x40;
	else
		DealHand[0] = 0x00;
 
	if (PokerHand[1].hold)
		DealHand[1] |= 0x40;
	else
		DealHand[1] = 0x00;

	if (PokerHand[2].hold)
		DealHand[2] |= 0x40;
	else
		DealHand[2] = 0x00;

	if (PokerHand[3].hold)
		DealHand[3] |= 0x40;
	else
		DealHand[3] = 0x00;

	if (PokerHand[4].hold)
		DealHand[4] |= 0x40;
	else
		DealHand[4] = 0x00;

}

int PokerGame::FindNumHeldCards(void)
{
int ctr=0;

	for (int i=0;i<5;i++)
	{
		if (PokerHand[i].hold)
			ctr++;
	}
	return(ctr);
}

void PokerGame::UpdatePokerHelds(void)
{
unsigned int Id,i;

	for (i=0; i<5; i++)
	{
		Id = TheObjectHandler::Instance()->GetObject2D("Held01")->GetID();
		if (PokerHand[i].hold)
			TheObjectHandler::Instance()->GetObject2D(Id+i)->SetVisible(true);
		else
			TheObjectHandler::Instance()->GetObject2D(Id+i)->SetVisible(false);
	}
}

void PokerGame::SetHoldStartLamps(void)
{	

	if (PokerHand[4].hold)		
		GET_BUTTONS->SetOSButtonActivity(true, "HoldTransferButton",LAMP_ON);		 
	else
		GET_BUTTONS->SetOSButtonActivity(true, "HoldTransferButton",LAMP_FLASH); 

	if (PokerHand[3].hold)		
		GET_BUTTONS->SetOSButtonActivity(true, "Hold4Button",LAMP_ON);		 
	else
		GET_BUTTONS->SetOSButtonActivity(true, "Hold4Button",LAMP_FLASH); 
		
	if (PokerHand[2].hold)
		GET_BUTTONS->SetOSButtonActivity(true, "Hold3Button",LAMP_ON);		 
	else
		GET_BUTTONS->SetOSButtonActivity(true, "Hold3Button",LAMP_FLASH); 
		
	if (PokerHand[1].hold)		
		GET_BUTTONS->SetOSButtonActivity(true, "Hold2Button",LAMP_ON);		 
	else
		GET_BUTTONS->SetOSButtonActivity(true, "Hold2Button",LAMP_FLASH); 
		
	if (PokerHand[0].hold)		
		GET_BUTTONS->SetOSButtonActivity(true, "HoldInfoButton",LAMP_ON);		 
	else
		GET_BUTTONS->SetOSButtonActivity(true, "HoldInfoButton",LAMP_FLASH); 
		
}

bool PokerGame::CardHoldPb(int button)
{
bool Picked=false;

	Object2D* allCards = TheObjectHandler::Instance()->GetObject2D("Cards");
	float CardX = allCards->GetInstance(button)->GetPosition().x;
	float CardY = allCards->GetInstance(button)->GetPosition().y;
	float CardWidth = 235;
	float CardHeight= 264;

	RECT rect = {(long)CardX, (long)CardY, (long)(CardX+CardWidth), (long)(CardY+CardHeight)};

//	if (TheInput::Instance()->Pick2D(D3DXVECTOR2(CardX,CardY),rect))						
//		Picked = true;

	return(Picked);
}

bool PokerGame::ReadCardHoldPb(int button)
{
	if (RegisterCardBtnArray[button])
	{		
		RegisterCardBtnArray[button] = false;
		return(true);
	}
	return(false);
}

void PokerGame::RegisterCardButtons(void)
{
unsigned char i;

	for (i=0; i<5; i++)
	{
		if (CardHoldPb(i) && GetActiveCardButton(i))
		{
			TheAudioManager::Instance()->GetAudioSample("CLICK")->Play();
			RegisterCardBtnArray[i] = true;
			return;
		}
	}
}

void PokerGame::InitializeCardButtonArray(void)
{
	for (i=0; i<5; i++)
	{
		RegisterCardBtnArray[i] = false;
		SetActiveCardButton(false,i);
		if (i<2)
			SetActiveCollectBoxButton(false,i);
	}

}

void PokerGame::SetActiveCardButton(bool State, int Button)
{
	CardButtonActive[Button] = State;
}

bool PokerGame::GetActiveCardButton(int Button)
{
	return(CardButtonActive[Button]);
}