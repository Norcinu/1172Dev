#include <Engine.h>
#include <ObjectHandler.h>
#include "GenericButtons.h"
#include "PopControl.h"
#include "ShowStakeOptionsProcess.h"

PopControl::PopControl()
{
	m_popOptionsVisible = false;
}

PopControl::~PopControl()
{
	m_popPositions.clear();
}

void PopControl::Initialise(const std::vector<unsigned int>& stakes, unsigned int defaultStakeID)
{
	Object2D* buttons = OBJECT_HANDLER->GetObject2D("PopButtons");	
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
		m_popPositions.push_back(pos);
		++popCount;		
	}
}

void PopControl::ClosePopOptions(unsigned int stakeID)
{	
	ChangeType type;
	if(m_popOptionsVisible)
	{
		type = HIDE_OPTIONS;
		m_popOptionsVisible = false;
	}
	else
	{
		type = UPDATE_OPTIONS;
		m_popOptionsVisible = false;
	}
	ENGINE->GetProcessManager()->AddProcessToList(new ShowStakeOptionsProcess(m_popPositions, type, stakeID));
}

void PopControl::ChangeStake(unsigned int stakeID)
{
	ChangeType type = SHOW_OPTIONS;	
	if (m_popOptionsVisible)
	{
		type = HIDE_OPTIONS;
		m_popOptionsVisible = false;
	}
	else
	{		
		type = UPDATE_OPTIONS;
		m_popOptionsVisible = false;						
	}

	ENGINE->GetProcessManager()->AddProcessToList(new ShowStakeOptionsProcess(m_popPositions, type, stakeID));
}

void PopControl::ShowHide(unsigned int stakeID)
{
	ChangeType type = SHOW_OPTIONS;	
	if(m_popOptionsVisible)
	{
		type = HIDE_OPTIONS;
	}
	
	m_popOptionsVisible = !m_popOptionsVisible;

	ENGINE->GetProcessManager()->AddProcessToQueue(new ShowStakeOptionsProcess(m_popPositions, type, stakeID));
}

bool PopControl::AreOptionsVisible() const
{
	return m_popOptionsVisible;
}