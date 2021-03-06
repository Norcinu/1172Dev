/*
Game.h 2012
Hanldes all aspects of the game. interfaces with Engine class
*/
#ifndef _GAME_H
#define _GAME_H

#include <AudioManager.h>
#include <Engine.h>
#include <GenericGame.h>
#include <ObjectHandler.h>
#include <Input.h>
#include <NonCopyable.h>
#include <Singleton.h>
#include <memory>
#include <vector>

#include "Defines.h"
#include "GenericButtons.h"

class PopControl;

using std::shared_ptr;

class Game : public GenericGame
{
private:
	Game(void);
	friend class Singleton<Game>;
public:
	~Game(void);
	void ComputeWinDoorOpen();
	void ComputeWinLive();
	void DrawCurrentError();
	void QuitToMainMenu();
	void QuitToMorningReboot();
	void GameShutDown();
	void RestoreData();
	void Roll();
	void SetDemoMode(bool demoMode);
	void SetAutoplay(bool autoplay);
	void SetDeduct(unsigned int deduct);
	void SetFeatureWinValue(unsigned int featureWinValue);
	void SetFinalWinValue(unsigned int finalWinValue);
	void SetInterimFeatureWinValue(unsigned int interimFeatureWinValue);
	void SetInterimWinValue(unsigned int interimWinValue);
	void IncrementFeatureWinValue(int featureWinValue);
	void IncrementFinalWinValue(int finalWinValue);
	void IncrementInterimFeatureWinValue(int interimFeatureWinValue);
	void IncrementInterimWinValue(int interimWinValue);
	void ClearFeatureWinValue();
	void ClearFinalWinValue();
	void ClearInterimFeatureWinValue();
	void ClearInterimWinValue();
	void SetStake(unsigned int stake);
	void StoreData();
	bool IsDemoMode();
	void UpdateDigits();
	bool CollectFromHopper(unsigned char type);
	bool GetAutoplay() const;
	bool Setup(HINSTANCE hinstance);
	int AwardAPJPot();
	unsigned int GetDeduct() const;
	unsigned int GetFeatureWinValue() const;
	unsigned int GetFinalWinValue() const;
	unsigned int GetInterimFeatureWinValue() const;
	unsigned int GetInterimWinValue() const;
	
	bool IsHD();
	bool HasStake(const unsigned int stake) const;
	void TogglePopOptions();
	void ClosePopOptions();
	bool Game::ArePopOptionsVisible() const;

	int GetStakeID() const;
	unsigned int GetNextStake() const;

	//ProcessManager m_PManager;
private:
	bool GameInit();
	bool GetPercentage(unsigned int stake, float* percentage);
	static void _cdecl BeginStoreDataThread(void* Param);
	void StoreDataThread();
	
private:
	bool m_autoplay;
	bool mDemoMode;	
	bool m_isHD;

	shared_ptr<PopControl> m_popControl;

	unsigned char mStoringThreadActive;

	unsigned int m_currentTotalStake[TOTAL_STAKES];
	unsigned int m_currentTotalWon[TOTAL_STAKES];
	unsigned int m_deduct;
	unsigned int m_featureWinValue;
	unsigned int m_finalWinValue;
	
	unsigned int m_interimFeatureWinValue;
	unsigned int m_interimWinValue;

	unsigned int mTargetValue;

	std::vector<unsigned int> m_gameStakes;
};

typedef Singleton<Game> TheGame;
#define THE_GAME TheGame::Instance()

#endif

