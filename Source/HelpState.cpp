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
	TheButtons::Instance()->DisableHWButtons();		
	mhelpScreenID = TheTextureManager::Instance()->GetTextureID("RGInfoPage01.png");	
	TheObjectHandler::Instance()->GetObject2D("Help")->SetTextureID(mhelpScreenID);	
	
	mhelpButtonID = TheTextureManager::Instance()->GetTextureID("LegendExitLit.png");
	TheObjectHandler::Instance()->GetObject2D("LegendButton1Lit")->SetTextureID(mhelpButtonID);	

	mhelpButtonID = TheTextureManager::Instance()->GetTextureID("LegendExitNlit.png");
	TheObjectHandler::Instance()->GetObject2D("LegendButton1Nlit")->SetTextureID(mhelpButtonID);
		
	if (NUM_INFO_SCREEN > 1)
	{
		mhelpButtonID = TheTextureManager::Instance()->GetTextureID("LegendForwardLit.png");
		TheObjectHandler::Instance()->GetObject2D("LegendButton2Lit")->SetTextureID(mhelpButtonID);	

		mhelpButtonID = TheTextureManager::Instance()->GetTextureID("LegendForwardNlit.png");
		TheObjectHandler::Instance()->GetObject2D("LegendButton2Nlit")->SetTextureID(mhelpButtonID);
	}
	
	//Set Visibility Off
	TheButtons::Instance()->SetOSButtonActivity(false, "AutoplayButton",NO_LEGEND);
	TheButtons::Instance()->SetOSButtonActivity(false, "CollectButton",NO_LEGEND);
	TheButtons::Instance()->SetOSButtonActivity(false, "HoldInfoButton",NO_LEGEND);
	TheButtons::Instance()->SetOSButtonActivity(false, "Hold2Button",NO_LEGEND);
	TheButtons::Instance()->SetOSButtonActivity(false, "Hold3Button",NO_LEGEND);
	TheButtons::Instance()->SetOSButtonActivity(false, "Hold4Button",NO_LEGEND);
	TheButtons::Instance()->SetOSButtonActivity(false, "HoldTransferButton",NO_LEGEND);
	TheButtons::Instance()->SetOSButtonActivity(false, "DealStart1PndButton",NO_LEGEND);
	TheButtons::Instance()->SetOSButtonActivity(false, "DealStart2PndButton",NO_LEGEND);

	TheObjectHandler::Instance()->GetObject2D("GraphicalButton01")->SetVisible(false);
	TheObjectHandler::Instance()->GetObject2D("GraphicalButton02")->SetVisible(false);
	TheObjectHandler::Instance()->GetObject2D("GraphicalButton03")->SetVisible(false);
	TheObjectHandler::Instance()->GetObject2D("GraphicalButton04")->SetVisible(false);
	TheObjectHandler::Instance()->GetObject2D("GraphicalButton05")->SetVisible(false);
	TheObjectHandler::Instance()->GetObject2D("GraphicalButton06")->SetVisible(false);
	TheObjectHandler::Instance()->GetObject2D("GraphicalButton07")->SetVisible(false);
	TheObjectHandler::Instance()->GetObject2D("GraphicalButton08")->SetVisible(false);
	TheObjectHandler::Instance()->GetObject2D("GraphicalButton09")->SetVisible(false);

	//Set Visibility On
	TheObjectHandler::Instance()->GetObject2D("Help")->SetVisible(true);
	TheObjectHandler::Instance()->GetObject2D("GraphicalButton01")->SetVisible(true);
	TheButtons::Instance()->SetOSButtonActivity(true, "InfoButton1",LAMP_ON);
	if (NUM_INFO_SCREEN > 1)
	{
		TheObjectHandler::Instance()->GetObject2D("GraphicalButton02")->SetVisible(true);
		TheButtons::Instance()->SetOSButtonActivity(true, "InfoButton2",LAMP_ON);
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
	TheObjectHandler::Instance()->GetObject2D("Help")->SetVisible(false);
	TheButtons::Instance()->SetOSButtonActivity(false, "InfoButton1",NO_LEGEND);
	TheButtons::Instance()->SetOSButtonActivity(false, "InfoButton2",NO_LEGEND);
	TheButtons::Instance()->SetOSButtonActivity(false, "InfoButton3",NO_LEGEND);

	//Set Visibility On
	TheButtons::Instance()->SetOSButtonActivity(false, "AutoplayButton");
	TheButtons::Instance()->SetOSButtonActivity(false, "CollectButton");
	TheButtons::Instance()->SetOSButtonActivity(false, "HoldInfoButton");
	TheButtons::Instance()->SetOSButtonActivity(false, "Hold2Button");
	TheButtons::Instance()->SetOSButtonActivity(false, "Hold3Button");
	TheButtons::Instance()->SetOSButtonActivity(false, "Hold4Button");
	TheButtons::Instance()->SetOSButtonActivity(false, "HoldTransferButton");
	TheButtons::Instance()->SetOSButtonActivity(false, "DealStart1PndButton");
	TheButtons::Instance()->SetOSButtonActivity(false, "DealStart2PndButton");

	TheObjectHandler::Instance()->GetObject2D("GraphicalButton01")->SetVisible(true);
	TheObjectHandler::Instance()->GetObject2D("GraphicalButton02")->SetVisible(true);
	TheObjectHandler::Instance()->GetObject2D("GraphicalButton03")->SetVisible(true);
	TheObjectHandler::Instance()->GetObject2D("GraphicalButton04")->SetVisible(true);
	TheObjectHandler::Instance()->GetObject2D("GraphicalButton05")->SetVisible(true);
	TheObjectHandler::Instance()->GetObject2D("GraphicalButton06")->SetVisible(true);
	TheObjectHandler::Instance()->GetObject2D("GraphicalButton07")->SetVisible(true);
	TheObjectHandler::Instance()->GetObject2D("GraphicalButton08")->SetVisible(true);
	TheObjectHandler::Instance()->GetObject2D("GraphicalButton09")->SetVisible(true);
			
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

		if(TheButtons::Instance()->OSButtonPressed("InfoButton1")) //Exit Button
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
			if(TheButtons::Instance()->OSButtonPressed("InfoButton2")) //Forward Button
			{
				mHelpScreenNumber++;
				RedrawHelpScreenFlag = true;
			}
		}

		if (mHelpScreenNumber > 1 && mHelpScreenNumber < NUM_INFO_SCREEN)
		{
			if(TheButtons::Instance()->OSButtonPressed("InfoButton2")) //Forward Button
			{
				mHelpScreenNumber++;
				RedrawHelpScreenFlag = true;
			}

			if(TheButtons::Instance()->OSButtonPressed("InfoButton3")) //Backward Button
			{
				mHelpScreenNumber--;
				RedrawHelpScreenFlag = true;
			}
		}

		if (mHelpScreenNumber == NUM_INFO_SCREEN)
		{
			if(TheButtons::Instance()->OSButtonPressed("InfoButton2")) //Backward Button
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
			TheObjectHandler::Instance()->GetObject2D("Help")->SetTextureID(mhelpScreenID+mHelpScreenNumber-1);

			if (mHelpScreenNumber == 1)
			{				
				TheObjectHandler::Instance()->GetObject2D("GraphicalButton03")->SetVisible(false);
				TheButtons::Instance()->SetOSButtonActivity(false, "InfoButton3",NO_LEGEND);

				TheButtons::Instance()->SetOSButtonActivity(false, "InfoButton2",NO_LEGEND);
				mhelpButtonID = TheTextureManager::Instance()->GetTextureID("LegendForwardLit.png");
				TheObjectHandler::Instance()->GetObject2D("LegendButton2Lit")->SetTextureID(mhelpButtonID);	

				mhelpButtonID = TheTextureManager::Instance()->GetTextureID("LegendForwardNlit.png");
				TheObjectHandler::Instance()->GetObject2D("LegendButton2Nlit")->SetTextureID(mhelpButtonID);
				TheButtons::Instance()->SetOSButtonActivity(true, "InfoButton2",LAMP_ON);

				ThePokerGame::Instance()->UpdateInfoDigits(true);
			}

			if (mHelpScreenNumber > 1 && mHelpScreenNumber < NUM_INFO_SCREEN)
			{
				TheButtons::Instance()->SetOSButtonActivity(false, "InfoButton2",NO_LEGEND);
				mhelpButtonID = TheTextureManager::Instance()->GetTextureID("LegendForwardLit.png");
				TheObjectHandler::Instance()->GetObject2D("LegendButton2Lit")->SetTextureID(mhelpButtonID);	

				mhelpButtonID = TheTextureManager::Instance()->GetTextureID("LegendForwardNlit.png");
				TheObjectHandler::Instance()->GetObject2D("LegendButton2Nlit")->SetTextureID(mhelpButtonID);
				TheButtons::Instance()->SetOSButtonActivity(true, "InfoButton2",LAMP_ON);

				TheButtons::Instance()->SetOSButtonActivity(false, "InfoButton3",NO_LEGEND);
				mhelpButtonID = TheTextureManager::Instance()->GetTextureID("LegendBackwardLit.png");
				TheObjectHandler::Instance()->GetObject2D("LegendButton3Lit")->SetTextureID(mhelpButtonID);	

				mhelpButtonID = TheTextureManager::Instance()->GetTextureID("LegendBackwardNlit.png");
				TheObjectHandler::Instance()->GetObject2D("LegendButton3Nlit")->SetTextureID(mhelpButtonID);
				TheObjectHandler::Instance()->GetObject2D("GraphicalButton03")->SetVisible(true);
				TheButtons::Instance()->SetOSButtonActivity(true, "InfoButton3",LAMP_ON);
			}

			if (mHelpScreenNumber == NUM_INFO_SCREEN)
			{
				TheButtons::Instance()->SetOSButtonActivity(false, "InfoButton2",NO_LEGEND);
				mhelpButtonID = TheTextureManager::Instance()->GetTextureID("LegendBackwardLit.png");
				TheObjectHandler::Instance()->GetObject2D("LegendButton2Lit")->SetTextureID(mhelpButtonID);	

				mhelpButtonID = TheTextureManager::Instance()->GetTextureID("LegendBackwardNlit.png");
				TheObjectHandler::Instance()->GetObject2D("LegendButton2Nlit")->SetTextureID(mhelpButtonID);
				TheButtons::Instance()->SetOSButtonActivity(true, "InfoButton2",LAMP_ON);

				TheObjectHandler::Instance()->GetObject2D("GraphicalButton03")->SetVisible(false);
				TheButtons::Instance()->SetOSButtonActivity(false, "InfoButton3",NO_LEGEND);
			}

			if (mHelpScreenNumber == 2)
				ThePokerGame::Instance()->UpdateGoldenDigits(true);
		}				
	}
}



