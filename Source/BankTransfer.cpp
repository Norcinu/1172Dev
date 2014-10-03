#include "Game.h"
#include "PokerGame.h"
#include <ObjectHandler.h>


void PokerGame::TransferBankToCredits(unsigned int MaxTransferValue)
{
static unsigned int Unit=0;
static unsigned int TransferAmount=0;
static int Stage = 1;

	if (GetTransferBankToCredits())
	{
		if (!GetTransferBanktoCreditTimeElaspsed())
			return;

		if (Stage == 1)
		{
			if (GetBankDeposit() > MaxTransferValue)
				TransferAmount = MaxTransferValue;
			else
				TransferAmount = GetBankDeposit();

			Unit = TransferAmount/10;
			Stage = 2;
		}
		else
		{	
			if (TransferAmount)
			{
				TheAudioManager::Instance()->GetAudioSample("WIN10")->Play();
				if (TransferAmount < Unit)
					Unit = TransferAmount;
				SubBankDeposit(Unit);
				AddToCdepositFromBank(Unit);
				SetCreditsPrevious(GetCredits());
				TransferAmount -= Unit;
				SetTransferBankToCreditDelay(0.1f);
				THE_GAME->UpdateDigits();
			}
			else
			{
				Unit=0;
				TransferAmount=0;
				Stage = 1;
				SetTransferBankToCredits(false);
			}
		}
	}
}

void PokerGame::SetTransferBankToCredits(bool Status)
{	
	TransferFlag = Status;	
}

bool PokerGame::GetTransferBankToCredits(void)
{	
	return(TransferFlag);	
}

void PokerGame::SetTransferBankToCreditDelay(float DelayTime)
{
	TransferDelay = ENGINE->GetSystemTimer().GetRunningTime() + GeneralSetGameDelay(DelayTime);
}

bool PokerGame::GetTransferBanktoCreditTimeElaspsed(void)
{
	if (ENGINE->GetSystemTimer().GetRunningTime() < TransferDelay)
		return(false);
	else
		return(true);
}