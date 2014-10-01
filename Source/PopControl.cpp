#include <Engine.h>
#include <ObjectHandler.h>
#include "GenericButtons.h"
#include "PopControl.h"
#include "ShowStakeOptionsProcess.h"

PopControl::PopControl()
{
	
	mPopOptionsVisible = false;
}

PopControl::~PopControl()
{
	mPopPositions.clear();
}

void PopControl::Initialise(const std::vector<unsigned int>& stakes, unsigned int defaultStakeID)
{
	Object2D* buttons = TheObjectHandler::Instance()->GetObject2D("PopButtons");	
	//get the position of the first stake option
	D3DXVECTOR2 visiblePosition = buttons->GetInstance(0)->GetPosition();
	
	//hide all instances
	for(unsigned int i  = 0; i < buttons->GetNumberOfInstances();++i)
	{
		buttons->GetInstance(i)->SetVisible(false);
	}
	int popCount = 0;
	for(size_t i = 0; i < stakes.size();++i)
	{			
		D3DXVECTOR2 pos = buttons->GetInstance(popCount)->GetPosition();
		mPopPositions.push_back(pos);
		++popCount;		
	}

	buttons->GetInstance(defaultStakeID)->SetVisible(true);
	buttons->GetInstance(defaultStakeID)->SetPosition(mPopPositions[0]);
}

void PopControl::ClosePopOptions(unsigned int stakeID)
{	
	ChangeType type;
	if(mPopOptionsVisible)
	{
		type = HIDE_OPTIONS;
		mPopOptionsVisible = false;
	}
	else
	{
		type = UPDATE_OPTIONS;
		mPopOptionsVisible = false;
	}
	TheEngine::Instance()->GetProcessManager()->AddProcessToList(new ShowStakeOptionsProcess(mPopPositions, type, stakeID));
}

void PopControl::ChangeStake(unsigned int stakeID)
{
	ChangeType type = SHOW_OPTIONS;	
	if(mPopOptionsVisible)
	{
		type = HIDE_OPTIONS;
		mPopOptionsVisible = false;
	}
	else
	{		
		type = UPDATE_OPTIONS;
		mPopOptionsVisible = false;						
	}

	//GET_BUTTONS->DisableButtons();
	GET_BUTTONS->DisableOSButtons();

	TheEngine::Instance()->GetProcessManager()->AddProcessToList(new ShowStakeOptionsProcess(mPopPositions, type, stakeID));
}

void PopControl::ShowHide(unsigned int stakeID)
{
	ChangeType type = SHOW_OPTIONS;	
	if(mPopOptionsVisible)
	{
		type = HIDE_OPTIONS;
	}
	
	//GET_BUTTONS->DisableButtons();
	GET_BUTTONS->DisableOSButtons();
	
	mPopOptionsVisible = !mPopOptionsVisible;

	TheEngine::Instance()->GetProcessManager()->AddProcessToQueue(new ShowStakeOptionsProcess(mPopPositions, type, stakeID));
}

bool PopControl::AreOptionsVisible() const
{
	return mPopOptionsVisible;
}