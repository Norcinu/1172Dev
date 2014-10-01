#include "PokerGame.h"
#include "Game.h"
#include <ObjectHandler.h>
#include "VariableSoundProcess.h"

extern unsigned char global_quit;
extern	signed long StandardStoreLimitValue[MaxSelectableGames];

unsigned char PokerGame::GoldenJokerFeature(unsigned int WinValue)
{
unsigned char Flag = 0;

	if (WinValue)
	{
		if (AllowGoldenJokerFeature())	
		{
			PlaceOneOfDoubleJoker(DrawHand);
			Flag = 1;
		}
	}
	return(Flag);
}

void PokerGame::GoldenJokerUpdateDrawHand(void)
{
	switch(GoldenJokerStage)
	{
		case 1:	GoldenJokerUpdateIntroduction();
				GoldenJokerStage = 2;
				break;
		case 2:	GoldenJokerUpdateDeleteHelds();
				GoldenJokerStage = 3;
				break;		
		case 3:	if (GoldenJokerUpdateButtons())
					GoldenJokerStage = 4;
				break;
		case 4:	GoldenJokerUpdateEnd();
				GoldenJokerStage = 5;
				break;
		default:
			    break;
	}
}


void PokerGame::GoldenJokerUpdateIntroduction(void)
{
	PayTemp = Pay;
	if (AutoPlayFlag)
		ResetAutoPlayFlag = 1;
	AutoPlayFlag = 0;
	
	TheGame::Instance()->SetAutoplay(false);
	TheButtons::Instance()->SetButtonActivity(false, "AutoPlay");
	TheButtons::Instance()->SetOSButtonActivity(false, "AutoplayButton");

	GoldenJokerFeatureIntro();
}

void PokerGame::GoldenJokerUpdateDeleteHelds(void)
{
	GoldenJokerRemoveHolds();		
	ShowCurrentWin(PayTemp,MLAMP_FLASH);
}

bool PokerGame::GoldenJokerUpdateButtons(void)
{
	SetGoldenJokerLamps();
	return(InsertGoldenJokerButtons());
}

void PokerGame::GoldenJokerUpdateEnd(void)
{	
	Pay = Chkwin(DrawHand,1);
	ShowCurrentWin(Pay,MLAMP_ON);				
	
	ShowCurrentWin(PayTemp,MLAMP_OFF);
		
	if (ResetAutoPlayFlag == 1)
		AutoPlayFlag = 1;

#ifdef SOAK_BUILD
	SoakDoubleJokerFlag = 1;
#endif
}

bool PokerGame::InsertGoldenJokerButtons(void)
{
bool Flag = false;

#ifdef SOAK_BUILD

unsigned char BestPos;

	BestPos = FindTheBestPosition();
	DrawHand[BestPos] = JOKER_CARD;
	PutCard(BestPos,JOKER_CARD);		
	Flag = true;

#else

	if ((TheButtons::Instance()->OSButtonPressed("HoldInfoButton") || ReadCardHoldPb(0)) && !(DrawHand[0]&CARD_HELD))
	{		
		DrawHand[0] = JOKER_CARD;
		PutCard(0,JOKER_CARD);			
		Flag = true;
	}
	else if ((TheButtons::Instance()->OSButtonPressed("Hold2Button") || ReadCardHoldPb(1)) && !(DrawHand[1]&CARD_HELD))
	{
		DrawHand[1] = JOKER_CARD;
		PutCard(1,JOKER_CARD);		
		Flag = true;
	}
	else if ((TheButtons::Instance()->OSButtonPressed("Hold3Button") || ReadCardHoldPb(2)) && !(DrawHand[2]&CARD_HELD))
	{
		DrawHand[2] = JOKER_CARD;
		PutCard(2,JOKER_CARD);		
		Flag = true;
	}
	else if ((TheButtons::Instance()->OSButtonPressed("Hold4Button") || ReadCardHoldPb(3)) && !(DrawHand[3]&CARD_HELD))
	{		
		DrawHand[3] = JOKER_CARD;
		PutCard(3,JOKER_CARD);		
		Flag = true;
	}
	else if ((TheButtons::Instance()->OSButtonPressed("HoldTransferButton") || ReadCardHoldPb(4)) && !(DrawHand[4]&CARD_HELD))
	{		
		DrawHand[4] = JOKER_CARD;
		PutCard(4,JOKER_CARD);		
		Flag = true;
	}

#endif

	return(Flag);
}

void PokerGame::ShowCurrentWin(unsigned int WinValue,int State)
{
int WinLevel;

	if (WinValue)
	{
		WinLevel = FindWinLevel(WinValue);
		SetAwardValueLitState(WinLevel,State);
	}
}

void PokerGame::SetGoldenJokerLamps(void)
{
	if (!PokerHand[4].hold)	
	{
		TheButtons::Instance()->SetOSButtonActivity(true, "HoldTransferButton",LAMP_FLASH);  
		ThePokerGame::Instance()->SetActiveCardButton(true,4);
	}
	else
		TheButtons::Instance()->SetOSButtonActivity(false, "HoldTransferButton",LAMP_OFF);		  

	if (!PokerHand[3].hold)
	{
		TheButtons::Instance()->SetOSButtonActivity(true, "Hold4Button",LAMP_FLASH);  
		ThePokerGame::Instance()->SetActiveCardButton(true,3);
	}
	else
		TheButtons::Instance()->SetOSButtonActivity(false, "Hold4Button",LAMP_OFF);		

	if (!PokerHand[2].hold)
	{
		TheButtons::Instance()->SetOSButtonActivity(true, "Hold3Button",LAMP_FLASH);  
		ThePokerGame::Instance()->SetActiveCardButton(true,2);
	}
	else
		TheButtons::Instance()->SetOSButtonActivity(false, "Hold3Button",LAMP_OFF);	
	

	if (!PokerHand[1].hold)
	{
		TheButtons::Instance()->SetOSButtonActivity(true, "Hold2Button",LAMP_FLASH);  
		ThePokerGame::Instance()->SetActiveCardButton(true,1);
	}
	else
		TheButtons::Instance()->SetOSButtonActivity(false, "Hold2Button",LAMP_OFF);	
	

	if (!PokerHand[0].hold)
	{
		TheButtons::Instance()->SetOSButtonActivity(true, "HoldInfoButton",LAMP_FLASH);  
		ThePokerGame::Instance()->SetActiveCardButton(true,0);
	}
	else
		TheButtons::Instance()->SetOSButtonActivity(false, "HoldInfoButton",LAMP_OFF);		
}

unsigned char PokerGame::AllowGoldenJokerHoldButtons(unsigned int Index)
{
	if (!(DrawHand[Index-1]&CARD_HELD))
		return(1);
	else
		return(0);
}

void PokerGame::GoldenJokerRemoveHolds(void)
{	
	for (int i=0; i<5; i++)
	{
		if ((DrawHand[i]&~CARD_HELD) != JOKER_CARD)
		{
			if (PokerHand[i].hold)
			{				
				PokerHand[i].hold = 0;
				DrawHand[i] = (DrawHand[i]&~CARD_HELD);		
			}	
		}
		else
		{
			if (!PokerHand[i].hold)
			{				
				DrawHand[i] |= CARD_HELD;
				PokerHand[i].hold = 1;				
			}
		}		
	}
}

unsigned char PokerGame::CheckForUnheldCards(unsigned char *DealtHand)
{
	for (unsigned char i=0; i<5; i++)
	{
		if (!(DealtHand[i]&CARD_HELD))
			return(1);
	}
	return(0);
}

unsigned char PokerGame::AllowGoldenJokerFeature(void)
{
unsigned char Allow=0;

	if (CheckForUnheldCards(DrawHand) && !CheckForJokerCard(DrawHand) && CheckForGoldenJokerBenefit(DrawHand) && GoldenJokerControl())
		Allow = 1;
	return(Allow);
}

unsigned int PokerGame::FindTheBestPosition(void)
{
unsigned  int CurrentPay, BestPay=0, BestPosition=0;
unsigned char TempDrawHand[5];

	for (int i=0; i<5; i++)
	{
		for (int j=0; j<5; j++)
		{
			TempDrawHand[j] = DrawHand[j];
		}
		 
		TempDrawHand[i] = JOKER_CARD;
		CurrentPay = Chkwin(TempDrawHand,1);

		if (CurrentPay > BestPay)
		{
			BestPay = CurrentPay;
			BestPosition = i;
		}
	}

	return(BestPosition);
}

void PokerGame::GoldenJokerFeatureIntro(void)
{
	TheEngine::Instance()->GetProcessManager()->AddProcessToQueue(new VariableSoundProcess("WOLF_SND",WOLF_SND,3));	
}

unsigned  int GoldenJokerChance[9] = {0,2,5,10,20,25,30,30,30}; //0% 0.2% 0.5% 1% 2% 2.5% 3% 3% 3%
unsigned char PokerGame::GoldenJokerControl(void)
{
unsigned char allow = 0;
  signed  int ActualIndex, CentralIndex;

	CentralIndex = 4;
	ActualIndex  = CentralIndex + StandardStore[GetGameIndex()]/(3*StandardStoreLimitValue[GetGameIndex()]);

	if (ActualIndex < 0)
		ActualIndex = 0;

	if (ActualIndex > 8)
		ActualIndex = 8;

	if (GetLocalIntRandomNumber(1000) < GoldenJokerChance[ActualIndex])
		allow = 1;
		
	return(allow);
}

unsigned char PokerGame::CheckForGoldenJokerBenefit(unsigned char *DealtHand)
{
unsigned char Counter = 0;
unsigned char Position[5] = {0};
unsigned char CardPos,StoreCard[2];
unsigned char BenefitPresent=0;
unsigned char InternalDrawHand[5];
unsigned char i,j;
unsigned  int BasicWin, GoldenJokerWin, DoubleJokerWin;

	for (i=0; i<5; i++)
	{
		InternalDrawHand[i] = DealtHand[i];
		GoldenJokerPositions[i] = -1;
		GoldenJokerWins[i] = 0;
	}

	for (i=0; i<5; i++)
	{
		if (!(InternalDrawHand[i]&0x40)) //ie card not held		
			Position[Counter++] = i;					
	}

	if (Counter)
	{
		BasicWin = Chkwin(InternalDrawHand,1);		
		for (i=0; i<Counter; i++)
		{
			CardPos = Position[i];
			StoreCard[0] = InternalDrawHand[CardPos];
			InternalDrawHand[CardPos] = JOKER_CARD;
			GoldenJokerWin = Chkwin(InternalDrawHand,1);
			
			if (GoldenJokerWin >= BasicWin)
			{
				for (j=0; j<5; j++)
				{
					if (j != CardPos)
					{
						StoreCard[1]   = InternalDrawHand[j];
						InternalDrawHand[j] = JOKER_CARD;
						DoubleJokerWin = Chkwin(InternalDrawHand,1);
						InternalDrawHand[j] = StoreCard[1];
						if (DoubleJokerWin > GoldenJokerWin)
						{
							GoldenJokerWins[BenefitPresent]=DoubleJokerWin;
							GoldenJokerPositions[BenefitPresent] = Position[i];
							BenefitPresent++;
							break;
						}
					}
				}
			}

			InternalDrawHand[CardPos] = StoreCard[0];
			
		}
	}
		
	return(BenefitPresent);
}