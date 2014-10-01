#include <ObjectHandler.h>
//#include <UpdateBehaviourFactory.h>
#include "TextAwayBehaviour.h"

/*
UpdateBehaviour* CreateTextAwayBehaviour()
{
	return new TextAwayBehaviour();
}*/

static bool a = TheUpdateBehaviourFactory::Instance()->AddType("TextAway", CreateTextAwayBehaviour);

TextAwayBehaviour::TextAwayBehaviour()
{
	m_text = 0;
	m_numFrames = 0;
	m_frameCount = 0;
	m_startingScale = D3DXVECTOR2(0.0f, 0.0f);
	m_moveAmount = D3DXVECTOR2(0.0f, 0.0f);
	m_currentMoveAmount = D3DXVECTOR2(0.0f, 0.0f);
	m_bounceIncrement = 0.0f;
	m_bounceMaxHeight = D3DXVECTOR2(0.0f, 0.0f);
	m_velocity = D3DXVECTOR2(0.0f, 0.0f);
	m_valuesSet = false;
}

TextAwayBehaviour::TextAwayBehaviour(Text* text, const D3DXVECTOR2& startingScale, const D3DXVECTOR2& moveAmount, 
	const D3DXVECTOR2& bounceMaxHeight, float bounceIncrement, unsigned int numFrames)
{
	SetValues(text, startingScale, moveAmount, bounceMaxHeight, bounceIncrement, numFrames);
}

TextAwayBehaviour::~TextAwayBehaviour()
{

}

void TextAwayBehaviour::ResetValues()
{
	m_text->SetPosition(m_text->GetPosition() - (m_moveAmount * (float)m_frameCount));
	m_frameCount = 0;
	m_valuesSet = false;
}

void TextAwayBehaviour::SetValues(Text* text, const D3DXVECTOR2& startingScale, const D3DXVECTOR2& moveAmount, 
	const D3DXVECTOR2& bounceMaxHeight, float bounceIncrement, unsigned int numFrames)
{
	m_text = text;
	m_numFrames = numFrames;
	m_frameCount = 0;
	m_startingScale = startingScale;
	m_moveAmount = moveAmount;
	m_velocity = moveAmount / (float)m_numFrames;
	m_bounceIncrement = bounceIncrement;
	m_bounceMaxHeight = bounceMaxHeight;
	m_currentMoveAmount = D3DXVECTOR2(0.0f, 0.0f);
	m_valuesSet = false;
}

void TextAwayBehaviour::Update(float deltaTime)
{
	if(m_triggered)
	{
		deltaTime = 0.01f;
		if(!m_text)
		{
			m_text = TheObjectHandler::Instance()->GetText(m_textName);
		}

		if(!m_valuesSet)
		{
			m_text->SetScale(m_startingScale);
			m_text->SetShadowOffset(D3DXVECTOR2(m_startingScale.x * 10, m_startingScale.y * 10));
			m_valuesSet = true;
		}
		D3DXVECTOR2 absMoveAmount(fabs(m_moveAmount.x), fabs(m_moveAmount.y));
		m_currentMoveAmount += D3DXVECTOR2(fabs(m_velocity.x), fabs(m_velocity.y))*deltaTime;

		D3DXVECTOR2 adjustment = (m_velocity);
		m_text->SetPosition(m_text->GetPosition() + adjustment);

		m_frameCount++;
		float scaleAmount = (float)m_frameCount/ (float)m_numFrames;
		D3DXVECTOR2 scale = m_startingScale;
		scale -= D3DXVECTOR2(scaleAmount, scaleAmount);
		m_text->SetScale(scale);

		if(m_text->GetScale().x < 0.0f || m_text->GetScale().x < 0.0f)
		{
			m_text->SetScale(D3DXVECTOR2(0.0f, 0.0f));
			m_triggered = false;
			ResetValues();
		}
		m_text->SetShadowOffset(D3DXVECTOR2(m_text->GetScale().x * 15, m_text->GetScale().y * 15));
	}
}

bool TextAwayBehaviour::Load(XMLRead* file)
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
	m_valuesSet = false;
	
	return true;
}