#include <sstream>
#include <TextureManager.h>
#include <RenderSequenceManager.h>
#include "Defines.h"
#include "PokerGame.h"

PokerGame::PokerGame(void)
{

}

PokerGame::~PokerGame(void)
{

}

bool PokerGame::PokerGameInit()
{
	ActivateHiloGambleGraphics = 0;

	SetGameIndex(Game100p);

	DisplayStake();
	
	for (i=0; i<MAX_AWARDS; i++)
	{
		AwardDigitColour[i] = 1;
		AwardValueLitState[i] = 0;
	}

	for (i=0; i<5; i++)
	{
		PokerHand[i].id = 0;
		PokerHand[i].hold = 0;
		HiloHand[i].id = 0;
		HiloHand[i].state = 0;
	}
	
	AwardTable[0][0] = 100;
	AwardTable[0][1] = 200;
	AwardTable[0][2] = 300;
	AwardTable[0][3] = 400;
	AwardTable[0][4] = 600;
	AwardTable[0][5] = 1000;
	AwardTable[0][6] = 2500;
	AwardTable[0][7] = 4000;
	AwardTable[0][8] = 6000;
	AwardTable[0][9] = 10000;
	AwardTable[0][10]= 15000;
	AwardTable[0][11]= 25000;
	AwardTable[0][12]= 50000;

	AwardTable[1][0] = 200;
	AwardTable[1][1] = 400;
	AwardTable[1][2] = 600;
	AwardTable[1][3] = 800;
	AwardTable[1][4] = 1200;
	AwardTable[1][5] = 2000;
	AwardTable[1][6] = 5000;
	AwardTable[1][7] = 8000;
	AwardTable[1][8] = 12000;
	AwardTable[1][9] = 20000;
	AwardTable[1][10]= 30000;
	AwardTable[1][11]= 50000;
	AwardTable[1][12]= 50000;

	AwardValuesX = 630;
	AwardValuesY[12] = 42;
	AwardValuesY[11] = 80;
	AwardValuesY[10] = 114;
	AwardValuesY[9]	= 146;
	AwardValuesY[8] = 182;
	AwardValuesY[7] = 218;
	AwardValuesY[6] = 252;
	AwardValuesY[5] = 286;
	AwardValuesY[4] = 318;
	AwardValuesY[3] = 354;
	AwardValuesY[2] = 390;
	AwardValuesY[1] = 428;
	AwardValuesY[0] = 450;


	Object2D* allCards = TheObjectHandler::Instance()->GetObject2D("Cards");
	allCards->GetInstance(0)->SetCurrentSprite(54);
	allCards->GetInstance(1)->SetCurrentSprite(54);
	allCards->GetInstance(2)->SetCurrentSprite(54);
	allCards->GetInstance(3)->SetCurrentSprite(54);
	allCards->GetInstance(4)->SetCurrentSprite(54);

	InitializeCardButtonArray();
	
	return true;
}