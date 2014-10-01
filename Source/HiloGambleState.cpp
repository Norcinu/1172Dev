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
	GET_BUTTONS->SetButtonActivity(false, "FrontStart");
	GET_BUTTONS->SetButtonActivity(false, "Stake");	
	GET_BUTTONS->SetButtonActivity(false, "TopStart");	
	GET_BUTTONS->SetButtonActivity(false, "Collect");

	GET_BUTTONS->SetOSButtonActivity(false, "CollectButton");
	GET_BUTTONS->SetOSButtonActivity(false, "HoldInfoButton");
	GET_BUTTONS->SetOSButtonActivity(false, "Hold2Button");
	GET_BUTTONS->SetOSButtonActivity(false, "Hold3Button");
	GET_BUTTONS->SetOSButtonActivity(false, "Hold4Button");
	GET_BUTTONS->SetOSButtonActivity(false, "HoldTransferButton");
	GET_BUTTONS->SetOSButtonActivity(false, "DealStart1PndButton");
	GET_BUTTONS->SetOSButtonActivity(false, "DealStart2PndButton");

	ThePokerGame::Instance()->HiloGambleStage = 1;
	ThePokerGame::Instance()->HiloDelayTimer = 0;
}

void HiloGambleState::Exit()
{
	ThePokerGame::Instance()->HiloGambleStage = 0;
	ThePokerGame::Instance()->ActivateHiloGambleGraphics = 0;
	GET_BUTTONS->SetOSButtonActivity(false, "LoButton");
	GET_BUTTONS->SetOSButtonActivity(false, "HiButton");
	if (ThePokerGame::Instance()->GetGameIndex() == Game200p)
		GET_BUTTONS->SetOSButtonActivity(false, "Swop2PndButton");
	else
		GET_BUTTONS->SetOSButtonActivity(false, "Swop1PndButton");
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
