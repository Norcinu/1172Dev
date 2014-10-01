#include <ObjectHandler.h>
#include <TextureManager.h>
#include <bo.h>
#include "GenericButtons.h"
#include "CountToCreditsProcess.h"
#include "CollectProcess.h"
#include "Defines.h"
#include "Game.h"

extern unsigned char global_quit;

CountToCreditsProcess::CountToCreditsProcess()
{
	m_animationTime = 0.0f;
	m_increment = 0;
	m_countToBankProcess = false;
	m_valueToHit = 0;
}

CountToCreditsProcess::~CountToCreditsProcess()
{
	
}

void CountToCreditsProcess::Complete()
{
	TheButtons::Instance()->SetButtonActivity(false, "Start");
	TheButtons::Instance()->SetButtonActivity(false, "OnePound");
	TheButtons::Instance()->SetButtonActivity(false, "TwoPound");
	TheAudioManager::Instance()->GetAudioSample("SX_BANKINC")->Stop();
}

void CountToCreditsProcess::Init()
{
	GameProcess::Init();

	m_increment = 10;

	if(TheGame::Instance()->GetFinalWinValue() > 5000)
	{
		m_increment = 30;
	}
	else if(TheGame::Instance()->GetFinalWinValue() < 500)
	{
		m_increment = 10;
	}

	if(!TheGame::Instance()->GetInterimWinValue())
	{
		SetCompleted();
	}

	TheButtons::Instance()->SetButtonActivity(true, "Start", LAMP_ON);
	if(TheGame::Instance()->GetStake()==MINIMUM_BET)
	{
		TheButtons::Instance()->SetButtonActivity(true, "OnePound", LAMP_ON);
	}
	else
	{
		TheButtons::Instance()->SetButtonActivity(true, "TwoPound", LAMP_ON);
	}
	
}

void CountToCreditsProcess::Update()
{
#ifndef FAST_PLAY
	if(!m_countToBankProcess && !global_quit)
	{
			m_countToBankProcess = true;
	}
	
	if(m_countToBankProcess)
	{
		if(TheButtons::Instance()->ButtonPressed("Start") ||
			TheButtons::Instance()->ButtonPressed("OnePound") ||
			TheButtons::Instance()->ButtonPressed("TwoPound"))
		{
			TheGame::Instance()->ClearInterimWinValue();
			TheGame::Instance()->SetDeduct(0);
			SetCompleted();
			return;
		}
		
		if(TheGame::Instance()->GetInterimWinValue())
		{	
			
			if(TheGame::Instance()->GetInterimWinValue() < (unsigned)m_increment)
			{
				TheGame::Instance()->ClearInterimWinValue();
				TheGame::Instance()->SetDeduct(TheGame::Instance()->GetInterimWinValue());
			}
			else
			{
				TheGame::Instance()->IncrementInterimWinValue(- m_increment);
				TheGame::Instance()->SetDeduct(TheGame::Instance()->GetInterimWinValue());
			}
		}
		else if(!TheGame::Instance()->GetInterimWinValue())
		{			
			SetCompleted();
		}
	}
#else
	if(TheGame::Instance()->GetInterimWinValue())
	{
		TheGame::Instance()->ClearInterimWinValue();
		SetCompleted();
	}
#endif
}