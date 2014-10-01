//#include <UpdateBehaviourFactory.h>
#include "SlideBehaviour.h"

/*
UpdateBehaviour* CreateSlide()
{
	return new SlideBehaviour;
}

static bool j = TheUpdateBehaviourFactory::Instance()->AddType("Slide", CreateSlide);*/

SlideBehaviour::SlideBehaviour()
{
	m_object = 0;
	m_titleText = 0;
	m_numbersText = 0;
	m_slideToEnd = true;
	m_startingPosition = D3DXVECTOR2(0.0f,0.0f);
	m_endAdjustment = D3DXVECTOR2(0.0f,0.0f);
	m_velocity = 0.0f;
	m_floatEpsilon = D3DXVECTOR2(1.0f,1.0f);
	m_currentAdjustment = D3DXVECTOR2(0,0);
	m_init = false;
}

void SlideBehaviour::SetTriggered(bool triggered)
{
	if(!m_init)
	{
		m_numbersText = TheObjectHandler::Instance()->GetText(m_numbersName);	
		m_titleText = TheObjectHandler::Instance()->GetText(m_titleName);

		if(!m_textInEndPos)
		{
			m_titleStartPos = m_titleText->GetPosition()- m_endAdjustment;
			m_numbersStartPos = m_numbersText->GetPosition()- m_endAdjustment;
		}
		else
		{
			m_titleStartPos = m_titleText->GetPosition();
			m_numbersStartPos = m_numbersText->GetPosition();
		}
			m_titleText->SetPosition(m_titleStartPos);
			m_numbersText->SetPosition(m_numbersStartPos);

		m_init = true;
	}
	
	if(!m_triggered)
	{
		m_triggered = true;
		m_object->SetVisible(true);
		m_titleText->SetVisible(true);
		m_numbersText->SetVisible(true);
	}	
}

bool SlideBehaviour::CheckWithEpsilon(D3DXVECTOR2 vec, D3DXVECTOR2 comparsionVec)
{
	D3DXVECTOR2 plus = comparsionVec + m_floatEpsilon;
	D3DXVECTOR2 minus = comparsionVec - m_floatEpsilon;
	
	if(vec.x <= plus.x && vec.x >= minus.x && vec.y <= plus.y && vec.y >= minus.y)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void SlideBehaviour::Update(float deltaTime)
{
	if(m_triggered)
	{
		if(m_slideToEnd)
		{
			if(!CheckWithEpsilon(m_currentAdjustment, m_endAdjustment))
			{
				if(m_currentAdjustment.x < m_endAdjustment.x)
				{
					m_currentAdjustment.x += m_velocity;
				}
				else if(m_currentAdjustment.x > m_endAdjustment.x)
				{
					m_currentAdjustment.x -= m_velocity;
				}

				if(m_currentAdjustment.y < m_endAdjustment.y)
				{
					m_currentAdjustment.y += m_velocity;				
				}
				else if(m_currentAdjustment.y > m_endAdjustment.y)
				{
					m_currentAdjustment.y -= m_velocity;
				}
				m_titleText->SetPosition(m_titleStartPos + m_currentAdjustment);
				m_numbersText->SetPosition(m_numbersStartPos + m_currentAdjustment);
				m_object->SetPosition(m_currentAdjustment + m_startingPosition);
			}
			else
			{
				m_slideToEnd = !m_slideToEnd;
				m_triggered = false;
				if(m_object->GetPosition().y<0)
				{
					m_object->SetVisible(false);
					m_titleText->SetVisible(false);
					m_numbersText->SetVisible(false);
				}
			}
		}
		else
		{
			if(!CheckWithEpsilon(m_currentAdjustment, D3DXVECTOR2(0.0f,0.0f)))
			{
				if(m_currentAdjustment.x<0.0f)
				{
					m_currentAdjustment.x +=m_velocity;				
				}
				else if(m_currentAdjustment.x > 0.0f)
				{
					m_currentAdjustment.x -= m_velocity;
				}
				if(m_currentAdjustment.y < 0.0f)
				{
					m_currentAdjustment.y +=m_velocity;				
				}
				else if(m_currentAdjustment.y > 0.0f)
				{
					m_currentAdjustment.y -= m_velocity;
				}
				m_titleText->SetPosition(m_titleStartPos+m_currentAdjustment);
				m_numbersText->SetPosition(m_numbersStartPos+m_currentAdjustment);
				m_object->SetPosition(m_currentAdjustment+m_startingPosition);
			}
			else
			{
				m_currentAdjustment = D3DXVECTOR2(0.0f,0.0f);
				m_slideToEnd = !m_slideToEnd;
				m_triggered = false;
				if(m_object->GetPosition().y<0)
				{
					m_object->SetVisible(false);
					m_titleText->SetVisible(false);
					m_numbersText->SetVisible(false);
				}
			}		
		}
	}
}

void SlideBehaviour::SetAtEnd(bool toEnd)
{
	if(m_slideToEnd!=toEnd)
	{
		m_slideToEnd = toEnd;
	}
}

bool SlideBehaviour::Load(XMLRead* file)
{
	std::string objectName;

	while(!file->GetNodeComplete("UpdateBehaviour"))
	{
		if(file->IsNodeName("Object"))			
		{
			file->GetAttributeString("Object", "Name", &objectName);
			m_object = TheObjectHandler::Instance()->GetObject2D(objectName);
		}

		if(file->IsNodeName("TitleText"))
		{
			file->GetAttributeString("TitleText", "Name", &m_titleName);
		}
		
		if(file->IsNodeName("Text"))
		{
			file->GetAttributeString("Text", "Name", &m_numbersName);
		}

		if(file->IsNodeName("StartPosition"))
		{
			file->GetAttributeD3DXVECTOR2("StartPosition", &m_startingPosition);
		}
	
		if(file->IsNodeName("EndPosition"))
		{
			file->GetAttributeD3DXVECTOR2("EndPosition", &m_endAdjustment);
		}

		if(file->IsNodeName("Settings"))
		{
			file->GetAttributeFloat("Settings", "Velocity", &m_velocity);
			file->GetAttributeBool("Settings", "TextAtEnd", &m_textInEndPos);
		}
	
		file->Read();
	}

	return true;
}