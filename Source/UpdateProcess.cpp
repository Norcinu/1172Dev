#include <ObjectHandler.h>
#include <sstream>
#include <bo.h>
#include "GenericButtons.h"
#include "UpdateProcess.h"
#include "PokerGame.h"


UpdateProcess::UpdateProcess()
{

}

void UpdateProcess::Update()
{
	PROFILE(__FUNCTION__);

	POKER_GAME->RegulateGraphicalLampFlashSpeed(20);
	POKER_GAME->TransferBankToCredits(GetVariableValue(MAX_WBANK_TRANSFER));
	POKER_GAME->UpdateAwardDigits();
	POKER_GAME->DrawRemoveHelds();
	POKER_GAME->DrawHiloCards();
	POKER_GAME->DrawHiloState();
	THE_BUTTONS->UpdateButtons();	
}