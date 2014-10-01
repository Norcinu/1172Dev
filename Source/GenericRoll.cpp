#include <sstream>
#include <GenericMorphControl.h>
#include "Game.h"
#include "Defines.h"
#include "WinData.h"
#include "GenericReelControl.h"


int FeatureGame2DB = 0;

void Game::Roll()
{
	if(IsDemoMode())
		ComputeWinDoorOpen();
	else
		ComputeWinLive();			//Real time/Production call
}

void Game::ComputeWinDoorOpen()
{
	m_reelControl->mWinAmount = 0;

	if(!GetLocalIntRandomNumber(5))
	{
		m_reelControl->mWinAmount = mWinData[mStake].mWinValues[GetLocalIntRandomNumber(mWinData[mStake].mWinsCount)]; //was ALL_WINS
	}
	if(GetCurrentFreeSpinsControl()->GetNumberOfFreeSpins())
		m_reelControl->mWinAmount =0; //do not reset it as it will need to set it again if door closed!!

	m_reelControl->SetWinTable(m_reelControl->mWinAmount); 
	m_reelControl->StartReelSpin(m_reelControl->mWinAmount);	//DOOR OPEN CALL
}

//--------------------------------------------------------------------------
void Game::ComputeWinLive()
{
	m_reelControl->mWinAmount = 0; //Ensure win reset to zero start of every game.

	if(!GetFreeSpinsControl(mStake)->GetNumberOfFreeSpins())
	{	//Not In Free Spins Process PJ and normal wins.
		if(!AwardAPJPot())  //Try every game To Awarded PJ 
		{
			//!PJ so Compute Next Win.
			m_reelControl->mWinAmount = GetMorphControl(mStake)->ComputeWinAmount(mStake);				
			
			unsigned int freeSpinsCount = GetCurrentFreeSpinsControl()->ComputeFSpinAllowed();
			
			if(!freeSpinsCount)
			{	//FSpin Not Awarded Normal Reel spin Win or Lose.
				m_reelControl->SetWinTable(m_reelControl->mWinAmount);
			}
			else
			{	//FSpin Awarded Spin in FSpin Symbols.
				FeatureGame2DB = freeSpinsCount;
				m_reelControl->SetFeatureTable(freeSpinsCount);
			}
			//
		}
		else
		{   //PJ Won go straight to losing game.
			m_reelControl->SetWinTable(m_reelControl->mWinAmount); 
		}
	}
	else
	{	//FreeSpins still remaining
		GetCurrentFreeSpinsControl()->ComputeFSpinOutcome();
		m_reelControl->SetWinTable(m_reelControl->mWinAmount);
	}
	m_reelControl->StartReelSpin(m_reelControl->mWinAmount);
}

int Game::AwardAPJPot()
{
	return 0;
}