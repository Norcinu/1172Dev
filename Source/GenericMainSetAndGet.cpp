/*
	Game.cpp
	Created 12/04/12 - James Martin
	Contains Game Setup, Shutdown
*/
#include <Input.h>
#include <ObjectHandler.h>
#include <sstream>
#include <TextureManager.h>

#include "Game.h"
#include "bo.h"
#include "Defines.h"
#include "PokerGame.h"
#include "PopControl.h"

void msg_service()
{

}

Game::Game(void)
{

}

Game::~Game(void)
{

}


void Game::SetAutoplay(bool autoplay)
{
	m_autoplay = autoplay;

	if (m_autoplay)
		ThePokerGame::Instance()->AutoPlayFlag = 1;
	else
		ThePokerGame::Instance()->AutoPlayFlag = 0;
}

void Game::SetDeduct(unsigned int deduct)
{
	m_deduct = deduct;
}

void Game::SetFeatureWinValue(unsigned int featureWinValue)
{
	m_featureWinValue = featureWinValue;
}

void Game::SetFinalWinValue(unsigned int finalWinValue)
{
	m_finalWinValue = finalWinValue;
}

void Game::SetInterimFeatureWinValue(unsigned int interimFeatureWinValue)
{
	m_interimFeatureWinValue = interimFeatureWinValue;
}

void Game::SetInterimWinValue(unsigned int interimWinValue)
{
	m_interimWinValue = interimWinValue;
}

void Game::IncrementFeatureWinValue(int featureWinValue)
{
	m_featureWinValue += featureWinValue;
}

void Game::IncrementFinalWinValue(int finalWinValue)
{
	m_finalWinValue += finalWinValue;
}

void Game::IncrementInterimFeatureWinValue(int interimFeatureWinValue)
{
	m_interimFeatureWinValue += interimFeatureWinValue;
}

void Game::IncrementInterimWinValue(int interimWinValue)
{
	m_interimWinValue += interimWinValue;
}

void Game::ClearFeatureWinValue()
{
	m_featureWinValue = 0;
}

void Game::ClearFinalWinValue()
{
	m_finalWinValue = 0;
}

void Game::ClearInterimFeatureWinValue()
{
	m_interimFeatureWinValue = 0;
}

void Game::ClearInterimWinValue()
{
	m_interimWinValue = 0;
}

void Game::SetStake(unsigned int stake)
{
	if (mStake != stake)
	{
		mStake = stake;
		ThePokerGame::Instance()->SetGameIndex((mStake == MAXIMUM_BET) ? 1 : 0);
		m_popControl->ChangeStake(GetStakeID()); 
	}
	else
	{
		m_popControl->ClosePopOptions(GetStakeID());
	}
}

void Game::SetDemoMode(bool demoMode)
{
	mDemoMode = demoMode;
}

bool Game::IsDemoMode()
{
	return mDemoMode;
}

bool Game::GetAutoplay() const
{
	return m_autoplay;
}


unsigned int Game::GetDeduct() const
{
	return m_deduct;
}

unsigned int Game::GetFeatureWinValue() const
{
	return m_featureWinValue;
}

unsigned int Game::GetFinalWinValue() const
{
	return m_finalWinValue;
}

unsigned int Game::GetInterimFeatureWinValue() const
{
	return m_interimFeatureWinValue;
}

unsigned int Game::GetInterimWinValue() const
{
	return m_interimWinValue;
}

bool Game::IsHD()
{
	unsigned int width = ENGINE->GetScreenWidth();
	unsigned int height = ENGINE->GetScreenHeight();
	
	if (width >= 1920 && height >= 1080) 
		m_isHD = true;
	else
		m_isHD = false;
	
	return m_isHD;
}

bool Game::HasStake(const unsigned int stake) const
{
	for (int i = 0; i < (TOTAL_STAKES+2); i++) 
	{
		if (stake == m_gameStakes[i])
			return true;
	}
	return false;
}

int Game::GetStakeID() const
{
	for(int i = 0; i < (TOTAL_STAKES+2);++i)
	{
		if(mStake == AllStakes[i])
		{
			return (i-2); //yuck
		}
	}
	return -1;
}

void Game::ClosePopOptions()
{
	if(m_popControl->AreOptionsVisible())
	{
		m_popControl->ClosePopOptions(GetStakeID());
	}
}

void Game::TogglePopOptions()
{
	m_popControl->ShowHide(GetStakeID());
}

bool Game::ArePopOptionsVisible() const
{
	return m_popControl->AreOptionsVisible();
}

unsigned int Game::GetNextStake() const
{
	for(size_t i = 0; i < m_gameStakes.size();++i)
	{
		if(mStake == m_gameStakes[i])
		{
			int nextStake = (i+1) % m_gameStakes.size();
			return m_gameStakes[nextStake];
		}
	}

	return m_gameStakes[0];//error
}