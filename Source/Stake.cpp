#include "PokerGame.h"
#include <ObjectHandler.h>
#include <TextureManager.h>

unsigned int PokerGame::GetStake(void)
{
unsigned int TheStake;

	if (GetGameIndex() == Game100p)
		TheStake = MINIMUM_BET_100p;
	else
		TheStake = MINIMUM_BET_200p;

	return(TheStake);
}

unsigned int PokerGame::Get100pStake(void)
{
	return(MINIMUM_BET_100p);
}

unsigned int PokerGame::Get200pStake(void)
{
	return(MINIMUM_BET_200p);
}

unsigned char PokerGame::AllowStakeButton(void)
{
unsigned char Allow;

	Allow = 0;

	return(Allow);
}

void PokerGame::SetGameIndex(unsigned int GameIndexValue)
{
	GameIndex = GameIndexValue;
}

unsigned int PokerGame::GetGameIndex(void)
{
	return(GameIndex);
}

void PokerGame::ChangeStake(void)
{	
	GameIndex++;
	if (GameIndex >= MaxSelectableGames)
		GameIndex = 0;
}

void PokerGame::DisplayStake(void)
{
unsigned int ID;

	if (GetGameIndex() == Game100p)	
		ID = TheTextureManager::Instance()->GetTextureID("Play100p.png");				
	else
		ID = TheTextureManager::Instance()->GetTextureID("Play200p.png");
	
	TheObjectHandler::Instance()->GetObject2D("PriceOfPlay")->SetTextureID(ID);	
	TheObjectHandler::Instance()->GetObject2D("PriceOfPlay")->SetVisible(true);
}