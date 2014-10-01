#include <ObjectHandler.h>
#include <TextureManager.h>
#include "Game.h"
#include "ShowStakeOptionsProcess.h"

static const float MOVE_SPEED_SD = 600.0f;
static const float MOVE_SPEED_HD = 900.0f;

unsigned int AllStakes[4] = {25, 50, 100, 200};

ShowStakeOptionsProcess::ShowStakeOptionsProcess(const std::vector<D3DXVECTOR2>& positions, ChangeType type, int oldStakeID)
{
	mChangeType = type;
	mSpeed = 0.0f;
	mOldStakeID = oldStakeID;
	pOptionOverlay = nullptr;
	mEndPositions = positions;
	if(TheGame::Instance()->IsHD())
	{
		mBaseMoveSpeed = MOVE_SPEED_HD;
	}
	else
	{
		mBaseMoveSpeed = MOVE_SPEED_SD;
	}
	
}

ShowStakeOptionsProcess::~ShowStakeOptionsProcess()
{

}

void ShowStakeOptionsProcess::Init()
{
	pStakeButton = TheObjectHandler::Instance()->GetObject2D("RGlassButtons")->GetInstance(1);

	Object2D* buttons = TheObjectHandler::Instance()->GetObject2D("PopButtons");

	buttons->SetVisible(true);

	D3DXVECTOR2 floatingPosition = mEndPositions[0];

	auto newStakeID = TheGame::Instance()->GetStakeID();

	int positionCount = 0;
	
	for(unsigned int i = 0; i < buttons->GetNumberOfInstances() - 1;++i)// - 1; i >= 0; --i)
	{
		if(!TheGame::Instance()->HasStake(AllStakes[i]))
		{
			continue;
		}

		Instance2D* option = buttons->GetInstance(i);		
		pOptions.push_back(option);		
						
		if(mChangeType == SHOW_OPTIONS ||
			(mChangeType == UPDATE_OPTIONS && i == newStakeID))
		{
			if(!option->GetVisible())
			{
				D3DXVECTOR2 pos = option->GetPosition();
				pos.y = pStakeButton->GetPosition().y;			
				option->SetPosition(pos);	
				option->SetVisible(true);
			}
		}			
		
		if(mChangeType != SHOW_OPTIONS)
		{		
			if(i == newStakeID)
			{
				mEndPositions[positionCount] = floatingPosition;
				option->SetVisible(true);
			}
			else
			{
				mEndPositions[positionCount].y = pStakeButton->GetPosition().y;	
			}
		}
		bool inPosition = false;
		if(mChangeType == UPDATE_OPTIONS && 
			(i != newStakeID &&
			 i != mOldStakeID))
			 inPosition = true;

		if(mChangeType == SHOW_OPTIONS && i == mOldStakeID)
		{
			inPosition = true;
		}

		++positionCount;

		mInPosition.push_back(inPosition);
	}

	if(mChangeType == HIDE_OPTIONS)
	{
		pOptionOverlay = buttons->GetInstance(buttons->GetNumberOfInstances() - 1);
		pOptionOverlay->SetPosition(pOptions[newStakeID]->GetPosition());
		pOptionOverlay->SetCurrentSprite(pOptions[newStakeID]->GetCurrentSprite());
		pOptionOverlay->SetVisible(true);
	}

	mSpeed = -mBaseMoveSpeed;

	
	std::string texture = "TouchUp.png";
	
	if(mChangeType != SHOW_OPTIONS)
	{
		mSpeed = mBaseMoveSpeed;
		texture = "ChangeStakeUp.png";		
	}
	unsigned int sprite = TheTextureManager::Instance()->GetTextureClass(texture)->GetAtlas()->GetAtlasPieceID(texture);
	pStakeButton->SetCurrentSprite(sprite);	
}

void ShowStakeOptionsProcess::Complete()
{
	Object2D* buttons = TheObjectHandler::Instance()->GetObject2D("PopButtons");
	for(size_t i = 0; i < buttons->GetNumberOfInstances();++i)
	{
		if(i != TheGame::Instance()->GetStakeID())
		{
			if(mChangeType != SHOW_OPTIONS)
				buttons->GetInstance(i)->SetVisible(false);
		}
	}

	if(pOptionOverlay)
	{
		pOptionOverlay->SetVisible(false);
	}
}

void ShowStakeOptionsProcess::Update()
{
	bool allInPosition = true;
	for(size_t i = 0; i < pOptions.size();++i)
	{
		if(mInPosition[i])
		{
			continue;
		}

		allInPosition = false;

		float yPosition = mEndPositions[i].y;

		D3DXVECTOR2 pos = pOptions[i]->GetPosition();
		bool up = false;
		if(pos.y < yPosition)
		{
			mSpeed = mBaseMoveSpeed;
		}
		else
		{
			mSpeed = -mBaseMoveSpeed;
			up = true;
		}

		float yChange = mSpeed * TheEngine::Instance()->GetSystemTimer().GetDT();

		if(up)
		{
			if((pos.y + yChange) <= yPosition)
			{
				yChange = yPosition - pos.y;					
				mInPosition[i] = true;
				if(!i && i != mOldStakeID)//if first one is locked before currently visible stake then enable current to move
				{
					mInPosition[mOldStakeID] = false;
				}
			}
		}
		else
		{
			if(pos.y + yChange >= yPosition)
			{
				yChange = yPosition - pos.y;
				mInPosition[i] = true;
			}
		}
		pos.y += yChange;

		pOptions[i]->SetPosition(pos);

		if(pOptionOverlay && i == TheGame::Instance()->GetStakeID())
		{
			pOptionOverlay->SetPosition(pos);
		}
	}

	if(allInPosition)
	{
		SetCompleted();
	}
}