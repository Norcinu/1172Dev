#include <ObjectHandler.h>
//#include <UpdateBehaviourFactory.h>
#include "TextParabolicBehaviour.h"

/*
UpdateBehaviour* CreateTextParabolicBehaviour()
{
	return new TextParabolicBehaviour();
}

static bool a = TheUpdateBehaviourFactory::Instance()->AddType("TextParabolic", CreateTextParabolicBehaviour);
*/

TextParabolicBehaviour::TextParabolicBehaviour()
{
	m_text = 0;
	m_numFrames = 0;
	m_frameCount = 0;
	m_startingScale = D3DXVECTOR2(0.0f, 0.0f);
	m_moveAmount = D3DXVECTOR2(0.0f, 0.0f);
	m_bounceIncrement = 0.0f;
	m_bounceMaxHeight = D3DXVECTOR2(0.0f, 0.0f);
	m_bounceValue = 0.0f;
	m_valuesSet = false;
}

TextParabolicBehaviour::TextParabolicBehaviour(Text* text, const D3DXVECTOR2& startingScale, const D3DXVECTOR2& moveAmount, 
	const D3DXVECTOR2& bounceMaxHeight, float bounceIncrement, unsigned int numFrames)
{
	SetValues(text, startingScale, moveAmount, bounceMaxHeight, bounceIncrement, numFrames);
}

TextParabolicBehaviour::~TextParabolicBehaviour()
{

}

void TextParabolicBehaviour::ResetValues()
{
	m_bounceValue = sin(m_startingScale.x);
	m_text->SetPosition(m_text->GetPosition() - (m_moveAmount * (float)m_frameCount));
	m_frameCount = 0;
	m_valuesSet = false;
}

void TextParabolicBehaviour::SetTriggered(bool triggered)
{
	if(!m_text)
	{
		m_text = TheObjectHandler::Instance()->GetText(m_textName);
	}

	if(m_triggered == triggered)
	{
		m_valuesSet = false;
		ResetValues();
	}	
	m_triggered = triggered;	
}

void TextParabolicBehaviour::SetValues(Text* text, const D3DXVECTOR2& startingScale, const D3DXVECTOR2& moveAmount, 
	const D3DXVECTOR2& bounceMaxHeight, float bounceIncrement, unsigned int numFrames)
{
	m_text = text;
	m_numFrames = numFrames;
	m_frameCount = 0;
	m_startingScale = startingScale;
	m_moveAmount = moveAmount / (float)m_numFrames;
	m_bounceIncrement = bounceIncrement;
	m_bounceMaxHeight = bounceMaxHeight;
	m_bounceValue = sin(m_startingScale.x);
	m_valuesSet = false;
}

void TextParabolicBehaviour::Update(float deltaTime)
{
	if(m_triggered)
	{
		if(!m_valuesSet)
		{
			m_text->SetScale(m_startingScale);
			m_text->SetShadowOffset(D3DXVECTOR2(m_startingScale.x * 10, m_startingScale.y * 10));
			m_valuesSet = true;
		}

		m_bounceValue += (m_bounceIncrement*deltaTime);
		m_text->SetPosition(m_text->GetPosition() + m_moveAmount);
		
		m_text->SetShadowOffset(D3DXVECTOR2(m_text->GetScale().x * 15, m_text->GetScale().y * 15));
			
		if(sin(m_bounceValue) < 0.0f)
		{
			m_bounceValue -= (m_bounceIncrement*deltaTime);
			D3DXVECTOR2 scale = m_text->GetScale();
			scale -= D3DXVECTOR2(sin(m_bounceIncrement*deltaTime) * m_bounceMaxHeight.x, 
				sin(m_bounceIncrement*deltaTime) * m_bounceMaxHeight.y);
			m_text->SetScale(scale);

			if(m_text->GetScale().x < 0.0f || m_text->GetScale().x < 0.0f)
			{
				m_text->SetScale(D3DXVECTOR2(0.0f, 0.0f));
			}
		}
		else
		{
			D3DXVECTOR2 bounceValue(sin(m_bounceValue) * m_bounceMaxHeight.x, 
				sin(m_bounceValue) * m_bounceMaxHeight.y);
			m_text->SetScale(m_startingScale + bounceValue); 
		}

		m_frameCount++;

		if(m_frameCount > m_numFrames)
		{
			m_triggered = false;
			m_text->SetVisible(false);
			ResetValues();
		}
	}
}

bool TextParabolicBehaviour::Load(XMLRead* file)
{
	D3DXVECTOR2 moveAmount(0.0f, 0.0f);

	while(!file->GetNodeComplete("UpdateBehaviour"))
	{
		if(file->IsNodeName("Settings"))
		{
			file->GetAttributeString("Settings", "TextName", &m_textName);
			file->GetAttributeUnsignedInt("Settings", "NumFrames", &m_numFrames);
			file->GetAttributeFloat("Settings", "BounceIncrement", &m_bounceIncrement);
		}

		if(file->IsNodeName("StartingScale"))
		{
			file->GetAttributeD3DXVECTOR2("StartingScale", &m_startingScale);
		}

		if(file->IsNodeName("MoveAmount"))
		{
			file->GetAttributeD3DXVECTOR2("MoveAmount", &moveAmount);
		}

		if(file->IsNodeName("BounceMaxHeight"))
		{
			file->GetAttributeD3DXVECTOR2("BounceMaxHeight", &m_bounceMaxHeight);
		}

		file->Read();
	}

	m_frameCount = 0;
	m_moveAmount = moveAmount / (float)m_numFrames;
	m_bounceValue = sin(m_startingScale.x);
	m_valuesSet = false;
	
	return true;
}