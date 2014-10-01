#include "Game.h"
#include "PokerGame.h"
#include <ObjectHandler.h>
#include <AudioManager.h>
#include "Defines.h"

extern unsigned char FlashTog;
void PokerGame::UpdateAwardDigits()
{
	char buff[32];
	char buff2[32];
	
	ShowAllAwardValueLitStates();
	for (int i=1; i<MAX_AWARDS; i++)
	{
		if (i<10)
			sprintf_s(buff2, "AwardLevel0%d",i);
		else
			sprintf_s(buff2, "AwardLevel%d",i);

		float AwardValue = (float)AwardTable[GameIndex][i];
		AwardValue /= 100.0f;
		sprintf_s(buff, "£%.02f", AwardValue);
		
		TheObjectHandler::Instance()->GetText(buff2)->SetPosition(D3DXVECTOR2((float)AwardValuesX,(float)AwardValuesY[i]));
		TheObjectHandler::Instance()->GetText(buff2)->SetScale(D3DXVECTOR2(1.0f,0.8f));
		if (AwardDigitColour[i])
			TheObjectHandler::Instance()->GetText(buff2)->SetColour(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		else
			TheObjectHandler::Instance()->GetText(buff2)->SetColour(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		TheObjectHandler::Instance()->GetText(buff2)->SetMessage(buff);
		TheObjectHandler::Instance()->GetText(buff2)->SetVisible(true);
	}
}


void PokerGame::SetAwardValueLitState(int Index,int State)
{
	AwardValueLitState[Index] = State;
}

void PokerGame::TurnOnAwardValue(int Index)
{
	AwardDigitColour[Index] = 0;
}

void PokerGame::TurnOffAwardValue(int Index)
{
	AwardDigitColour[Index] = 1;
}

void PokerGame::FlashAwardValue(int Index)
{
	if (!FlashTog)
		TurnOnAwardValue(Index);
	else
		TurnOffAwardValue(Index);		
}

void PokerGame::AntiFlashAwardValue(int Index)
{
	if (FlashTog)
		TurnOnAwardValue(Index);
	else
		TurnOffAwardValue(Index);		
}

void PokerGame::ShowAllAwardValueLitStates(void)
{
	for (int Index=0;Index<LEVEL;Index++)
	{
		switch(AwardValueLitState[Index])
		{
			case MLAMP_OFF   :TurnOffAwardValue(Index);    break; 
			case MLAMP_ON    :TurnOnAwardValue(Index);     break; 
			case MLAMP_FLASH :FlashAwardValue(Index);	   break; 
			case MLAMP_AFLASH:AntiFlashAwardValue(Index);  break; 
			default:									   break;	
		}
	}
}