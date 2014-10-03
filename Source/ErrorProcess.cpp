#include <AudioManager.h>
#include <ObjectHandler.h>
#include <TextureManager.h>
#include "bo.h"
#include "ErrorProcess.h"
#include "Game.h"

ErrorProcess::ErrorProcess()
{
	m_currentError = 0;
	mHadError = false;
}

ErrorProcess::~ErrorProcess()
{
	
}

void ErrorProcess::Complete()
{

}

void ErrorProcess::Init()
{
	GameProcess::Init();
}

void ErrorProcess::Update()
{
	PROFILE(__FUNCTION__);	
	if(m_currentError = GetCurrentError())
	{	
		mHadError = true;
		if(GetGameInProgress())
		{
			THE_GAME->SetAutoplay(false);
		}
		else
		{
			THE_GAME->DrawCurrentError();
		}
		if(GetSwitchStatus(REFILL_KEY))
		{
			THE_GAME->QuitToMainMenu();
		}
	}
	else
	{
		if(mHadError) //This way if a warning error such as comms buffer comes up this will self clear
		{
			TheAudioManager::Instance()->GetAudioSample("ALARM")->Stop();			
			mHadError = false;
		}
	}
}