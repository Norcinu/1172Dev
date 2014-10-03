#include <ObjectHandler.h>
#include <TextureManager.h>
#include <RenderSequenceManager.h>
#include "ErrorProcess.h"
#include "Defines.h"
#include "Game.h"
#include "GenericMD5Verification.h"
#include "ShaderManager.h"
#include "UpdateProcess.h"
#include "CheckForDemoPlayProcess.h"

extern unsigned char global_quit;
unsigned int IntError = 0;

#define VIZTECHVERSION 0x46

bool Game::Setup(HINSTANCE hinstance)
{
	
	IntError = SetEnvironment(RELEASEx);

	if(IntError == 0x01)
	{
		MessageBox(NULL, "Shell Out Of Date", "Error", MB_OK);
		return false;
	}

	if(IntError == 0x02)
	{
		MessageBox(NULL, "BO Lib Incorrect", "Error", MB_OK);
		return false;
	}

	if(!ENGINE->Initialize("Config.pdt", GetHardWareType(), hinstance, VIZTECHVERSION, 1))
	{
		return false;
	}

	ENGINE->SetWriteablePath("D:\\machine\\game_data\\");

	THE_BUTTONS->LampsOff();

	if(GetDoorStatus() 
		|| GetCurrentError() 
		|| GetBankDeposit() && GetBankDeposit() >= GetVariableValue(MAX_WIN_BANK)
		|| GetCredits() && GetCredits() >= GetVariableValue(MAX_CREDITS))
	{
		Sleep(100);			
		DisableNoteValidator();
		Sleep(100);
	}
	else
	{
		Sleep(100);			
		EnableNoteValidator();
		Sleep(100);
	}

	ENGINE->SetMD5CheckCodes(FILE_NAMES, MDAT_CHECKSUMS, NUMBER_OF_CHECKSUMS);

	if(!ENGINE->LoadData("Logo.png", "DealStart", LOAD_LONG))
	{
		return false;
	}
	
	TheRenderSequenceManager::Instance()->SetClearColour(D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),0);

	RestoreData();

	if(!GameInit())
	{
		global_quit = true;
		return false;
	}

	IntError = 0;
	SendHeaderOnly((GAME_ONE_START_CODE+GetGameNumber(MODEL_NUMBER)), 1);

	if (!GetDoorStatus())
		SendCredit2Server(GetCredits());

	ENGINE->GetProcessManager()->AddProcessToList(new ErrorProcess);
	ENGINE->GetProcessManager()->AddProcessToList(new UpdateProcess);
	ENGINE->GetProcessManager()->AddProcessToList(new CheckForDemoPlayProcess);
	ENGINE->EmptyMessageQueue();
	
	return true;
}