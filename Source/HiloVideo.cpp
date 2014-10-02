#include "PokerGame.h"
#include <ObjectHandler.h>

void PokerGame::HiloPutCard(unsigned char Pos,unsigned char Card)
{
 unsigned char Tag;
	
	Pos %= NUM_CARDS_IN_HAND;

	if (!Card)
	{
		HiloHand[Pos].id    = 55;
		HiloHand[Pos].state = 0;
	}
	else
	{
		Tag = (Card&0xc0);
		Card &= 0x3f;
		HiloHand[Pos].id    = Card;
		HiloHand[Pos].state = Tag;
	}
	
}

void PokerGame::HiloRemoveCard(unsigned char Pos)
{
	HiloHand[Pos].id    = 0;
	HiloHand[Pos].state = 0;
}

void PokerGame::StartHiloGamble(void)
{
 
}

void PokerGame::ExitHiloGamble(void)
{
 unsigned char i;

	for (i=0;i<LEVEL;i++)
		SetAwardValueLitState(i,MLAMP_OFF);
	
	HiloGambleFlag = 0;
}

void PokerGame::DrawHiloCards(void)
{
	if (ActivateHiloGambleGraphics)
	{
		for (int i=0; i < NUM_CARDS_IN_HAND; i++)
		{
			Object2D* allCards = OBJECT_HANDLER->GetObject2D("Cards");						
			if (HiloHand[i].id)		
			{			
				allCards->GetInstance(i)->SetVisible(true);
				allCards->GetInstance(i)->SetCurrentSprite(ThePokerGame::Instance()->HiloHand[i].id - 1);				
			}
			else
				allCards->GetInstance(i)->SetVisible(false);
		}
	}
}

void PokerGame::DrawHiloState(void)
{
unsigned int StateId;

	if (ActivateHiloGambleGraphics)
	{

		for (int i = 0; i < NUM_CARDS_IN_HAND; i++)
		{
			StateId = OBJECT_HANDLER->GetObject2D("HI01")->GetID()+i;
			OBJECT_HANDLER->GetObject2D(StateId)->SetVisible(false);
			StateId = OBJECT_HANDLER->GetObject2D("LO01")->GetID()+i;
			OBJECT_HANDLER->GetObject2D(StateId)->SetVisible(false);

			if (HiloHand[i].state)
			{						 			
				if (HiloHand[i].state == HI_SELECTED)
				{
					StateId = OBJECT_HANDLER->GetObject2D("HI01")->GetID()+i;
					OBJECT_HANDLER->GetObject2D(StateId)->SetVisible(true);
				}
				else
				{
					StateId = OBJECT_HANDLER->GetObject2D("LO01")->GetID()+i;
					OBJECT_HANDLER->GetObject2D(StateId)->SetVisible(true);
				}
			}			
		}			
	}
}

void PokerGame::DrawRemoveHelds(void)
{
unsigned int StateId;

	if (ActivateHiloGambleGraphics)
	{

		for (int i=0; i < NUM_CARDS_IN_HAND; i++)
		{
			if (!PokerHand[i].hold)
			{
				StateId = OBJECT_HANDLER->GetObject2D("Held01")->GetID()+i;
				OBJECT_HANDLER->GetObject2D(StateId)->SetVisible(false);
			}
		}
	}
}
