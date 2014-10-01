#include <ObjectHandler.h>
#include "Game.h"
#include "JackpotProcess.h"
#include "Defines.h"
#include "PokerGame.h"

JackpotProcess::JackpotProcess()
{	
	mImage	       = TheObjectHandler::Instance()->GetObject2D("Jackpot");

	mInitialX      = mX	= mImage->GetPosition().x;
	mInitialY	   = mY	= mImage->GetPosition().y;
	mInitialXScale = mXScale = mImage->GetScale().x;
	mInitialYScale = mYScale = mImage->GetScale().y;

	mImageWidth	   = 1512*mXScale;
	mImageHeight   = 254*mYScale;	
	mNumSteps      = 20;
	mFinalHeight   = mImageHeight/mNumSteps; 
	mFinalWidth    = mImageWidth/mNumSteps;
	mStepHeight    = (mImageHeight-mFinalHeight)/mNumSteps;
	mStepWidth     = (mImageWidth-mFinalWidth)/mNumSteps;

	mCurrentStep = 0;

	mImage->SetVisible(true);

	TheAudioManager::Instance()->GetAudioSample("SX_FSPIN")->Play();

	mJackpotDelayTimer = 0;
}

JackpotProcess::~JackpotProcess()
{
	
}

void JackpotProcess::Complete()
{
	mImage->SetPosition(D3DXVECTOR2((float)mInitialX,(float)mInitialY));
	mImage->SetScale(D3DXVECTOR2((float)mInitialXScale,(float)mInitialYScale));
	mImage->SetVisible(false);
}

void JackpotProcess::Update()
{	
float NewImageWidth, NewImageHeight;

	if (TheEngine::Instance()->GetSystemTimer().GetRunningTime() < mJackpotDelayTimer)
	{
		return;
	}

	NewImageWidth  = mImageWidth-mStepWidth*mCurrentStep;
	NewImageHeight = mImageHeight-mStepHeight*mCurrentStep;

	mX = (1920 -  NewImageWidth)/2 ;
	mY = (1080 - NewImageHeight)/2;

	mXScale = NewImageWidth/mImageWidth;
	mYScale = NewImageHeight/mImageHeight;

	mImage->SetPosition(D3DXVECTOR2((float)mX,(float)mY));
	mImage->SetScale(D3DXVECTOR2((float)mXScale,(float)mYScale));

	mImage->SetVisible(true);

	mCurrentStep++;

	if (mCurrentStep >= mNumSteps)
		SetCompleted();
	else
	{
		if (mCurrentStep == 3)
			mJackpotDelayTimer = TheEngine::Instance()->GetSystemTimer().GetRunningTime() + ThePokerGame::Instance()->GeneralSetGameDelay(1.0f);
		else
			mJackpotDelayTimer = TheEngine::Instance()->GetSystemTimer().GetRunningTime() + ThePokerGame::Instance()->GeneralSetGameDelay(0.01f);
	}
}