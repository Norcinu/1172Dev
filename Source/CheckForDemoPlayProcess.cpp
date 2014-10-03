#include "CheckForDemoPlayProcess.h"
#include "Game.h"
#include "Defines.h"
#include "PokerGame.h"

CheckForDemoPlayProcess::CheckForDemoPlayProcess()
{
	mNoteValidatorFlag = false;
	mFinishedDemoGame = true;
}

CheckForDemoPlayProcess::~CheckForDemoPlayProcess()
{

}

void CheckForDemoPlayProcess::Init()
{
	GameProcess::Init();
}

void CheckForDemoPlayProcess::Update()
{	
	PROFILE(__FUNCTION__);
	if(!GetCurrentError() && !GetGameInProgress())
	{
		if(GetDoorStatus())
		{
			OBJECT_HANDLER->GetObject2D("DoorOpen")->SetVisible(true);

			if(!THE_GAME->IsDemoMode())
			{
				if(ENGINE->GetCurrentState()->GetName() == "Help")
				{
					ENGINE->StateTransition("DealStart");
					return;
				}
				THE_BUTTONS->DisableHWButtons();
				
				THE_BUTTONS->SetOSButtonActivity(false, "CollectButton");
				THE_BUTTONS->SetOSButtonActivity(false, "HoldInfoButton");
				THE_BUTTONS->SetOSButtonActivity(false, "Hold2Button");
				THE_BUTTONS->SetOSButtonActivity(false, "Hold3Button");
				THE_BUTTONS->SetOSButtonActivity(false, "Hold4Button");
				THE_BUTTONS->SetOSButtonActivity(false, "HoldTransferButton");
				THE_BUTTONS->SetOSButtonActivity(false, "DealStart1PndButton");
				THE_BUTTONS->SetOSButtonActivity(false, "DealStart2PndButton");

				THE_GAME->SetAutoplay(false);
				OBJECT_HANDLER->GetObject2D("RGDemoMode")->SetVisible(true);												
				OBJECT_HANDLER->GetObject2D("Demo")->SetVisible(true);
				OBJECT_HANDLER->GetText("WIN_TEXT")->SetVisible(false);
			}

			if(!mNoteValidatorFlag)
			{
				mNoteValidatorFlag = true;
				DisableNoteValidator();
			}

			if((GetCredits() < MINIMUM_BET))
			{
				THE_GAME->SetAutoplay(false);
				AddDemoModeCredits();
			}

			if(GetDoorStatus() && !THE_GAME->IsDemoMode())
			{
				THE_GAME->SetAutoplay(false);
		
				if ((!GetDoorStatus() || GetSwitchStatus(REFILL_KEY)) && mNoteValidatorFlag)
				{
					mNoteValidatorFlag = false;		// Enable N/V before exiting demo mode!
					EnableNoteValidator();
					return;
				}

				if(ThePokerGame::Instance()->ReadYesDemoPb())
				{										
					THE_GAME->SetDemoMode(true);
					AddDemoModeCredits();					
					OBJECT_HANDLER->GetObject2D("RGDemoMode")->SetVisible(false);					
					OBJECT_HANDLER->GetObject2D("Demo")->SetVisible(true);
					OBJECT_HANDLER->GetText("WIN_TEXT")->SetVisible(false);
					TheInput::Instance()->ResetMousePosition();
				}
			}
			mFinishedDemoGame = false;
		}

		if(!GetDoorStatus())
		{
			if(!mFinishedDemoGame)
			{			
				if(THE_GAME->IsDemoMode())
				{
					THE_GAME->SetDemoMode(false);			
					THE_GAME->SetAutoplay(false);					
				}
				else
				{					
					OBJECT_HANDLER->GetObject2D("RGDemoMode")->SetVisible(false);					
				}

				OBJECT_HANDLER->GetObject2D("DoorOpen")->SetVisible(false);
				OBJECT_HANDLER->GetObject2D("Demo")->SetVisible(false);				
				OBJECT_HANDLER->GetText("WIN_TEXT")->SetVisible(true);
				mFinishedDemoGame = true;
			}

			
			if(mNoteValidatorFlag)
			{
				if(GetDoorStatus() || GetCurrentError() || GetCredits() >= GetVariableValue(MAX_CREDITS))
				{
					DisableNoteValidator();
				}

				else
				{
					EnableNoteValidator();
					mNoteValidatorFlag = false;
					if(THE_GAME->IsDemoMode())
					{
						THE_GAME->SetAutoplay(false);
					}
				}
			}
		}


		/*if(GetCredits() + GetBankDeposit() >= THE_GAME->GetStake())
		{
			TheEngine::Instance()->StateTransition("DealStart");
		}
		else
		{
			if(GetTerminalFormat() > 1 && !(GetCredits() + GetBankDeposit()))
			{
				TheEngine::Instance()->StateTransition("CompendiumLoad");
			}
			else
			{
				TheAudioManager::Instance()->GetAudioSample("SX_NOCREDIT")->Play();
				TheEngine::Instance()->StateTransition("Standby");
			}
		}*/

	}
}

void CheckForDemoPlayProcess::Complete()
{

}

void CheckForDemoPlayProcess::AddDemoModeCredits()
{
	if(GetDoorStatus() && (GetCredits() < THE_GAME->GetStake())
		&& !GetSwitchStatus(REFILL_KEY) && THE_GAME->IsDemoMode())
	{
		AddToDemoCredits(2000);
	}
}