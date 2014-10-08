#include "Game.h"
#include "PokerGame.h"
#include <ObjectHandler.h>
#include <AudioManager.h>
#include "Defines.h"


void PokerGame::UpdateInfoDigits(bool Visibility)
{
	char buff[32];

	if (Visibility == true)
	{
		float Percentage = (float)GetTargetPercentage();		
		sprintf_s(buff, "%2.0f%%%%", Percentage);				
		OBJECT_HANDLER->GetText("HelpInfoDigits")->SetPosition(D3DXVECTOR2((float)1735,(float)185));
		OBJECT_HANDLER->GetText("HelpInfoDigits")->SetScale(D3DXVECTOR2(1.3f,1.3f));	
		OBJECT_HANDLER->GetText("HelpInfoDigits")->SetMessage(buff);
	}
	OBJECT_HANDLER->GetText("HelpInfoDigits")->SetVisible(Visibility);	
}

void PokerGame::UpdateGoldenDigits(bool Visibility)
{
	char buff[32];

	if (Visibility == true)
	{
		float Percentage = (float)GetTargetPercentage();		
		sprintf_s(buff, "%2.0f%%%%", Percentage);				
		OBJECT_HANDLER->GetText("HelpGoldenDigits")->SetPosition(D3DXVECTOR2((float)818,(float)495));
		OBJECT_HANDLER->GetText("HelpGoldenDigits")->SetScale(D3DXVECTOR2(2.0f,2.0f));	
		OBJECT_HANDLER->GetText("HelpGoldenDigits")->SetMessage(buff);
	}
	OBJECT_HANDLER->GetText("HelpGoldenDigits")->SetVisible(Visibility);	
}

void PokerGame::UpdateErrorDigits(bool Visibility)
{
	char buff[32];

	if (Visibility == true)
	{		
		sprintf_s(buff, "%d", GetCurrentError());

		unsigned int Width  = 24;
		unsigned int Height = 40;

		unsigned int X = (unsigned int)OBJECT_HANDLER->GetObject2D("RGErrorMessage")->GetPosition().x;
		unsigned int Y = (unsigned int)OBJECT_HANDLER->GetObject2D("RGErrorMessage")->GetPosition().y;
		X += 420;
		Y += 125;

		unsigned int Div = 1;
		unsigned int DigitCounter=1;

		do
		{
			Div *= 10;
			if (GetCurrentError()/Div)
				DigitCounter++;
			else
				break;
		}while(1);
		
		//X -= DigitCounter*Width/2;
		//Y -= Height/2;
		

		OBJECT_HANDLER->GetText("GenErrDigits")->SetPosition(D3DXVECTOR2((float)X,(float)Y));
		OBJECT_HANDLER->GetText("GenErrDigits")->SetScale(D3DXVECTOR2(1.0f,1.0f));	
		OBJECT_HANDLER->GetText("GenErrDigits")->SetMessage(buff);
	}
	OBJECT_HANDLER->GetText("GenErrDigits")->SetVisible(Visibility);	
}