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

	ThePokerGame::Instance()->RegulateGraphicalLampFlashSpeed(20);
	ThePokerGame::Instance()->TransferBankToCredits(GetVariableValue(MAX_WBANK_TRANSFER));
	ThePokerGame::Instance()->UpdateAwardDigits();
	ThePokerGame::Instance()->DrawRemoveHelds();
	ThePokerGame::Instance()->DrawHiloCards();
	ThePokerGame::Instance()->DrawHiloState();
	THE_BUTTONS->UpdateButtons();	
}