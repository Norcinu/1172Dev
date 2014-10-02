#include "PokerGame.h"
#include <ObjectHandler.h>


bool PokerGame::CollectBoxPb(int button)
{
	bool Picked = false;

	Object2D* CollectBox = OBJECT_HANDLER->GetObject2D("RGCollectOrPrintTicket");
	float X = CollectBox->GetPosition().x;
	float Y = CollectBox->GetPosition().y;
	float Width = 600;
	float Height= 250;

	if (button == COLLECT_COINS_PB)
	{
		X     +=  54;
		Y     += 176;
	}
	else
	{
		X     += 326;
		Y     += 176;
	}

	Width  = 184;
	Height = 54;

	RECT rect = {(long)X, (long)Y, (long)(X+Width), (long)(Y+Height)};

	if (TheInput::Instance()->Pick2D(CollectBox))
		Picked = true;

	return(Picked);
}

bool PokerGame::ReadCollectBoxPb(int button)
{
	if (RegisterCollectBoxBtnArray[button])
	{
		TheAudioManager::Instance()->GetAudioSample("DROP")->Play();
		RegisterCollectBoxBtnArray[button] = false;
		return(true);
	}
	return(false);
}

void PokerGame::RegisterCollectBoxButtons(void)
{
unsigned char i;

	for (i=0; i<2; i++)
	{
		if (CollectBoxPb(i) && GetActiveCollectBoxButton(i))
		{
			TheAudioManager::Instance()->GetAudioSample("DROP")->Play();
			RegisterCollectBoxBtnArray[i] = true;
			return;
		}
	}
}

void PokerGame::SetActiveCollectBoxButton(bool State, int Button)
{
	CollectBoxButtonActive[Button] = State;
}

bool PokerGame::GetActiveCollectBoxButton(int Button)
{
	return(CollectBoxButtonActive[Button]);
}