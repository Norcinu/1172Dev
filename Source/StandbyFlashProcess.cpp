#include <ObjectHandler.h>
#include "StandbyFlashProcess.h"
#include <Engine.h>

StandbyFlashProcess::StandbyFlashProcess()
{
	m_alphaSwitch = false;
	m_standbyColour = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	m_animationTime = 0.0f;
}

StandbyFlashProcess::~StandbyFlashProcess()
{
	
}

void StandbyFlashProcess::Complete()
{
	OBJECT_HANDLER->GetObject2D("NoCredit")->SetVisible(false);
}

void StandbyFlashProcess::Init()
{
	GameProcess::Init();
	OBJECT_HANDLER->GetObject2D("NoCredit")->SetVisible(true);
}

void StandbyFlashProcess::Update()
{
	if((ENGINE->GetSystemTimer().GetRunningTime() > m_animationTime) && !m_alphaSwitch)
	{
		m_standbyColour.a += 0.02f;

		if(m_standbyColour.a >= 1.0f)
		{
			m_animationTime = ENGINE->GetSystemTimer().GetRunningTime() + 1.5f;
			m_alphaSwitch = true;
		}
	}
	else if((ENGINE->GetSystemTimer().GetRunningTime() > m_animationTime) && m_alphaSwitch)
	{
		m_standbyColour.a -= 0.02f;

		if(m_standbyColour.a <= 0.0f)
		{
			m_animationTime = ENGINE->GetSystemTimer().GetRunningTime() + 0.5f;
			m_alphaSwitch = false;
		}
	}

	OBJECT_HANDLER->GetObject2D("NoCredit")->SetColour(m_standbyColour);
}