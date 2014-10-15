#include "Game.h"
#include <ObjectHandler.h>
#include <AudioManager.h>
#include "Defines.h"
#include "PokerGame.h"

void Game::UpdateDigits()
{
#ifdef FAST_PLAY
	THE_GAME->SetDeduct(0);
#endif
	char buff[32];
	
	float credits = (float)GetCredits();
	credits /= 100.0f;
	sprintf_s(buff, "£%.02f", credits);
		
	OBJECT_HANDLER->GetText("CREDIT_TEXT")->SetMessage(buff);

	float bank = (float)GetBankDeposit() - THE_GAME->GetDeduct();
	bank /= 100.0f;
	sprintf_s(buff, "£%.02f", bank);

	OBJECT_HANDLER->GetText("BANK_TEXT")->SetMessage(buff);

	
	float winnings = (float)POKER_GAME->FinalWinValue;
	winnings /= 100.0f;
	sprintf_s(buff, "£%.02f", winnings);

	OBJECT_HANDLER->GetText("WIN_TEXT")->SetMessage(buff);
			
}