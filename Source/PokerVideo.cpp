#include "PokerGame.h"
#include <ObjectHandler.h>

unsigned char FlashTog  = 0;

void PokerGame::PutCard(unsigned char Pos, unsigned char Card)
{	
	if (!Card)
	{
		PokerHand[Pos].id   =  55;
		PokerHand[Pos].hold =  0;
	}
	else
		PokerHand[Pos].id =  Card;
}

void PokerGame::RemoveCard(unsigned char Pos)
{
	PokerHand[Pos].id	=  0;
	PokerHand[Pos].hold =  0;
}

void PokerGame::ExitPokerGame(void)
{
	for (int i=0;i<LEVEL;i++)
		SetAwardValueLitState(i,MLAMP_OFF);
}

void PokerGame::ClearUpAfterGamble(void)
{
	if (HiloGambleFlag)
		ExitHiloGamble();
	else
		ExitPokerGame();

}

void PokerGame::RegulateGraphicalLampFlashSpeed(int FlashSpeed)
{
static signed char FlashGap = FlashSpeed;
	
	if (--FlashGap < 0)
	{
		FlashGap = FlashSpeed;
		FlashTog = !FlashTog;		
	}	
}

