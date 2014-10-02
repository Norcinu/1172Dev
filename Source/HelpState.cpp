#include <ObjectHandler.h>
#include <TextureManager.h>
#include <sstream>
#include "GenericButtons.h"
#include "Defines.h"
#include "Game.h"
#include "HelpState.h"
#include "PokerGame.h"

static bool Registered = TheEngine::Instance()->AddState("Help", new HelpState());

extern unsigned char global_quit;

HelpState::HelpState()
{
	m_timeOut = 0.0f;
	mTotalWonVisible = false;
}

HelpState::~HelpState()
{

}

void HelpState::Enter()
{
	THE_BUTTONS->DisableHWButtons();		
	mhelpScreenID = TheTextureManager::Instance()->GetTextureID("RGInfoPage01.png");	
	OBJECT_HANDLER->GetObject2D("Help")->SetTextureID(mhelpScreenID);	
	
	mhelpButtonID = TheTextureManager::Instance()->GetTextureID("LegendExitLit.png");
	OBJECT_HANDLER->GetObject2D("LegendButton1Lit")->SetTextureID(mhelpButtonID);	

	mhelpButtonID = TheTextureManager::Instance()->GetTextureID("LegendExitNlit.png");
	OBJECT_HANDLER->GetObject2D("LegendButton1Nlit")->SetTextureID(mhelpButtonID);
		
	if (NUM_INFO_SCREEN > 1)
	{
		mhelpButtonID = TheTextureManager::Instance()->GetTextureID("LegendForwardLit.png");
		OBJECT_HANDLER->GetObject2D("LegendButton2Lit")->SetTextureID(mhelpButtonID);	

		mhelpButtonID = TheTextureManager::Instance()->GetTextureID("LegendForwardNlit.png");
		OBJECT_HANDLER->GetObject2D("LegendButton2Nlit")->SetTextureID(mhelpButtonID);
	}
	
	//Set Visibility Off
	THE_BUTTONS->SetOSButtonActivity(false, "AutoplayButton",NO_LEGEND);
	THE_BUTTONS->SetOSButtonActivity(false, "CollectButton",NO_LEGEND);
	THE_BUTTONS->SetOSButtonActivity(false, "HoldInfoButton",NO_LEGEND);
	THE_BUTTONS->SetOSButtonActivity(false, "Hold2Button",NO_LEGEND);
	THE_BUTTONS->SetOSButtonActivity(false, "Hold3Button",NO_LEGEND);
	THE_BUTTONS->SetOSButtonActivity(false, "Hold4Button",NO_LEGEND);
	THE_BUTTONS->SetOSButtonActivity(false, "HoldTransferButton",NO_LEGEND);
	THE_BUTTONS->SetOSButtonActivity(false, "DealStart1PndButton",NO_LEGEND);
	THE_BUTTONS->SetOSButtonActivity(false, "DealStart2PndButton",NO_LEGEND);

	OBJECT_HANDLER->GetObject2D("GraphicalButton01")->SetVisible(false);
	OBJECT_HANDLER->GetObject2D("GraphicalButton02")->SetVisible(false);
	OBJECT_HANDLER->GetObject2D("GraphicalButton03")->SetVisible(false);
	OBJECT_HANDLER->GetObject2D("GraphicalButton04")->SetVisible(false);
	OBJECT_HANDLER->GetObject2D("GraphicalButton05")->SetVisible(false);
	OBJECT_HANDLER->GetObject2D("GraphicalButton06")->SetVisible(false);
	OBJECT_HANDLER->GetObject2D("GraphicalButton07")->SetVisible(false);
	OBJECT_HANDLER->GetObject2D("GraphicalButton08")->SetVisible(false);
	OBJECT_HANDLER->GetObject2D("GraphicalButton09")->SetVisible(false);
	
	//Set Visibility On
	OBJECT_HANDLER->GetObject2D("Help")->SetVisible(true);
	OBJECT_HANDLER->GetObject2D("GraphicalButton01")->SetVisible(true);
	THE_BUTTONS->SetOSButtonActivity(true, "InfoButton1",LAMP_ON);
	if (NUM_INFO_SCREEN > 1)
	{
		OBJECT_HANDLER->GetObject2D("GraphicalButton02")->SetVisible(true);
		THE_BUTTONS->SetOSButtonActivity(true, "InfoButton2",LAMP_ON);
	}
	
	ThePokerGame::Instance()->UpdateInfoDigits(true);
	
	mHelpScreenNumber = 1;
	m_timeOut = TheEngine::Instance()->GetSystemTimer().GetRunningTime() + 30.0f;
}

void HelpState::Exit()
{	
	//Set Visibility Off
	ThePokerGame::Instance()->UpdateInfoDigits(false);
	ThePokerGame::Instance()->UpdateGoldenDigits(false);
	OBJECT_HANDLER->GetObject2D("Help")->SetVisible(false);
	THE_BUTTONS->SetOSButtonActivity(false, "InfoButton1",NO_LEGEND);
	THE_BUTTONS->SetOSButtonActivity(false, "InfoButton2",NO_LEGEND);
	THE_BUTTONS->SetOSButtonActivity(false, "InfoButton3",NO_LEGEND);

	//Set Visibility On
	THE_BUTTONS->SetOSButtonActivity(false, "AutoplayButton");
	THE_BUTTONS->SetOSButtonActivity(false, "CollectButton");
	THE_BUTTONS->SetOSButtonActivity(false, "HoldInfoButton");
	THE_BUTTONS->SetOSButtonActivity(false, "Hold2Button");
	THE_BUTTONS->SetOSButtonActivity(false, "Hold3Button");
	THE_BUTTONS->SetOSButtonActivity(false, "Hold4Button");
	THE_BUTTONS->SetOSButtonActivity(false, "HoldTransferButton");
	THE_BUTTONS->SetOSButtonActivity(false, "DealStart1PndButton");
	THE_BUTTONS->SetOSButtonActivity(false, "DealStart2PndButton");

	OBJECT_HANDLER->GetObject2D("GraphicalButton01")->SetVisible(true);
	OBJECT_HANDLER->GetObject2D("GraphicalButton02")->SetVisible(true);
	OBJECT_HANDLER->GetObject2D("GraphicalButton03")->SetVisible(true);
	OBJECT_HANDLER->GetObject2D("GraphicalButton04")->SetVisible(true);
	OBJECT_HANDLER->GetObject2D("GraphicalButton05")->SetVisible(true);
	OBJECT_HANDLER->GetObject2D("GraphicalButton06")->SetVisible(true);
	OBJECT_HANDLER->GetObject2D("GraphicalButton07")->SetVisible(true);
	OBJECT_HANDLER->GetObject2D("GraphicalButton08")->SetVisible(true);
	OBJECT_HANDLER->GetObject2D("GraphicalButton09")->SetVisible(true);
			
}

void HelpState::Update()
{
bool RedrawHelpScreenFlag=false;

	if(GetCurrentError())
	{
		return;
	}

	if(!global_quit)
	{
		if(TheEngine::Instance()->GetSystemTimer().GetRunningTime() > m_timeOut)
		{
			if(GetCredits() >= MINIMUM_BET)
			{
				TheEngine::Instance()->StateTransition("DealStart");
			}
			else
			{
				TheEngine::Instance()->StateTransition("Standby");
			}
		}

		if(THE_BUTTONS->OSButtonPressed("InfoButton1")) //Exit Button
		{
			if(GetCredits() >= MINIMUM_BET)
			{
				TheEngine::Instance()->StateTransition("DealStart");
			}
			else
			{
				TheEngine::Instance()->StateTransition("Standby");
			}
		}


		if (mHelpScreenNumber == 1)
		{
			if(THE_BUTTONS->OSButtonPressed("InfoButton2")) //Forward Button
			{
				mHelpScreenNumber++;
				RedrawHelpScreenFlag = true;
			}
		}

		if (mHelpScreenNumber > 1 && mHelpScreenNumber < NUM_INFO_SCREEN)
		{
			if(THE_BUTTONS->OSButtonPressed("InfoButton2")) //Forward Button
			{
				mHelpScreenNumber++;
				RedrawHelpScreenFlag = true;
			}

			if(THE_BUTTONS->OSButtonPressed("InfoButton3")) //Backward Button
			{
				mHelpScreenNumber--;
				RedrawHelpScreenFlag = true;
			}
		}

		if (mHelpScreenNumber == NUM_INFO_SCREEN)
		{
			if(THE_BUTTONS->OSButtonPressed("InfoButton2")) //Backward Button
			{
				mHelpScreenNumber--;
				RedrawHelpScreenFlag = true;
			}
		}

		if (RedrawHelpScreenFlag)
		{
			ThePokerGame::Instance()->UpdateInfoDigits(false);
			ThePokerGame::Instance()->UpdateGoldenDigits(false);

			m_timeOut = TheEngine::Instance()->GetSystemTimer().GetRunningTime() + 30.0f;				
			OBJECT_HANDLER->GetObject2D("Help")->SetTextureID(mhelpScreenID+mHelpScreenNumber-1);

			if (mHelpScreenNumber == 1)
			{				
				OBJECT_HANDLER->GetObject2D("GraphicalButton03")->SetVisible(false);
				THE_BUTTONS->SetOSButtonActivity(false, "InfoButton3",NO_LEGEND);

				THE_BUTTONS->SetOSButtonActivity(false, "InfoButton2",NO_LEGEND);
				mhelpButtonID = TheTextureManager::Instance()->GetTextureID("LegendForwardLit.png");
				OBJECT_HANDLER->GetObject2D("LegendButton2Lit")->SetTextureID(mhelpButtonID);	

				mhelpButtonID = TheTextureManager::Instance()->GetTextureID("LegendForwardNlit.png");
				OBJECT_HANDLER->GetObject2D("LegendButton2Nlit")->SetTextureID(mhelpButtonID);
				THE_BUTTONS->SetOSButtonActivity(true, "InfoButton2",LAMP_ON);

				ThePokerGame::Instance()->UpdateInfoDigits(true);
			}

			if (mHelpScreenNumber > 1 && mHelpScreenNumber < NUM_INFO_SCREEN)
			{
				THE_BUTTONS->SetOSButtonActivity(false, "InfoButton2",NO_LEGEND);
				mhelpButtonID = TheTextureManager::Instance()->GetTextureID("LegendForwardLit.png");
				OBJECT_HANDLER->GetObject2D("LegendButton2Lit")->SetTextureID(mhelpButtonID);	

				mhelpButtonID = TheTextureManager::Instance()->GetTextureID("LegendForwardNlit.png");
				OBJECT_HANDLER->GetObject2D("LegendButton2Nlit")->SetTextureID(mhelpButtonID);
				THE_BUTTONS->SetOSButtonActivity(true, "InfoButton2",LAMP_ON);

				THE_BUTTONS->SetOSButtonActivity(false, "InfoButton3",NO_LEGEND);
				mhelpButtonID = TheTextureManager::Instance()->GetTextureID("LegendBackwardLit.png");
				OBJECT_HANDLER->GetObject2D("LegendButton3Lit")->SetTextureID(mhelpButtonID);	

				mhelpButtonID = TheTextureManager::Instance()->GetTextureID("LegendBackwardNlit.png");
				OBJECT_HANDLER->GetObject2D("LegendButton3Nlit")->SetTextureID(mhelpButtonID);
				OBJECT_HANDLER->GetObject2D("GraphicalButton03")->SetVisible(true);
				THE_BUTTONS->SetOSButtonActivity(true, "InfoButton3",LAMP_ON);
			}
			
			if (mHelpScreenNumber == NUM_INFO_SCREEN)
			{
				THE_BUTTONS->SetOSButtonActivity(false, "InfoButton2",NO_LEGEND);
				mhelpButtonID = TheTextureManager::Instance()->GetTextureID("LegendBackwardLit.png");
				OBJECT_HANDLER->GetObject2D("LegendButton2Lit")->SetTextureID(mhelpButtonID);	

				mhelpButtonID = TheTextureManager::Instance()->GetTextureID("LegendBackwardNlit.png");
				OBJECT_HANDLER->GetObject2D("LegendButton2Nlit")->SetTextureID(mhelpButtonID);
				THE_BUTTONS->SetOSButtonActivity(true, "InfoButton2",LAMP_ON);

				OBJECT_HANDLER->GetObject2D("GraphicalButton03")->SetVisible(false);
				THE_BUTTONS->SetOSButtonActivity(false, "InfoButton3",NO_LEGEND);
			}
			
			if (mHelpScreenNumber == 2)
				ThePokerGame::Instance()->UpdateGoldenDigits(true);
		}				
	}
}



