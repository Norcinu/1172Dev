//#include <UpdateBehaviourFactory.h>
#include "ButtonPressBehaviour.h"


ButtonPressBehaviour::ButtonPressBehaviour()
{
	m_object = 0;
	m_reversing = false;
	m_numFrames = 0;
}

void ButtonPressBehaviour::SetTriggered(bool triggered)
{
	Texture* tex = TheTextureManager::Instance()->GetTextureClass(m_object->GetTextureID()); 
	m_numFrames = tex->GetAtlas()->m_numTextures;
	if(triggered)
	{
		m_reversing = false;
		m_triggered = true;
	}
	else
	{
		m_reversing = true;
		m_triggered = true;
	}
}

void ButtonPressBehaviour::Update(float deltaTime)
{
	if(m_triggered)
	{		
		if(m_object->GetCurrentSprite() == 0 && m_reversing)
		{
			m_reversing = false;
			m_triggered = false;
			return;
		}
	
		if(!m_reversing)
		{
			if(m_object->GetCurrentSprite() < (unsigned)(m_numFrames - 1))
			{
				m_object->SetCurrentSprite(m_object->GetCurrentSprite() + 1);
			}
		}
		else
		{	
			if(m_object->GetCurrentSprite() > 0)
			{
				m_object->SetCurrentSprite(m_object->GetCurrentSprite() - 1);
			}
		}
	}
}

bool ButtonPressBehaviour::Load(XMLRead* file)
{
	/*std::string objectName;
	while(!file->GetNodeComplete("UpdateBehaviour"))
	{
		if(file->IsNodeName("Object"))
		{
			file->GetAttributeString("Object", "Name", &objectName);
			m_object = TheObjectHandler::Instance()->GetObject2D(objectName); 
		}
		file->Read();
	}*/
	return true;
}