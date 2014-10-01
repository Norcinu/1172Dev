#include <ObjectHandler.h>
#include <TextureManager.h>
//#include <UpdateBehaviourFactory.h>
#include "ImagePatternBehaviour.h"

/*
UpdateBehaviour* CreateImagePattern()
{
	return new ImagePatternBehaviour;
}*/

//static bool a = TheUpdateBehaviourFactory::Instance()->AddType("ImagePattern", CreateImagePattern);

ImagePatternBehaviour::ImagePatternBehaviour()
{
	m_spread = 0;
	m_rotatingInterval = 0.0f;
	m_holdTime = 0.0f;
	m_runningTime = 0.0f;
	m_droppingLines = 0;
	m_fallSpeed = 0.0f;
	m_allInPosition = false;
}

ImagePatternBehaviour::~ImagePatternBehaviour()
{

}

void ImagePatternBehaviour::SetTriggered(bool triggered)
{
	m_triggered = triggered;
	m_rotatingTimeAccumulated = 0.0f;
	m_runningTime = 0.0f;
	m_allInPosition = false;
}

void ImagePatternBehaviour::Update(float deltaTime)
{
	if(m_triggered)
	{
		bool allFallen = true;
		
		while(m_object->GetNumberOfInstances() != m_positions.size())
		{
			unsigned int numInstances = m_object->GetNumberOfInstances();
			m_positions[numInstances].m_increment = m_moveSpeed;
			m_positions[numInstances].m_velocity = (m_positions[numInstances].m_position - m_startingPosition) / (float)m_moveSpeed;
			
			m_positions[numInstances].m_fallSpeed = GetRandomFloat(m_fallSpeed / 2, m_fallSpeed);
			unsigned int numFrames = TheTextureManager::Instance()->GetTextureClass(m_object->GetTexture1ID())->GetAtlas()->m_numTextures;		
			unsigned int sprite = m_positions[numInstances].m_line;

			if(sprite>=numFrames)
			{
				sprite -=numFrames;
			}
			
			m_object->AddAnInstance(true, m_object->GetColour(), m_startingPosition, m_object->GetScale(), m_object->GetRotationCentre(), m_object->GetRotationCentre(), 0.0f, sprite);
		}

		m_object->SetVisible(true);
		m_rotatingTimeAccumulated += deltaTime;

		for(unsigned int i = 0;  i < m_object->GetNumberOfInstances();i++)
		{
			if(m_rotatingTimeAccumulated >= m_rotatingInterval)
			{
				m_object->GetInstance(i)->SpriteIncrement();
			}

			if(!m_allInPosition)
			{
				m_allInPosition = true;

				if(m_positions[i].m_increment <= 0)
				{
					continue;
				}
				else
				{
					m_object->GetInstance(i)->SetPosition(m_object->GetInstance(i)->GetPosition() + m_positions[i].m_velocity);
					m_positions[i].m_increment--;					
					m_allInPosition = false;
				}
			}
			else
			{
				if(m_runningTime > m_holdTime) //begin Dropping
				{			
					D3DXVECTOR2 position = m_object->GetInstance(i)->GetPosition();
					position.y += m_positions[i].m_fallSpeed;
					m_object->GetInstance(i)->SetPosition(position);
				}

				if(m_object->GetInstance(i)->GetPosition().y < TheEngine::Instance()->GetScreenHeight())
				{
					allFallen = false;
				}		
			}
		}		

		if(m_allInPosition)
		{
			m_runningTime += deltaTime;
		}
		if(m_rotatingTimeAccumulated >= m_rotatingInterval)
		{
			m_rotatingTimeAccumulated -= m_rotatingInterval;
		}

		if(allFallen && m_allInPosition)
		{
			m_triggered = false;
			m_object->SetVisible(false);
			
			for(unsigned int i = 0; i < m_object->GetHighestInstanceID() + 1; i++)
			{
				if(m_object->HasInstance(i))
				{
					m_object->RemoveInstance(i);
				}
			}

			m_rotatingTimeAccumulated = 0.0f;
			m_runningTime = 0.0f;
			m_droppingLines = m_totalLines;
		}
	}
}

bool ImagePatternBehaviour::Load(XMLRead* file)
{
	while(!file->GetNodeComplete("UpdateBehaviour"))
	{
		if(file->IsNodeName("Settings"))
		{
			std::string objectName;
			file->GetAttributeString("Settings", "ObjectName", &objectName);
			m_object = TheObjectHandler::Instance()->GetObject2D(objectName);

			file->GetAttributeString("Settings", "PatternTexture", &m_patternTextureName);
			file->GetAttributeUnsignedInt("Settings", "CoinSpread", &m_coinSpread);
			
			IDirect3DTexture9* texture = TheTextureManager::Instance()->GetTexture(m_patternTextureName);			

			D3DSURFACE_DESC texData;
			texture->GetLevelDesc(0, &texData);
			D3DXVECTOR2 pos = m_object->GetPosition();
			unsigned int width = texData.Width;
			unsigned int height = texData.Height;
			float centreX = (float)texData.Width / 2;
			float centreY = (float)texData.Height / 2;
			D3DLOCKED_RECT lr;

			HRESULT hResult = texture->LockRect(0,&lr, NULL, 0);

			if(FAILED(hResult))
			{
				return false;
			}
			BYTE* bytePointer=(BYTE*)lr.pBits; 

			 for (unsigned int y = 0; y < height; y++)
			{
				for (unsigned int x = 0 ; x < width; x++)
				{
					unsigned int index =(x * 4 + (y * (lr.Pitch)));

					BYTE B = bytePointer[index];
					BYTE G = bytePointer[index+1];
					BYTE R = bytePointer[index+2];
					BYTE A = bytePointer[index+3];

					if(R < 255 || B < 255 || G < 255)
					{
						int xPos = (int)((x - centreX) * m_coinSpread);
						int yPos = (int)((y - centreY) * m_coinSpread);
				
						m_positions.push_back(PosLinePair(D3DXVECTOR2(pos.x + xPos, pos.y + yPos), width - x));
					}
				}
			}
			texture->UnlockRect(0);

			m_totalLines = m_droppingLines = width;
			
			file->GetAttributeFloat("Settings", "RotatingInterval", &m_rotatingInterval);
			file->GetAttributeFloat("Settings", "HoldTime", &m_holdTime);
			file->GetAttributeFloat("Settings", "FallSpeed", &m_fallSpeed);
			file->GetAttributeUnsignedInt("Settings", "MoveSpeed", &m_moveSpeed);
		}
		
		if(file->IsNodeName("StartingPosition"))
		{
			file->GetAttributeD3DXVECTOR2("StartingPosition", &m_startingPosition);
		}
		file->Read();
	}
	return true; 
}