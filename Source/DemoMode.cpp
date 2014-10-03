#include "PokerGame.h"
#include "Game.h"
#include <ObjectHandler.h>


bool PokerGame::YesDemoPb(void)
{
bool Picked=false;

	Object2D* DemoBox = OBJECT_HANDLER->GetObject2D("RGDemoMode");
	float X = DemoBox->GetPosition().x;
	float Y = DemoBox->GetPosition().y;
	float Width = 600;
	float Height= 250;

	RECT rect = {(long)X, (long)Y, (long)(X+Width), (long)(Y+Height)};

//	if (TheInput::Instance()->Pick2D(D3DXVECTOR2(X,Y),rect))						
//		Picked = true;

	return(Picked);
}

bool PokerGame::ReadYesDemoPb(void)
{
	if (YesDemoPbStatus)
	{
		TheAudioManager::Instance()->GetAudioSample("DROP")->Play();
		YesDemoPbStatus = false;		
		return(true);
	}
	return(false);
}

void PokerGame::RegisterYesDemoButton(void)
{
	if (GetDoorStatus() && !THE_GAME->IsDemoMode())
	{
		if (YesDemoPb())
		{
			TheAudioManager::Instance()->GetAudioSample("DROP")->Play();
			YesDemoPbStatus = true;
		}
	}
}

