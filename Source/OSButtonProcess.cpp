#include <Input.h>
#include <ObjectHandler.h>
#include <TextureManager.h>
#include "GenericButtons.h"
#include "Game.h"
#include "OSButtonProcess.h"

OSButtonProcess::OSButtonProcess(OSButton* button)
{
	mButton = button;
	mButtonObject = OBJECT_HANDLER->GetObject2D(mButton->GetName());	

	if (ENGINE->GetCurrentState()->GetName() == "HoldStart" &&
		(mButton->GetLegendName() == "LegendHoldInfoLit" ||
		 mButton->GetLegendName() == "LegendHold2Lit" ||		
		 mButton->GetLegendName() == "LegendHold3Lit" ||		
		 mButton->GetLegendName() == "LegendHold4Lit" ||		
		 mButton->GetLegendName() == "LegendHoldTransferLit" ||
		 mButton->GetLegendName() == "LegendCollectLit"))

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
	THE_BUTTONS->SetButtonProcessEnded();
}

void OSButtonProcess::Update()
{		
	mButton->SetPressed();		

	if(mButton->IsReleased() || !mButton->IsActive())
	{		
		SetCompleted();
	}	
}
