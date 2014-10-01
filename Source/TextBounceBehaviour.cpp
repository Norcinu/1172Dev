#include <ObjectHandler.h>
//#include <UpdateBehaviourFactory.h>
#include "TextBounceBehaviour.h"

/*
UpdateBehaviour* CreateTextBounceBehaviour()
{
	return new TextBounceBehaviour();
}

static bool a = TheUpdateBehaviourFactory::Instance()->AddType("TextBounce", CreateTextBounceBehaviour);
*/

TextBounceBehaviour::TextBounceBehaviour()
{
	m_text = 0;
	m_startingScale = D3DXVECTOR2(0.0f, 0.0f);
	m_bounceIncrement = 0.0f;
	m_bounceMaxHeight = D3DXVECTOR2(0.0f, 0.0f);
	m_bounceValue = 0.0f;
	m_valuesSet = false;
	m_stop = false;
}

TextBounceBehaviour::TextBounceBehaviour(Text* text, const D3DXVECTOR2& startingScale, 
	const D3DXVECTOR2& bounceMaxHeight, float bounceIncrement)
{
	SetValues(text, startingScale, bounceMaxHeight, bounceIncrement);
}

void TextBounceBehaviour::ResetValues()
{
	m_bounceValue = sin(m_startingScale.x);
	m_valuesSet = false;
}

void TextBounceBehaviour::SetTriggered(bool triggered)
{
	if(!m_text)
	{
		m_text = TheObjectHandler::Instance()->GetText(m_textName); 
		m_shadowOffset = m_text->GetShadowOffset();
	}

	if(m_triggered == triggered)
	{
		m_valuesSet = false;
		ResetValues();
	}
	if(triggered) //if starting then reset stop flag
	{
		m_stop = false;
		m_triggered = triggered;
	}
	else
	{
		m_stop = true;
	}
	
}

void TextBounceBehaviour::SetValues(Text* text, const D3DXVECTOR2& startingScale, 
	const D3DXVECTOR2& bounceMaxHeight, float bounceIncrement)
{
	m_text = text;
	m_startingScale = startingScale;
	m_bounceIncrement = bounceIncrement;
	m_bounceMaxHeight = bounceMaxHeight;
	m_bounceValue = sin(m_startingScale.x);
	m_valuesSet = false;
}

void TextBounceBehaviour::Update(float deltaTime)
{
	if(m_triggered)
	{
		if(!m_stop)
		{
			if(!m_valuesSet)
			{
				m_text->SetShadowOffset(D3DXVECTOR2(m_startingScale.x * 10, m_startingScale.y * 10));
				m_valuesSet = true;
			}

			m_bounceValue += m_bounceIncrement*deltaTime;
	
			D3DXVECTOR2 shadowScale = (m_text->GetScale() + m_bounceMaxHeight) - m_startingScale;
			m_text->SetShadowOffset(D3DXVECTOR2(shadowScale.x * 25, shadowScale.y * 25));
			
			D3DXVECTOR2 bounceValue(sin(m_bounceValue) * m_bounceMaxHeight.x, 
				sin(m_bounceValue) * m_bounceMaxHeight.y);
			m_text->SetScale(m_startingScale + bounceValue); 
		}
		else
		{
			if(m_valuesSet)
			{
				m_bounceValue += m_bounceIncrement*deltaTime;
	
				D3DXVECTOR2 shadowScale = (m_text->GetScale() + m_bounceMaxHeight) - m_startingScale;
				m_text->SetShadowOffset(D3DXVECTOR2(shadowScale.x * 25, shadowScale.y * 25));
			
				D3DXVECTOR2 bounceValue(sin(m_bounceValue) * m_bounceMaxHeight.x, 
					sin(m_bounceValue) * m_bounceMaxHeight.y);
				m_text->SetScale(m_startingScale + bounceValue); 

				D3DXVECTOR2 originalSize = m_startingScale;// - m_bounceMaxHeight;
				if(originalSize.x > m_text->GetScale().x - 0.1f && 
					originalSize.x < m_text->GetScale().x + 0.1f)
				{
					m_text->SetScale(originalSize);
					m_text->SetShadowOffset(m_shadowOffset);
					m_valuesSet = false;
					ResetValues();
					m_triggered = false;
				}
			}
		}
	}
}

bool TextBounceBehaviour::Load(XMLRead* file)
{
	while(!file->GetNodeComplete("UpdateBehaviour"))
	{
		if(file->IsNodeName("Settings"))
		{
			file->GetAttributeString("Settings", "TextName", &m_textName);
			file->GetAttributeFloat("Settings", "BounceIncrement", &m_bounceIncrement);
		}

		if(file->IsNodeName("StartingScale"))
		{
			file->GetAttributeD3DXVECTOR2("StartingScale", &m_startingScale);
		}

		if(file->IsNodeName("BounceMaxHeight"))
		{
			file->GetAttributeD3DXVECTOR2("BounceMaxHeight", &m_bounceMaxHeight);
		}

		file->Read();
	}

	m_bounceValue = sin(m_startingScale.x);
	m_valuesSet = false;
	
	return true;
}