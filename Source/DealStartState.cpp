#include <ObjectHandler.h>
#include <TextureManager.h>
#include "GenericButtons.h"
#include "CollectProcess.h"
#include "Defines.h"
#include "Game.h"
#include "DealStartState.h"
#include "MaxBankProcess.h"
#include "CollectProcess.h"

#include "NMI_log.h"
#include "JackpotProcess.h"

#include "PokerGame.h"

static bool Registered = TheEngine::Instance()->AddState("DealStart", new DealStartState());

extern unsigned char global_quit;

DealStartState::DealStartState()
{
	lCredit = 0;
	mHelpActiveDelayTimer = 0.0f;
}

DealStartState::~DealStartState()
{

}

void DealStartState::Enter()
{
#ifdef SOAK_BUILD
	if (GetLocalCharRandomNumber(100) < 50)
	{
		TheGame::Instance()->SetStake(MAXIMUM_BET);
		ThePokerGame::Instance()->SetGameIndex(Game200p);
	}
	else
	{
		TheGame::Instance()->SetStake(MINIMUM_BET);
		ThePokerGame::Instance()->SetGameIndex(Game100p);
	}

	if (GetCredits()+GetBankDeposit() <= MINIMUM_BET)
	{
		TheGame::Instance()->SetStake(MINIMUM_BET);
		ThePokerGame::Instance()->SetGameIndex(Game100p);
	}
#endif

	ThePokerGame::Instance()->SetPointerAndMode();

	ThePokerGame::Instance()->DisplayStake();

	ThePokerGame::Instance()->AllowHighWinFlag = 0;

	THE_BUTTONS->SetOSButtonActivity(false, "HiButton",NO_LEGEND);
	THE_BUTTONS->SetOSButtonActivity(false, "LoButton",NO_LEGEND);
	THE_BUTTONS->SetOSButtonActivity(false, "Swop2PndButton",NO_LEGEND);
	THE_BUTTONS->SetOSButtonActivity(false, "Swop1PndButton",NO_LEGEND);

	THE_BUTTONS->SetOSButtonActivity(false, "Hold2Button");
	THE_BUTTONS->SetOSButtonActivity(false, "Hold3Button");
	THE_BUTTONS->SetOSButtonActivity(false, "Hold4Button");
	THE_BUTTONS->SetOSButtonActivity(true, "DealStart2PndButton");
	THE_BUTTONS->SetOSButtonActivity(true, "DealStart1PndButton");

	if (ThePokerGame::Instance()->ResetAutoPlayFlag)
	{
		TheGame::Instance()->SetAutoplay(true);
		ThePokerGame::Instance()->ResetAutoPlayFlag = 0;
	}

	if((GetCredits() > lCredit || GetCredits() < TheGame::Instance()->GetStake()))	
		TheGame::Instance()->SetAutoplay(false);
		
	TheEngine::Instance()->GetProcessManager()->FlushProcessQueue();

	TheGame::Instance()->UpdateDigits();

	if(GetBankDeposit()>= GetVariableValue(MAX_WIN_BANK))
	{
#ifdef SOAK_BUILD
		ZeroBankDeposit();
#else
		TheEngine::Instance()->GetProcessManager()->AddProcessToQueue(new MaxBankProcess);
#endif
	}

	mHelpActiveDelayTimer = 0;
	
}

void DealStartState::Exit()
{
	ThePokerGame::Instance()->FinalWinValue = 0;

	for (int i=0;i<LEVEL;i++)
		ThePokerGame::Instance()->SetAwardValueLitState(i,MLAMP_OFF);

	THE_BUTTONS->SetButtonActivity(false, "FrontStart");
	THE_BUTTONS->SetButtonActivity(false, "Stake");
	THE_BUTTONS->SetButtonActivity(false, "TopStart");
	THE_BUTTONS->SetButtonActivity(false, "Menu");
	THE_BUTTONS->SetButtonActivity(false, "Collect");
	THE_BUTTONS->SetButtonActivity(false, "Transfer");
	if(!TheGame::Instance()->GetAutoplay())
	{
		THE_BUTTONS->SetButtonActivity(false, "AutoPlay");
	}

	THE_BUTTONS->SetOSButtonActivity(false, "HoldTransferButton");
	THE_BUTTONS->SetOSButtonActivity(false, "DealStart2PndButton");
	THE_BUTTONS->SetOSButtonActivity(false, "DealStart1PndButton");
	THE_BUTTONS->SetOSButtonActivity(false, "CollectButton");
	THE_BUTTONS->SetOSButtonActivity(false, "HoldTransferButton");
	THE_BUTTONS->SetOSButtonActivity(false, "HoldInfoButton");

	OBJECT_HANDLER->GetObject2D("DealStartMsg")->SetVisible(false);	
#ifndef FAST_PLAY
	TheEngine::Instance()->SetScreensToDraw(SCREEN1);
#endif

	lCredit = GetCredits() - TheGame::Instance()->GetStake(); //subtract stake from next spin	
}

#include <sstream>

void DealStartState::Update()
{	
	
	PROFILE(__FUNCTION__);	

	if(TheEngine::Instance()->GetProcessManager()->GetNumQueueProcesses())
	{
		return;
	}

	if(GetCurrentError() && !GetGameInProgress())
	{
		return;
	}

	if(GetDoorStatus() && !TheGame::Instance()->IsDemoMode())
	{
		return;
	}

	if(!ThePokerGame::Instance()->GetTransferBanktoCreditTimeElaspsed())
	{
		return;
	}

	if(!GetGameInProgress()) //cannot exit in FS game
	{
		if(GetSwitchStatus(REFILL_KEY))
		{
			TheGame::Instance()->QuitToMainMenu();
			return;
		}
	}

	if(!global_quit)
	{		
		OBJECT_HANDLER->GetObject2D("DealStartMsg")->SetVisible(true);

		THE_BUTTONS->DealStartButtons(mHelpActiveDelayTimer);

#ifdef SOAK_BUILD
		if((GetCredits() < MAXIMUM_BET) && GetBankDeposit())
		{
			TransferFromBankToCredits();
		}
#endif

		if(!GetCurrentError())
		{
			TheGame::Instance()->UpdateDigits();
			
			if((GetCredits()+GetBankDeposit() < MINIMUM_BET))
			{				
				TheGame::Instance()->SetAutoplay(false);
				TheEngine::Instance()->StateTransition("Standby");
				return;
			}
			
			bool changeStake = false;
			bool startGame = false;
			if(TheGame::Instance()->GetStake() == MINIMUM_BET)
			{
				if (THE_BUTTONS->ButtonPressed("TopStart") ||
				    THE_BUTTONS->ButtonPressed("FrontStart") ||
				    THE_BUTTONS->OSButtonPressed("DealStart1PndButton"))				
				{					
					startGame = true;
				}
				else if (THE_BUTTONS->ButtonPressed("Stake") ||
					     THE_BUTTONS->OSButtonPressed("DealStart2PndButton"))
				{
					changeStake = true;
				}
			}
			else
			{
				if(THE_BUTTONS->ButtonPressed("FrontStart") ||
				   THE_BUTTONS->ButtonPressed("TopStart") ||
				   THE_BUTTONS->OSButtonPressed("DealStart2PndButton"))
				{					
					startGame = true;
				}
				else if(THE_BUTTONS->ButtonPressed("Stake") ||
					    THE_BUTTONS->OSButtonPressed("DealStart1PndButton"))
				{
					changeStake = true;
				}
			}
			
			if(THE_BUTTONS->ButtonPressed("Transfer") ||
			   THE_BUTTONS->OSButtonPressed("HoldTransferButton"))
			{
				ThePokerGame::Instance()->SetTransferBankToCredits(true);
			}
#ifdef SOAK_BUILD
			if(GetCredits() >= TheGame::Instance()->GetStake())
#else			
			if((THE_BUTTONS->ButtonPressed("FrontStart") || TheGame::Instance()->GetAutoplay() || 
				startGame) && (GetCredits() >= TheGame::Instance()->GetStake()))
#endif
			{

				SetGameInProgress();
				TheGame::Instance()->ClearFinalWinValue();

				if(TheGame::Instance()->GetStake() == MINIMUM_BET)
					ThePokerGame::Instance()->WageredAmount = MINIMUM_BET;
				else
					ThePokerGame::Instance()->WageredAmount = MAXIMUM_BET;

				ThePokerGame::Instance()->AllowHighWinFlag = ThePokerGame::Instance()->AllowHighWins();
				if (ThePokerGame::Instance()->AllowHighWinFlag)
					ThePokerGame::Instance()->AllowedHighWin = ThePokerGame::Instance()->SelectWinType();

				ThePokerGame::Instance()->SetGameStartTime();

				TheEngine::Instance()->StateTransition("Cointrol");
			}
			else if((THE_BUTTONS->ButtonPressed("AutoPlay") || THE_BUTTONS->OSButtonPressed("AutoplayButton")) && 
				     !TheGame::Instance()->GetAutoplay())
			{				
				TheGame::Instance()->SetAutoplay(true);
			}
			else if((THE_BUTTONS->ButtonPressed("Collect") || THE_BUTTONS->OSButtonPressed("CollectButton")) && (GetCredits()+GetBankDeposit()>0))
			{
				TheEngine::Instance()->GetProcessManager()->AddProcessToQueue(new CollectProcess);
				return;
			}
			else if(changeStake)
			{
				if(TheGame::Instance()->GetStake() == MINIMUM_BET)
				{					
					TheGame::Instance()->SetStake(MAXIMUM_BET);
					ThePokerGame::Instance()->SetGameIndex(Game200p);
				}
				else if(TheGame::Instance()->GetStake() == MAXIMUM_BET)
				{					
					TheGame::Instance()->SetStake(MINIMUM_BET);
					ThePokerGame::Instance()->SetGameIndex(Game100p);
				}	

				ThePokerGame::Instance()->DisplayStake();
			}
			else if(THE_BUTTONS->OSButtonPressed("HoldInfoButton"))
			{				
				TheEngine::Instance()->StateTransition("Help");
			}
			else if(THE_BUTTONS->ButtonPressed("Menu"))
			{
				TheGame::Instance()->QuitToMainMenu();
			}
		}
	}
}
