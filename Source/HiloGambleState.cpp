#include <ObjectHandler.h>
#include <TextureManager.h>
#include "GenericButtons.h"
#include "Defines.h"
#include "Game.h"
#include "HiloGambleState.h"
#include "PokerHandProcess.h"
#include "PokerGame.h"

static bool Registered = TheEngine::Instance()->AddState("HiloGamble", new HiloGambleState());

extern unsigned char global_quit;

HiloGambleState::HiloGambleState()
{

}

HiloGambleState::~HiloGambleState()
{

}

void HiloGambleState::Enter()
{
	TheButtons::Instance()->SetButtonActivity(false, "Start");
	TheButtons::Instance()->SetButtonActivity(false, "OnePound");	
	TheButtons::Instance()->SetButtonActivity(false, "TwoPound");	
	TheButtons::Instance()->SetButtonActivity(false, "Collect");

	TheButtons::Instance()->SetOSButtonActivity(false, "CollectButton");
	TheButtons::Instance()->SetOSButtonActivity(false, "HoldInfoButton");
	TheButtons::Instance()->SetOSButtonActivity(false, "Hold2Button");
	TheButtons::Instance()->SetOSButtonActivity(false, "Hold3Button");
	TheButtons::Instance()->SetOSButtonActivity(false, "Hold4Button");
	TheButtons::Instance()->SetOSButtonActivity(false, "HoldTransferButton");
	TheButtons::Instance()->SetOSButtonActivity(false, "DealStart1PndButton");
	TheButtons::Instance()->SetOSButtonActivity(false, "DealStart2PndButton");

	ThePokerGame::Instance()->HiloGambleStage = 1;
	ThePokerGame::Instance()->HiloDelayTimer = 0;
}

void HiloGambleState::Exit()
{
	ThePokerGame::Instance()->HiloGambleStage = 0;
	ThePokerGame::Instance()->ActivateHiloGambleGraphics = 0;
	TheButtons::Instance()->SetOSButtonActivity(false, "LoButton");
	TheButtons::Instance()->SetOSButtonActivity(false, "HiButton");
	if (ThePokerGame::Instance()->GetGameIndex() == Game200p)
		TheButtons::Instance()->SetOSButtonActivity(false, "Swop2PndButton");
	else
		TheButtons::Instance()->SetOSButtonActivity(false, "Swop1PndButton");
}

#include <sstream>

void HiloGambleState::Update()
{		
	PROFILE(__FUNCTION__);	

	if(TheEngine::Instance()->GetProcessManager()->GetNumQueueProcesses())
	{
		return;
	}

	if(GetCurrentError())
	{
		return;
	}

	if(!global_quit)
	{
		ThePokerGame::Instance()->HiloGamble(ThePokerGame::Instance()->JokerWin);
		
		if (ThePokerGame::Instance()->HiloGambleStage > 9)			
			TheEngine::Instance()->StateTransition("Payment");
	}
}
