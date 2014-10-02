#include "GenericButtons.h"
#include "ButtonProcess.h"
#include "Game.h"
#include "OSButtonProcess.h"
#include "Defines.h"
#include "PokerGame.h"
#include <algorithm>

extern unsigned char FlashTog;

using std::for_each;
using std::begin;
using std::end;
using std::pair;
using std::string;

HardwareButton::HardwareButton(const char* BtnName, unsigned char offset, unsigned char mask, unsigned char lamp)
{
	mName = BtnName;
	mOffset = offset;
	mMask = mask;
	mLamp = lamp;
	mActive = false;
	mButtonReleased = false;
	mButtonPressed = false;
}

void HardwareButton::Clear()
{
	mButtonReleased = false;
	mButtonPressed = false;
}

void HardwareButton::Release()
{
	mButtonPressed = false;
	mButtonReleased = true;
}

bool HardwareButton::CheckPressed()
{
	return GetSwitchStatus(mOffset, mMask)?true:false;
}

void HardwareButton::SetActive(bool active, unsigned char state)
{
	mActive = active;

	if(state == LAMP_ON)
	{
		SetLampOn(mLamp);
	}
	else if(state == LAMP_OFF)
	{
		SetLampOff(mLamp);
	}
	else if(state == LAMP_FLASH)
	{
		SetLampFlash(mLamp);
	}
	else if(state == LAMP_ANTI)
	{
		SetLampAnti(mLamp);
	}

	if(!mActive)
	{

	}
}

OSButton::OSButton(const char* objName,const char* objLlegend,const char* objUlegend)
{
	mName = objName;
	mLlegend = objLlegend;
	mUlegend = objUlegend;	
	mActive = false;
	mButtonReleased = false;
	mButtonPressed = false;
}

bool OSButton::CheckPressed()
{
	if(TheInput::Instance()->Pick2D(mName))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void OSButton::SetActive(bool active, unsigned char state)
{
	mActive = active;	
	
	if(state == LAMP_ON)
	{
		OBJECT_HANDLER->GetObject2D(mUlegend)->SetVisible(false);
		OBJECT_HANDLER->GetObject2D(mLlegend)->SetVisible(true);		
	}
	else if(state == LAMP_OFF)
	{
		OBJECT_HANDLER->GetObject2D(mLlegend)->SetVisible(false);
		OBJECT_HANDLER->GetObject2D(mUlegend)->SetVisible(true);
	}
	else if(state == LAMP_FLASH)
	{
		if (!FlashTog)
		{
			OBJECT_HANDLER->GetObject2D(mUlegend)->SetVisible(false);
			OBJECT_HANDLER->GetObject2D(mLlegend)->SetVisible(true);
		}
		else
		{
			OBJECT_HANDLER->GetObject2D(mLlegend)->SetVisible(false);
			OBJECT_HANDLER->GetObject2D(mUlegend)->SetVisible(true);
		}
	}
	else if(state == LAMP_ANTI)
	{
		if (FlashTog)
		{
			OBJECT_HANDLER->GetObject2D(mUlegend)->SetVisible(false);
			OBJECT_HANDLER->GetObject2D(mLlegend)->SetVisible(true);	
		}
		else
		{
			OBJECT_HANDLER->GetObject2D(mLlegend)->SetVisible(false);
			OBJECT_HANDLER->GetObject2D(mUlegend)->SetVisible(true);
		}
	}
	else
	{
		OBJECT_HANDLER->GetObject2D(mLlegend)->SetVisible(false);
		OBJECT_HANDLER->GetObject2D(mUlegend)->SetVisible(false);
	}
}

void OSButton::Clear()
{
	mButtonReleased = false;
	mButtonPressed = false;
}

void OSButton::Release()
{
	mButtonPressed = false;
	mButtonReleased = true;
}

Buttons::Buttons(void)
{
	mStandbyTimer = 0;
	mStandbyType = 1;
	mButtonProcessStarted = false;

	
}

void Buttons::Initialise()
{
	mButtons["FrontStart"] = new HardwareButton("FrontStart",LH7_BUTTON, LH7_LAMP);
	mButtons["Stake"] = new HardwareButton("Stake",LH5_BUTTON, LH5_LAMP);
	mButtons["TopStart"] = new HardwareButton("TopStart",LH6_BUTTON, LH6_LAMP);
	mButtons["Transfer"] = new HardwareButton("Transfer",LH4_BUTTON, LH4_LAMP);
	mButtons["AutoPlay"] = new HardwareButton("AutoPlay",LH3_BUTTON, LH3_LAMP);
	mButtons["Menu"] = new HardwareButton("Menu",LH2_BUTTON, LH2_LAMP);
	mButtons["Collect"] = new HardwareButton("Collect",LH1_BUTTON, LH1_LAMP);
		
	mOSButtons["AutoplayButton"]	  = new OSButton("GraphicalButton09","LegendAutoPlayLit","LegendAutoPlayNlit");
	mOSButtons["DealStart2PndButton"] = new OSButton("GraphicalButton08","Legend2DealDrawLit","Legend2DealDrawNlit");
	mOSButtons["DealStart1PndButton"] = new OSButton("GraphicalButton07","Legend1DealDrawLit","Legend1DealDrawNlit");
	mOSButtons["HoldTransferButton"]  = new OSButton("GraphicalButton06","LegendHoldTransferLit","LegendHoldTransferNlit");
	mOSButtons["Hold4Button"]		  = new OSButton("GraphicalButton05","LegendHold4Lit","LegendHold4Nlit");
	mOSButtons["Hold3Button"]		  = new OSButton("GraphicalButton04","LegendHold3Lit","LegendHold3Nlit");
	mOSButtons["Hold2Button"]		  = new OSButton("GraphicalButton03","LegendHold2Lit","LegendHold2Nlit");
	mOSButtons["HoldInfoButton"]	  = new OSButton("GraphicalButton02","LegendHoldInfoLit","LegendHoldInfoNlit");
	mOSButtons["CollectButton"]		  = new OSButton("GraphicalButton01","LegendCollectLit","LegendCollectNlit");

	//mOSButtons["Swop2PndButton"]	  = new OSButton("GraphicalButton08","Legend2SwopLit","Legend2SwopNlit");
	mOSButtons["Swop1PndButton"]	  = new OSButton("GraphicalButton07","Legend1SwopLit","Legend1SwopNlit");
	mOSButtons["LoButton"]			  = new OSButton("GraphicalButton05","LegendLoLit","LegendLoNlit");
	mOSButtons["HiButton"]			  = new OSButton("GraphicalButton03","LegendHiLit","LegendHiNlit");

	
	mOSButtons["InfoButton1"]		  = new OSButton("GraphicalButton01","LegendButton1Lit","LegendButton1Nlit");
	mOSButtons["InfoButton2"]		  = new OSButton("GraphicalButton02","LegendButton2Lit","LegendButton2Nlit");
	mOSButtons["InfoButton3"]		  = new OSButton("GraphicalButton03","LegendButton3Lit","LegendButton3Nlit");
	
}

Buttons::~Buttons(void)
{
}

void Buttons::UpdateButtons()
{
	PROFILE(__FUNCTION__);
#ifndef FAST_PLAY

	if(TheGame::Instance()->GetAutoplay())
	{
		if(ButtonPressed("AutoPlay") || OSButtonPressed("AutoplayButton"))
		{
			TheGame::Instance()->SetAutoplay(false);
			SetButtonActivity(false, "AutoPlay", LAMP_OFF);
			SetOSButtonActivity(false, "AutoplayButton",LAMP_OFF);		
		}
	}

	if(mButtonProcessStarted)
	{
		return;
	}
	
	if(TheInput::Instance()->KeyHit(Mouse_LButton))
	{
		for(OnScreenButtons::iterator it = mOSButtons.begin(); it != mOSButtons.end();it++)
		{
			if(it->second->IsActive())
			{
				if(it->second->CheckPressed())
				{					
					TheEngine::Instance()->GetProcessManager()->AddProcessToList(new OSButtonProcess(it->second));
					mButtonProcessStarted = true;
					return;
				}
			}
		}		

		if (!mButtonProcessStarted)
		{
			ThePokerGame::Instance()->RegisterCardButtons();
			ThePokerGame::Instance()->RegisterYesDemoButton();
			ThePokerGame::Instance()->RegisterCollectBoxButtons();
		}
	}

	for(HardwareButtons::iterator it = mButtons.begin(); it != mButtons.end();it++)
	{
		if(it->second->IsActive())
		{
			if(it->second->CheckPressed())
			{
				for(HardwareButtons::iterator itr = mButtons.begin(); itr != mButtons.end();itr++)
					itr->second->SetActive(itr->second->IsActive(), LAMP_OFF);
				it->second->SetActive(it->second->IsActive(), LAMP_ON);

				TheEngine::Instance()->GetProcessManager()->AddProcessToList(new ButtonProcess(it->second));
				mButtonProcessStarted = true;
				return;
			}
		}
	}
#endif
}

bool Buttons::ButtonPressed(const char* buttonName)
{
	PROFILE(__FUNCTION__);
	HardwareButtons::iterator it = mButtons.find(buttonName);
	if(it != mButtons.end())
	{
		if(it->second->IsPressed())
		{
			it->second->Release();
			return true;
		}
	}
	return false;
}

bool Buttons::OSButtonPressed(const char* buttonName)
{
	PROFILE(__FUNCTION__);
	OnScreenButtons::iterator it = mOSButtons.find(buttonName);
	if(it != mOSButtons.end())
	{
		if(it->second->IsPressed())
		{
			it->second->Release();
			return true;
		}
	}
	return false;
}

void Buttons::DisableHWButtons()
{
	HardwareButtons::iterator it = mButtons.begin();

	while(it != mButtons.end())
	{
		it->second->SetActive(false);
		++it;
	}
}

void Buttons::SetButtonActivity(bool active, const char* buttonName, unsigned char state)
{
	for(HardwareButtons::iterator it = mButtons.begin(); it != mButtons.end();it++)
	{
		if (it->second->CheckPressed())
			return;
	}

	HardwareButtons::iterator it = mButtons.find(buttonName);
	if(it != mButtons.end())	
	{
		it->second->SetActive(active, state);
	}
}

void Buttons::SetOSButtonActivity(bool active, const char* buttonName, unsigned char state)
{
	for(HardwareButtons::iterator it = mButtons.begin(); it != mButtons.end();it++)
	{
		if (it->second->CheckPressed() && it->second->IsActive())
		{
			if (it->second->GetName() == "Stake" && buttonName == "DealStart1PndButton")
				state = LAMP_ON;
			else if (it->second->GetName() == "TopStart" && buttonName == "DealStart2PndButton")
				state = LAMP_ON;
			else if (it->second->GetName() == "AutoPlay" && buttonName == "AutoplayButton")
				state = LAMP_ON;
			else if (it->second->GetName() == "Collect" && buttonName == "CollectButton")
				state = LAMP_ON;
			else
				state = LAMP_OFF;
			break;
		}
	}

	OnScreenButtons::iterator it = mOSButtons.find(buttonName);
	if(it != mOSButtons.end())
	{
		it->second->SetActive(active, state);
	}
}

void Buttons::LampsOff()
{
	SetLampState(LH1_LAMP, LAMP_OFF);
	SetLampState(LH2_LAMP, LAMP_OFF);
	SetLampState(LH3_LAMP, LAMP_OFF);
	SetLampState(LH4_LAMP, LAMP_OFF);
	SetLampState(LH5_LAMP, LAMP_OFF);
	SetLampState(LH6_LAMP, LAMP_OFF);
	SetLampState(LH7_LAMP, LAMP_OFF);
	SetLampState(LH8_LAMP, LAMP_OFF);
}


void Buttons::SetLampState(unsigned char Lamp, unsigned char State)
{
	switch(State)
	{
	case LAMP_OFF:
		SetLampOff(Lamp);
		break;
	case LAMP_ON:
		SetLampOn(Lamp);
		break;
	case LAMP_FLASH:
		SetLampFlash(Lamp);
		break;
	case LAMP_ANTI:
		SetLampAnti(Lamp);
		break;
	};
}

void Buttons::StandbyButtons()
{
	if(GetTerminalFormat() > 1)
		THE_BUTTONS->SetButtonActivity(true, "Menu", LAMP_ON);
	else
		THE_BUTTONS->SetButtonActivity(false, "Menu", LAMP_OFF);
	THE_BUTTONS->SetOSButtonActivity(true, "HoldInfoButton", LAMP_ON);


	if(((GetCredits()+GetBankDeposit() >= GetMinPayoutValue()) ||
		(GetCredits()+GetBankDeposit()>0 && GetTerminalType() !=HOPPER))
		&& !GetDoorStatus())
	{
		THE_BUTTONS->SetButtonActivity(true, "Collect", LAMP_ON);
		THE_BUTTONS->SetOSButtonActivity(true, "CollectButton", LAMP_ON);
	}
	else
	{
		THE_BUTTONS->SetButtonActivity(false, "Collect");
		THE_BUTTONS->SetOSButtonActivity(false, "CollectButton");
	}
	
	unsigned int bdelay = 16; //needs this delay in standby, otherwise too slow??

	if(mStandbyTimer == 0)
		mStandbyType = GetLocalIntRandomNumber(5) + 1;

	if(mStandbyType == 1 || mStandbyType == 2)
	{
		if(mStandbyType == 1)
			mStandbyTimer ++;
		if(mStandbyType == 2)
			if(mStandbyTimer)mStandbyTimer--;

		SetLampState(LH1_LAMP, mStandbyTimer<=(bdelay*1)							  ? LAMP_ON : LAMP_OFF);
		SetLampState(LH2_LAMP, mStandbyTimer> (bdelay*1) && mStandbyTimer<=(bdelay*2) ? LAMP_ON : LAMP_OFF);
		SetLampState(LH3_LAMP, mStandbyTimer> (bdelay*2) && mStandbyTimer<=(bdelay*3) ? LAMP_ON : LAMP_OFF);
		SetLampState(LH4_LAMP, mStandbyTimer> (bdelay*3) && mStandbyTimer<=(bdelay*4) ? LAMP_ON : LAMP_OFF);
		SetLampState(LH5_LAMP, mStandbyTimer> (bdelay*4) && mStandbyTimer<=(bdelay*5) ? LAMP_ON : LAMP_OFF);
		SetLampState(LH6_LAMP, mStandbyTimer> (bdelay*5) && mStandbyTimer<=(bdelay*6) ? LAMP_ON : LAMP_OFF);
		SetLampState(LH7_LAMP, mStandbyTimer> (bdelay*6) && mStandbyTimer<=(bdelay*7) ? LAMP_ON : LAMP_OFF);
		SetLampState(LH8_LAMP, mStandbyTimer> (bdelay*7) && mStandbyTimer<=(bdelay*8) ? LAMP_ON : LAMP_OFF);
		
		if(mStandbyTimer>(bdelay*8))
			mStandbyTimer = 0;
	}
	else if(mStandbyType==3)
	{
		mStandbyTimer ++;
		if(mStandbyTimer<=(bdelay*1))
		{
			LampsOff();
		}
		if(mStandbyTimer>=(bdelay*2))
			mStandbyTimer = 0;
	}
	else if(mStandbyType==4)
	{
		mStandbyTimer ++;
		
		SetLampState(LH1_LAMP, mStandbyTimer<=(bdelay*1)							 ? LAMP_ON : LAMP_OFF);
		SetLampState(LH8_LAMP, mStandbyTimer<=(bdelay*1)							 ? LAMP_ON : LAMP_OFF);

		SetLampState(LH2_LAMP, mStandbyTimer> (bdelay*1) && mStandbyTimer<=(bdelay*2) ? LAMP_ON : LAMP_OFF);
		SetLampState(LH7_LAMP, mStandbyTimer> (bdelay*1) && mStandbyTimer<=(bdelay*2) ? LAMP_ON : LAMP_OFF);

		SetLampState(LH3_LAMP, mStandbyTimer> (bdelay*2) && mStandbyTimer<=(bdelay*3) ? LAMP_ON : LAMP_OFF);		
		SetLampState(LH6_LAMP, mStandbyTimer> (bdelay*2) && mStandbyTimer<=(bdelay*3) ? LAMP_ON : LAMP_OFF);		

		SetLampState(LH4_LAMP, mStandbyTimer> (bdelay*3) && mStandbyTimer<=(bdelay*4) ? LAMP_ON : LAMP_OFF);		
		SetLampState(LH5_LAMP, mStandbyTimer> (bdelay*3) && mStandbyTimer<=(bdelay*4) ? LAMP_ON : LAMP_OFF);		

		if(mStandbyTimer>=(bdelay*4))
			mStandbyTimer = 0;
	}
	else if(mStandbyType==5)
	{
		mStandbyTimer ++;

		if(mStandbyTimer<=(bdelay*1))
			SetLampState(LH1_LAMP, LAMP_ON);
		if(mStandbyTimer<=(bdelay*1))
			SetLampState(LH8_LAMP, LAMP_ON);

		if(mStandbyTimer> (bdelay*1) && mStandbyTimer<=(bdelay*2))
			SetLampState(LH2_LAMP, LAMP_ON);
		if(mStandbyTimer> (bdelay*1) && mStandbyTimer<=(bdelay*2))
			SetLampState(LH7_LAMP, LAMP_ON);

		if(mStandbyTimer> (bdelay*2) && mStandbyTimer<=(bdelay*3))
			SetLampState(LH3_LAMP, LAMP_ON);
		if(mStandbyTimer> (bdelay*2) && mStandbyTimer<=(bdelay*3))
			SetLampState(LH6_LAMP, LAMP_ON);

		if(mStandbyTimer> (bdelay*3) && mStandbyTimer<=(bdelay*4))
			SetLampState(LH4_LAMP, LAMP_ON);
		if(mStandbyTimer> (bdelay*3) && mStandbyTimer<=(bdelay*4))
			SetLampState(LH5_LAMP, LAMP_ON);

		if(mStandbyTimer> (bdelay*4) && mStandbyTimer<=(bdelay*5))
			SetLampState(LH4_LAMP, LAMP_OFF);
		if(mStandbyTimer> (bdelay*4) && mStandbyTimer<=(bdelay*5))
			SetLampState(LH5_LAMP, LAMP_OFF);

		if(mStandbyTimer> (bdelay*5) && mStandbyTimer<=(bdelay*6))
			SetLampState(LH3_LAMP, LAMP_OFF);
		if(mStandbyTimer> (bdelay*5) && mStandbyTimer<=(bdelay*6))
			SetLampState(LH5_LAMP, LAMP_OFF);

		if(mStandbyTimer> (bdelay*6) && mStandbyTimer<=(bdelay*7))
			SetLampState(LH2_LAMP, LAMP_OFF);
		if(mStandbyTimer> (bdelay*6) && mStandbyTimer<=(bdelay*7))
			SetLampState(LH7_LAMP, LAMP_OFF);

		if(mStandbyTimer> (bdelay*7) && mStandbyTimer<=(bdelay*8))
			SetLampState(LH1_LAMP, LAMP_OFF);
		if(mStandbyTimer> (bdelay*7) && mStandbyTimer<=(bdelay*8))
			SetLampState(LH8_LAMP, LAMP_OFF);

		if(mStandbyTimer>=(bdelay*8))
			mStandbyTimer = 0;
	}
}

void Buttons::DealStartButtons(float DelayTimer)
{
	PROFILE(__FUNCTION__);
#ifndef FAST_PLAY
	
	if(GetCredits() >= MAXIMUM_BET)
	{
		SetButtonActivity(true, "FrontStart", LAMP_FLASH);
		SetButtonActivity(true, "Stake",LAMP_ON);
		SetButtonActivity(true, "TopStart", LAMP_ON);
		SetOSButtonActivity(true, "DealStart1PndButton",LAMP_ON);
		SetOSButtonActivity(true, "DealStart2PndButton",LAMP_ON);
	}
	else if(GetCredits() >= MINIMUM_BET)
	{
		SetButtonActivity(true, "FrontStart", LAMP_FLASH);
		SetButtonActivity(true, "Stake",LAMP_ON);
		SetOSButtonActivity(true, "DealStart1PndButton",LAMP_ON);
	}
	else
	{
		SetButtonActivity(false, "FrontStart");
		SetButtonActivity(false, "Stake");
		SetButtonActivity(false, "TopStart");
		SetOSButtonActivity(false, "DealStart1PndButton");
		SetOSButtonActivity(false, "DealStart2PndButton");
	}
	
	if(GetTerminalFormat() > 1)
		SetButtonActivity(true, "Menu", LAMP_ON);
	else
		SetButtonActivity(false, "Menu", LAMP_OFF);
	if(GetBankDeposit())
	{
		unsigned char lamp = LAMP_ON;
		if(GetCredits() < TheGame::Instance()->GetStake())
			lamp = LAMP_FLASH;
		SetButtonActivity(true, "Transfer", lamp);
		SetOSButtonActivity(true, "HoldTransferButton",LAMP_ON);		
	}
	else
	{
		SetButtonActivity(false, "Transfer");
		SetOSButtonActivity(false, "HoldTransferButton");
	}
		
	if(GetCredits() >= TheGame::Instance()->GetStake())
	{
		if (TheGame::Instance()->GetAutoplay())
		{
			SetButtonActivity(true, "AutoPlay", LAMP_ON);
			SetOSButtonActivity(true, "AutoplayButton",LAMP_ON);
		}
		else
		{
			SetButtonActivity(true, "AutoPlay", LAMP_FLASH);
			SetOSButtonActivity(true, "AutoplayButton",LAMP_FLASH);
		}
	}
	else
	{
		SetButtonActivity(false, "AutoPlay");
		SetOSButtonActivity(false, "AutoplayButton");
	}

	
	if(((GetCredits()+GetBankDeposit() >= GetMinPayoutValue()) ||
		(GetCredits()+GetBankDeposit()>0 && GetTerminalType() != HOPPER))
	&& !GetDoorStatus())
	{
		SetButtonActivity(true, "Collect", LAMP_ON);
		SetOSButtonActivity(true, "CollectButton",LAMP_ON);
	}
	else
	{
		SetButtonActivity(false, "Collect");
		SetOSButtonActivity(false, "CollectButton");
	}

	if(DelayTimer < TheEngine::Instance()->GetSystemTimer().GetRunningTime())
	{
		SetOSButtonActivity(true, "HoldInfoButton",LAMP_ON);
	}
	
#endif
}


void Buttons::HoldStartButtons()
{
	PROFILE(__FUNCTION__);
#ifndef FAST_PLAY
	
	SetButtonActivity(true, "FrontStart", LAMP_FLASH);
	SetButtonActivity(true, "TopStart", LAMP_ON);
	SetButtonActivity(false, "Stake");				
	
	if (TheGame::Instance()->GetStake() == MAXIMUM_BET)
	{
		SetOSButtonActivity(true, "DealStart2PndButton",LAMP_ON);
		SetOSButtonActivity(false, "DealStart1PndButton");
	}
	else
	{
		SetOSButtonActivity(true, "DealStart1PndButton", LAMP_ON);
		SetOSButtonActivity(false, "DealStart2PndButton");
	}

	
	if(GetCredits() >= TheGame::Instance()->GetStake())
	{
		if (TheGame::Instance()->GetAutoplay())
		{
			SetButtonActivity(true, "AutoPlay", LAMP_ON);
			SetOSButtonActivity(true, "AutoplayButton",LAMP_ON);
		}
		else
		{
			SetButtonActivity(true, "AutoPlay", LAMP_FLASH);
			SetOSButtonActivity(true, "AutoplayButton",LAMP_FLASH);
		}
	}
	else
	{
		SetButtonActivity(false, "AutoPlay");
		SetOSButtonActivity(false, "AutoplayButton");
	}
					
#endif
}

void Buttons::DisableOSButtons()
{
	for_each(begin(mOSButtons), end(mOSButtons), [] (pair<string,OSButton*> b)
	{
		b.second->SetActive(false, LAMP_OFF);
	});
}
