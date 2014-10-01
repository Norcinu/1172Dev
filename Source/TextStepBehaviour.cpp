#include <ObjectHandler.h>
//#include <UpdateBehaviourFactory.h>
#include "TextStepBehaviour.h"

/*
UpdateBehaviour* CreateTextStepBehaviour()
{
	return new TextStepBehaviour();
}*/

static bool a = TheUpdateBehaviourFactory::Instance()->AddType("TextStepBehaviour", CreateTextStepBehaviour);

TextStepBehaviour::TextStepBehaviour()
{
	m_text = 0;
	m_numFrames = 0;
	m_frameCount = 0;
	m_startingScale = D3DXVECTOR2(0.0f, 0.0f);
	m_bounceIncrement = 0.0f;
	m_bounceValue = 0.0f;
	m_valuesSet = false;
}

TextStepBehaviour::TextStepBehaviour(Text* text, float bounceMaxHeight, float bounceIncrement, unsigned int numFrames)
{
	SetValues(text, bounceMaxHeight, bounceIncrement, numFrames);
}

TextStepBehaviour::~TextStepBehaviour()
{

}

void TextStepBehaviour::ResetValues()
{
	m_startingScale = m_text->GetScale();
	m_bounceValue = sin(m_startingScale.x);
	m_frameCount = 0;
	m_valuesSet = false;
}

void TextStepBehaviour::SetTriggered(bool triggered)
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

void TextStepBehaviour::SetValues(Text* text, float bounceMaxHeight, float bounceIncrement, unsigned int numFrames)
{
	m_text = text;
	m_numFrames = numFrames;
	m_frameCount = 0;
	m_startingScale = m_text->GetScale();
	m_bounceIncrement = bounceIncrement;
	m_bounceMaxHeight = bounceMaxHeight;
	m_bounceValue = sin(m_startingScale.x);
	m_valuesSet = false;
}

void TextStepBehaviour::Update(float deltaTime)
{
	if(m_triggered)
	{
		if(!m_valuesSet)
		{
			m_text->SetShadowOffset(D3DXVECTOR2(m_startingScale.x * 10, m_startingScale.y * 10));
			m_valuesSet = true;
		}

		m_bounceValue += m_bounceIncrement;//*deltaTime;

		if(m_bounceValue < (D3DX_PI / 2))
		{
			m_text->SetShadowOffset(D3DXVECTOR2(m_text->GetScale().x * 15, m_text->GetScale().y * 15));
			D3DXVECTOR2 bounceValue(sin(m_bounceValue) * m_bounceMaxHeight, sin(m_bounceValue) * m_bounceMaxHeight);
			m_text->SetScale(m_startingScale + bounceValue); 
		}	

		m_frameCount++;

		if(m_frameCount > m_numFrames)
		{
			m_triggered = false;
			ResetValues();
		}
	}
}

bool TextStepBehaviour::Load(XMLRead* file)
{
	while(!file->GetNodeComplete("UpdateBehaviour"))
	{
		if(file->IsNodeName("Settings"))
		{
			file->GetAttributeString("Settings", "TextName", &m_textName);
			file->GetAttributeUnsignedInt("Settings", "NumFrames", &m_numFrames);
			file->GetAttributeFloat("Settings", "BounceIncrement", &m_bounceIncrement);
			file->GetAttributeFloat("Settings", "BounceMaxHeight", &m_bounceMaxHeight);
		}

		if(file->IsNodeName("StartingScale"))
		{
			file->GetAttributeD3DXVECTOR2("StartingScale", &m_startingScale);
		}
		file->Read();
	}
	m_frameCount = 0;
	m_bounceValue = sin(m_startingScale.x);
	m_valuesSet = false;

	return true;
}