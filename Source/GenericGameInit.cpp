#include <sstream>
#include <TextureManager.h>
#include <RenderSequenceManager.h>
#include "Defines.h"
#include "Game.h"
#include "PokerGame.h"
#include "PopControl.h"

bool Game::GameInit()
{
	m_autoplay = false;
	mDemoMode = false;
	m_isHD = true;

	m_finalWinValue = 0;
	m_featureWinValue = 0;
	m_interimFeatureWinValue = 0;
	m_interimWinValue = 0;
	m_deduct = 0;

	mTargetValue = 0;

	m_gameStakes.reserve(TOTAL_STAKES);
	m_gameStakes.push_back(MINIMUM_BET);
	m_gameStakes.push_back(MAXIMUM_BET);
	
	m_currentTotalStake[0] = 0;
	m_currentTotalStake[1] = 0;
	m_currentTotalWon[0] = 0;
	m_currentTotalWon[1] = 0;
	
	//Default Stake To Max
	mStake = m_gameStakes[0];

	m_popControl = std::make_shared<PopControl>();
	m_popControl->Initialise(m_gameStakes, GetStakeID());
		
	if(GetUsesBackOffice())
	{
		TheAudioManager::Instance()->SetVolume(GetRemoteMasterVolume());
	}
	else
	{
		TheAudioManager::Instance()->SetVolume(GetLocalMasterVolume());
	}
	
#ifdef SOAK_BUILD
	TheAudioManager::Instance()->EnableSuperFastPlay();
#endif
	
#ifdef FIXED_POP
	mStake = FIXED_POP;
#endif

	THE_BUTTONS->Initialise();

	POKER_GAME->PokerGameInit();

	POKER_GAME->UpdateAwardDigits();
	
	UpdateDigits();

	return true;
}

bool Game::GetPercentage(unsigned int stake, float* percentage)
{
	switch(stake)
	{
	case(100):
		{
			*percentage = 0.0f;
			return true;
			break;
		}
	case(200):
		{
			*percentage = 2.0f;
			return true;
			break;
		}
	default:
		{
			return false;
		}
	}
}