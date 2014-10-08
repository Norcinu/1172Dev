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
		
		OBJECT_HANDLER->GetText(buff2)->SetPosition(D3DXVECTOR2((float)AwardValuesX,(float)AwardValuesY[i]));
		OBJECT_HANDLER->GetText(buff2)->SetScale(D3DXVECTOR2(1.3f,1.2f)); //1.0f, 0.8f
		if (AwardDigitColour[i])
			OBJECT_HANDLER->GetText(buff2)->SetColour(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.f));
		else
			OBJECT_HANDLER->GetText(buff2)->SetColour(D3DXCOLOR(1.f, 1.f, 0.f, 1.0f));
		OBJECT_HANDLER->GetText(buff2)->SetMessage(buff);
		OBJECT_HANDLER->GetText(buff2)->SetVisible(true);
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