
#ifdef _AUTOPLAY
//#include "vld.h"	//Visual Leak Detector header
#endif

#include "Game.h"

bool InviteRemoteDebugger(void)
{
	 bool debuggerAttached = false ;
	 // File “waitforremotedebugger” must exist in same folder as game.
	 // exe in order for the game to wait for remote debugger connection
	 while (debuggerAttached == false)
	 {
		 if (::IsDebuggerPresent()==TRUE) // From winbase.h
		 {
			 debuggerAttached = true;
			 break;
		 }
		 ::Sleep(1000);
	  }
	  return debuggerAttached;
}

int WINAPI WinMain(	HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow)
{	
	/*while(!InviteRemoteDebugger())
	{
		Sleep(2);
	}*/

	if(TheGame::Instance()->Setup(hinstance))
	{
		TheEngine::Instance()->Run();
	}
	
	TheGame::Instance()->GameShutDown();
	return 0;
}