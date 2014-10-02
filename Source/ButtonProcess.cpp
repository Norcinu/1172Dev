#include "GenericButtons.h"
#include "ButtonProcess.h"
#include "Game.h"
#include "bo.h"

ButtonProcess::ButtonProcess(HardwareButton* button)
{
	mButton = button;
	TheAudioManager::Instance()->GetAudioSample("DROP")->Play();
}

ButtonProcess::~ButtonProcess()
{

}

void ButtonProcess::Complete()
{
	mButton->Clear();
	THE_BUTTONS->SetButtonProcessEnded();
}

void ButtonProcess::Update()
{
	if (!mButton->CheckPressed())
	{
		mButton->SetPressed();
	
		if(mButton->IsReleased())
		{
			SetCompleted();
		}
	}	
}