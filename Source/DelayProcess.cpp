#include <Engine.h>
#include "DelayProcess.h"

DelayProcess::DelayProcess(float holdTime)
{
	mHoldTime = holdTime;
	mTimer = 0.0f;
}

void DelayProcess::Init()
{
	mTimer = TheEngine::Instance()->GetSystemTimer().GetRunningTime() + mHoldTime;
}

void DelayProcess::Update()
{
	if(mTimer < TheEngine::Instance()->GetSystemTimer().GetRunningTime())
	{
		SetCompleted();
	}
}