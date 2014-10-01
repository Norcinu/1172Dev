#include <Input.h>
#include <ObjectHandler.h>
#include <TextureManager.h>
#include "GenericButtons.h"
#include "Game.h"
#include "OSButtonProcess.h"

OSButtonProcess::OSButtonProcess(OSButton* button)
{
	mButton = button;
	mButtonObject = TheObjectHandler::Instance()->GetObject2D(mButton->GetName());	

	if (TheEngine::Instance()->GetCurrentState()->GetName() == "HoldStart" &&
		(mButton->GetLegendName() == "LegendHoldInfoLit" ||
		 mButton->GetLegendName() == "LegendHold2Lit" ||		
		 mButton->GetLegendName() == "LegendHold3Lit" ||		
		 mButton->GetLegendName() == "LegendHold4Lit" ||		
		 mButton->GetLegendName() == "LegendHoldTransferLit"))

		TheAudioManager::Instance()->GetAudioSample("CLICK")->Play();
	else
		TheAudioManager::Instance()->GetAudioSample("DROP")->Play();
}

OSButtonProcess::~OSButtonProcess()
{

}

void OSButtonProcess::Complete()
{	
	mButton->Clear();
	TheButtons::Instance()->SetButtonProcessEnded();
}

void OSButtonProcess::Update()
{		
	mButton->SetPressed();		

	if(mButton->IsReleased())
	{		
		SetCompleted();
	}	
}