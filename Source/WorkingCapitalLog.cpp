#include <iostream>
#include <fstream>
#include <FileRead.h>

#include <GenericMorphControl.h>
#include "WorkingCapitalLog.h"
#include <bo.h>
#include "Game.h"
#include "Defines.h"
#include "StakeChangeProcess.h"


#ifdef SOAK_BUILD
	extern unsigned long zero_cdeposit(void);
	extern unsigned long add_cdeposit(unsigned long value);
#endif

WorkingCapitalLog::WorkingCapitalLog()
{
	m_GameCount = 0;
	m_WorkingCapital = 0;
	m_SessonNumber = 0;
	m_CountPlayersCollecting = 0;
	m_CountPlayersLosing = 0;

	m_MostGamesInWinSession = 0;
	m_LeastGamesInWinSession = 1000; //  Max 1
	m_AvgGamesInSession = 0;

	m_LongestLosingGameSession = 0;
	m_ShortestLosingGameSession = 1000;	//Max 100

	m_HighestCollectAmount = 0;
	m_LowestCollectAmount = 10000;

	m_HighestSessionPercentage = 0.0f;
	m_LowestSessionPercentage = 1000.0f;

	m_SessionTotalWon        = 0;
	m_SessionTotalWagered    = 0;
	
	m_TotalWagered	= 0;
	m_TotalWon		= 0;
	m_TotalGames	= 0;
	m_TotalGamesInLosingSession	= 0;
	m_TotalWins		= 0;

	m_Percentage = 0;

}

WorkingCapitalLog::~WorkingCapitalLog()
{

}

void WorkingCapitalLog::Init()
{
#ifdef SOAK_BUILD
	m_SessonNumber++;

	//Zero everything down
	ZeroBankDeposit();
	zero_cdeposit();

#ifndef FIXED_POP
	TheGame::Instance()->SetStake(MINIMUM_BET);
	if(GetLocalIntRandomNumber(2))
		TheGame::Instance()->SetStake(MAXIMUM_BET);

	StakeChangeProcess* stakeChange = new StakeChangeProcess(TheGame::Instance()->GetStake());
	TheEngine::Instance()->GetProcessManager()->AddProcessToQueue(stakeChange);
#endif
	m_WorkingCapital = 100* TheGame::Instance()->GetStake();

	int tmpWCapital = m_WorkingCapital;

	while(tmpWCapital>0)
	{
		add_cdeposit(100);
		tmpWCapital -= 100;
	}

	m_CurrentCapital = m_WorkingCapital;
	m_GameCount = TheGame::Instance()->mGameCounter;
	
	m_SessionTotalWon     = 0;
	m_SessionTotalWagered = 0;

	
	TheGame::Instance()->GetMorphControl(TheGame::Instance()->GetStake())->ResetTablePointer();
#endif
}

void WorkingCapitalLog::Update() 
{	
	m_CurrentCapital = GetCredits() + GetBankDeposit();
	
	m_SessionTotalWon     += TheGame::Instance()->GetFinalWinValue(); 
	m_SessionTotalWagered += TheGame::Instance()->GetStake();


	//Sessions end when one of the following is true;
	if(m_CurrentCapital >= (m_WorkingCapital * 2) || m_CurrentCapital <  TheGame::Instance()->GetStake()) 
	{
		Complete();
		Init();
	}
}

void WorkingCapitalLog::Complete() 
{
	unsigned int Collect = GetCredits() + GetBankDeposit();	

	float SessionWinPercentage = (m_SessionTotalWon / m_SessionTotalWagered) * 100.0f;

	m_GameCount = TheGame::Instance()->mGameCounter - m_GameCount;


	//Player Wins
	if(m_CurrentCapital >= (m_WorkingCapital * 2))
	{
		m_CountPlayersCollecting++;
		
		if(Collect > m_HighestCollectAmount)
		{
			m_HighestCollectAmount = Collect;
		}

		if(Collect < m_LowestCollectAmount)
		{
			m_LowestCollectAmount = Collect;
		}
		
		if(SessionWinPercentage > m_HighestSessionPercentage)
		{
			m_HighestSessionPercentage = SessionWinPercentage;
		}

		if(SessionWinPercentage < m_LowestSessionPercentage)
		{
			m_LowestSessionPercentage = SessionWinPercentage;
		}
		
		if(m_GameCount > m_MostGamesInWinSession)
		{
			m_MostGamesInWinSession = m_GameCount;
		}

		if(m_GameCount < m_LeastGamesInWinSession)
		{
			m_LeastGamesInWinSession = m_GameCount;
		}
		
	}

	//Player Loses
	else if (m_CurrentCapital < TheGame::Instance()->GetStake())
	{
		m_CountPlayersLosing++;
		m_TotalGamesInLosingSession += m_GameCount;

		if( m_GameCount > m_LongestLosingGameSession)
		{
			m_LongestLosingGameSession = m_GameCount;
		}

		if( m_GameCount < m_ShortestLosingGameSession)
		{
			m_ShortestLosingGameSession = m_GameCount;
		}
	
	}
//#ifndef RECORD_NMI_LOG
	StoreData();
//#endif
}

void WorkingCapitalLog::StoreData()
{
	m_TotalGames += m_GameCount;
	
	m_AvgGamesInSession = m_TotalGames / m_SessonNumber;
	if(m_CountPlayersLosing)
		m_AvgGamesInLosingSession = m_TotalGamesInLosingSession / m_CountPlayersLosing;

	m_TotalWagered += m_SessionTotalWagered;
	m_TotalWon     += m_SessionTotalWon;
	m_Percentage    = m_TotalWon / m_TotalWagered;
	m_Percentage   *= 100;	

	static float TotalPercentage = m_Percentage;
	
	SetFileAction();   //Always add before writing to disk

	std::ofstream LogFile;
	LogFile.open (WCAPTICAL_LOG_PATH);

	LogFile << "Working Capital: \t\t" << m_WorkingCapital << "\n";
	LogFile << "SessonNumber: \t\t\t" << m_SessonNumber << "\n";
	LogFile << "Count Of Players Collecting: \t" << m_CountPlayersCollecting << "\n";
	LogFile << "Count Of Players Losing: \t" << m_CountPlayersLosing << "\n \n";

	LogFile << "Avg Games In Session: \t\t" << m_AvgGamesInSession << "\n";

	LogFile << "Most Games In Collect Session:  " << m_MostGamesInWinSession << "\n";
	LogFile << "Least Games In Collect Session: " << m_LeastGamesInWinSession << "\n";

	LogFile << "Longest Losing Games Sequence: " << m_LongestLosingGameSession << "\n";
	LogFile << "Shortest Losing Games Sequence:" << m_ShortestLosingGameSession << "\n";
	LogFile << "Avg Games In Losing Session: \t\t" << m_AvgGamesInLosingSession << "\n";

	LogFile << "Highest Collect Amount: \t" << m_HighestCollectAmount << "\n";
	LogFile << "Lowest  Collect Amount: \t" << m_HighestCollectAmount << "\n";

	LogFile << "Highest Percentage Session: " << m_HighestSessionPercentage << "%\n";
	LogFile << "Lowest Percentage Session:  " << m_LowestSessionPercentage << "%\n";
	
	LogFile << "Total Games Played: \t\t" << m_TotalGames << "\n";
	LogFile << "Overall Percentage: \t\t" << m_Percentage << "%\n";

	LogFile.close();

	ClearFileAction(); //Always Clear post write.
}
