#include "Game.h"
#include <ObjectHandler.h>
#include <TextureManager.h>
#include "PokerGame.h"

void Game::DrawCurrentError()
{
	static int PlayOnce;
	unsigned int MessageID;
	
	if(GetCurrentError())								//ACTUAL ERRORS
	{
		if(!PlayOnce)
		{
#ifndef FAST_PLAY
			TheAudioManager::Instance()->GetAudioSample("ALARM")->Play(true);
#endif
			PlayOnce = 1;
		}

		switch(GetCurrentError())
		{
			case 96:	MessageID = TheTextureManager::Instance()->GetTextureID("RGE_HOPARM.png");  
						break;
			case 97:	MessageID = TheTextureManager::Instance()->GetTextureID("RGE_HOPOPT.png"); 
						break;
			case 98:	MessageID = TheTextureManager::Instance()->GetTextureID("RGE_HOP10P.png"); 
						break;
			case 99:	MessageID = TheTextureManager::Instance()->GetTextureID("RGE_HOPEMP.png"); 
						break;
			case 101:	MessageID = TheTextureManager::Instance()->GetTextureID("RGE_ERREDC.png"); 
						break;
			default:	MessageID = TheTextureManager::Instance()->GetTextureID("RGErrGen.png");	  
						break;
		}

		TheObjectHandler::Instance()->GetObject2D("RGErrorMessage")->SetTextureID(MessageID);
		TheObjectHandler::Instance()->GetObject2D("RGErrorMessage")->SetVisible(true);
		if (MessageID == TheTextureManager::Instance()->GetTextureID("RGErrGen.png"))
		{
			ThePokerGame::Instance()->UpdateErrorDigits(true);
		}
	}
	else												//ACTUAL WARNING OR MESSAGES
	{
		if(PlayOnce)
		{
			TheAudioManager::Instance()->GetAudioSample("ALARM")->Stop();
			PlayOnce = 0;
		}

		TheObjectHandler::Instance()->GetObject2D("RGErrMessage")->SetVisible(false);
		ThePokerGame::Instance()->UpdateErrorDigits(false);
		
	}
}