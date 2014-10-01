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
		sprintf_s(buff, "%2.0f%%", Percentage);				
		TheObjectHandler::Instance()->GetText("HelpInfoDigits")->SetPosition(D3DXVECTOR2((float)1678,(float)232));
		TheObjectHandler::Instance()->GetText("HelpInfoDigits")->SetScale(D3DXVECTOR2(1.3f,1.3f));	
		TheObjectHandler::Instance()->GetText("HelpInfoDigits")->SetMessage(buff);
	}
	TheObjectHandler::Instance()->GetText("HelpInfoDigits")->SetVisible(Visibility);	
}


void PokerGame::UpdateGoldenDigits(bool Visibility)
{
	char buff[32];

	if (Visibility == true)
	{
		float Percentage = (float)GetTargetPercentage();		
		sprintf_s(buff, "%2.0f%%", Percentage);				
		TheObjectHandler::Instance()->GetText("HelpGoldenDigits")->SetPosition(D3DXVECTOR2((float)830,(float)500));
		TheObjectHandler::Instance()->GetText("HelpGoldenDigits")->SetScale(D3DXVECTOR2(2.3f,2.3f));	
		TheObjectHandler::Instance()->GetText("HelpGoldenDigits")->SetMessage(buff);
	}
	TheObjectHandler::Instance()->GetText("HelpGoldenDigits")->SetVisible(Visibility);	
}

void PokerGame::UpdateErrorDigits(bool Visibility)
{
	char buff[32];

	if (Visibility == true)
	{		
		sprintf_s(buff, "%d", GetCurrentError());

		unsigned int Width  = 24;
		unsigned int Height = 40;

		unsigned int X = (unsigned int)TheObjectHandler::Instance()->GetObject2D("RGErrorMessage")->GetPosition().x;
		unsigned int Y = (unsigned int)TheObjectHandler::Instance()->GetObject2D("RGErrorMessage")->GetPosition().y;
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
		

		TheObjectHandler::Instance()->GetText("GenErrDigits")->SetPosition(D3DXVECTOR2((float)X,(float)Y));
		TheObjectHandler::Instance()->GetText("GenErrDigits")->SetScale(D3DXVECTOR2(1.0f,1.0f));	
		TheObjectHandler::Instance()->GetText("GenErrDigits")->SetMessage(buff);
	}
	TheObjectHandler::Instance()->GetText("GenErrDigits")->SetVisible(Visibility);	
}