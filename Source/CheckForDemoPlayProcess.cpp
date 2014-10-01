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
			TheObjectHandler::Instance()->GetObject2D("DoorOpen")->SetVisible(true);

			if(!TheGame::Instance()->IsDemoMode())
			{
				if(TheEngine::Instance()->GetCurrentState()->GetName() == "Help")
				{
					TheEngine::Instance()->StateTransition("DealStart");
					return;
				}
				GET_BUTTONS->DisableHWButtons();
				
				GET_BUTTONS->SetOSButtonActivity(false, "CollectButton");
				GET_BUTTONS->SetOSButtonActivity(false, "HoldInfoButton");
				GET_BUTTONS->SetOSButtonActivity(false, "Hold2Button");
				GET_BUTTONS->SetOSButtonActivity(false, "Hold3Button");
				GET_BUTTONS->SetOSButtonActivity(false, "Hold4Button");
				GET_BUTTONS->SetOSButtonActivity(false, "HoldTransferButton");
				GET_BUTTONS->SetOSButtonActivity(false, "DealStart1PndButton");
				GET_BUTTONS->SetOSButtonActivity(false, "DealStart2PndButton");

				TheGame::Instance()->SetAutoplay(false);
				TheObjectHandler::Instance()->GetObject2D("RGDemoMode")->SetVisible(true);												
				TheObjectHandler::Instance()->GetObject2D("Demo")->SetVisible(true);
				TheObjectHandler::Instance()->GetText("WIN_TEXT")->SetVisible(false);
			}

			if(!mNoteValidatorFlag)
			{
				mNoteValidatorFlag = true;
				DisableNoteValidator();
			}

			if((GetCredits() < MINIMUM_BET))
			{
				TheGame::Instance()->SetAutoplay(false);
				AddDemoModeCredits();
			}

			if(GetDoorStatus() && !TheGame::Instance()->IsDemoMode())
			{
				TheGame::Instance()->SetAutoplay(false);
		
				if ((!GetDoorStatus() || GetSwitchStatus(REFILL_KEY)) && mNoteValidatorFlag)
				{
					mNoteValidatorFlag = false;		// Enable N/V before exiting demo mode!
					EnableNoteValidator();
					return;
				}

				if(ThePokerGame::Instance()->ReadYesDemoPb())
				{										
					TheGame::Instance()->SetDemoMode(true);
					AddDemoModeCredits();					
					TheObjectHandler::Instance()->GetObject2D("RGDemoMode")->SetVisible(false);					
					TheObjectHandler::Instance()->GetObject2D("Demo")->SetVisible(true);
					TheObjectHandler::Instance()->GetText("WIN_TEXT")->SetVisible(false);
					TheInput::Instance()->ResetMousePosition();
				}
			}
			mFinishedDemoGame = false;
		}

		if(!GetDoorStatus())
		{
			if(!mFinishedDemoGame)
			{			
				if(TheGame::Instance()->IsDemoMode())
				{
					TheGame::Instance()->SetDemoMode(false);			
					TheGame::Instance()->SetAutoplay(false);					
				}
				else
				{					
					TheObjectHandler::Instance()->GetObject2D("RGDemoMode")->SetVisible(false);					
				}

				TheObjectHandler::Instance()->GetObject2D("DoorOpen")->SetVisible(false);
				TheObjectHandler::Instance()->GetObject2D("Demo")->SetVisible(false);				
				TheObjectHandler::Instance()->GetText("WIN_TEXT")->SetVisible(true);
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
					if(TheGame::Instance()->IsDemoMode())
					{
						TheGame::Instance()->SetAutoplay(false);
					}
				}
			}
		}
	}
}

void CheckForDemoPlayProcess::Complete()
{

}

void CheckForDemoPlayProcess::AddDemoModeCredits()
{
	if(GetDoorStatus() && (GetCredits() < TheGame::Instance()->GetStake())
		&& !GetSwitchStatus(REFILL_KEY) && TheGame::Instance()->IsDemoMode())
	{
		AddToDemoCredits(2000);
	}
}