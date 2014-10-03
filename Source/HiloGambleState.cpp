#include <ObjectHandler.h>
#include <TextureManager.h>
#include "GenericButtons.h"
#include "Defines.h"
#include "Game.h"
#include "HiloGambleState.h"
#include "PokerHandProcess.h"
#include "PokerGame.h"

static bool Registered = ENGINE->AddState("HiloGamble", new HiloGambleState());

extern unsigned char global_quit;

HiloGambleState::HiloGambleState()
{

}

HiloGambleState::~HiloGambleState()
{

}

void HiloGambleState::Enter()
{
	THE_BUTTONS->SetButtonActivity(false, "FrontStart");
	THE_BUTTONS->SetButtonActivity(false, "Stake");	
	THE_BUTTONS->SetButtonActivity(false, "TopStart");	
	THE_BUTTONS->SetButtonActivity(false, "Collect");

	THE_BUTTONS->SetOSButtonActivity(false, "CollectButton");
	THE_BUTTONS->SetOSButtonActivity(false, "HoldInfoButton");
	THE_BUTTONS->SetOSButtonActivity(false, "Hold2Button");
	THE_BUTTONS->SetOSButtonActivity(false, "Hold3Button");
	THE_BUTTONS->SetOSButtonActivity(false, "Hold4Button");
	THE_BUTTONS->SetOSButtonActivity(false, "HoldTransferButton");
	THE_BUTTONS->SetOSButtonActivity(false, "DealStart1PndButton");
	THE_BUTTONS->SetOSButtonActivity(false, "DealStart2PndButton");

	ThePokerGame::Instance()->HiloGambleStage = 1;
	ThePokerGame::Instance()->HiloDelayTimer = 0;
	
	//OBJECT_HANDLER->GetObject2D("GraphicalButton07")->SetVisible(true);
	//OBJECT_HANDLER->GetObject2D("GraphicalButton10")->SetVisible(true);
	OBJECT_HANDLER->GetObject2D("GraphicalButton08")->SetVisible(false);
	OBJECT_HANDLER->GetObject2D("Legend1DealDrawLit")->SetVisible(false);
	OBJECT_HANDLER->GetObject2D("Legend1DealDrawNlit")->SetVisible(false);
	OBJECT_HANDLER->GetObject2D("Legend2DealDrawLit")->SetVisible(false);
	OBJECT_HANDLER->GetObject2D("Legend2DealDrawNlit")->SetVisible(false);
	
	OBJECT_HANDLER->GetObject2D("LegendHoldInfoNlit")->SetVisible(false);
	OBJECT_HANDLER->GetObject2D("LegendHold3Nlit")->SetVisible(false);
	
	OBJECT_HANDLER->GetObject2D("LegendHoldTransferNlit")->SetVisible(false);
	
	OBJECT_HANDLER->GetObject2D("GraphicalButton02")->SetVisible(false);
	OBJECT_HANDLER->GetObject2D("GraphicalButton04")->SetVisible(false);
	OBJECT_HANDLER->GetObject2D("GraphicalButton06")->SetVisible(false);
}

void HiloGambleState::Exit()
{
	ThePokerGame::Instance()->HiloGambleStage = 0;
	ThePokerGame::Instance()->ActivateHiloGambleGraphics = 0;
	THE_BUTTONS->SetOSButtonActivity(false, "LoButton");
	THE_BUTTONS->SetOSButtonActivity(false, "HiButton");

	OBJECT_HANDLER->GetObject2D("LegendHoldInfoNlit")->SetVisible(true);
	OBJECT_HANDLER->GetObject2D("LegendHold3Nlit")->SetVisible(true);

	OBJECT_HANDLER->GetObject2D("LegendHoldTransferNlit")->SetVisible(true);

	OBJECT_HANDLER->GetObject2D("GraphicalButton02")->SetVisible(true);
	OBJECT_HANDLER->GetObject2D("GraphicalButton04")->SetVisible(true);
	OBJECT_HANDLER->GetObject2D("GraphicalButton06")->SetVisible(true);
	
//	OBJECT_HANDLER->GetObject2D("LegendSwopNlit")->SetVisible(false);
	//OBJECT_HANDLER->GetObject2D("GraphicalButton10")->SetVisible(false);
	/*if (ThePokerGame::Instance()->GetGameIndex() == Game200p)
		THE_BUTTONS->SetOSButtonActivity(false, "Swop2PndButton");
	else*/
		//THE_BUTTONS->SetOSButtonActivity(false, "SwopButton");
}

#include <sstream>

void HiloGambleState::Update()
{		
	PROFILE(__FUNCTION__);	

	if(ENGINE->GetProcessManager()->GetNumQueueProcesses())
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
			ENGINE->StateTransition("Payment");
	}
}
