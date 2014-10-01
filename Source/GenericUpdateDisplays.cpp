#include "Game.h"
#include <ObjectHandler.h>
#include <AudioManager.h>
#include "Defines.h"
#include "PokerGame.h"

void Game::UpdateDigits()
{
#ifdef FAST_PLAY
	TheGame::Instance()->SetDeduct(0);
#endif
	char buff[32];
	
	float credits = (float)GetCredits();
	credits /= 100.0f;
	sprintf_s(buff, "£%.02f", credits);
		
	TheObjectHandler::Instance()->GetText("CREDIT_TEXT")->SetMessage(buff);

	float bank = (float)GetBankDeposit() - TheGame::Instance()->GetDeduct();
	bank /= 100.0f;
	sprintf_s(buff, "£%.02f", bank);

	TheObjectHandler::Instance()->GetText("BANK_TEXT")->SetMessage(buff);

	
	float winnings = (float)ThePokerGame::Instance()->FinalWinValue;
	winnings /= 100.0f;
	sprintf_s(buff, "£%.02f", winnings);

	TheObjectHandler::Instance()->GetText("WIN_TEXT")->SetMessage(buff);
			
}