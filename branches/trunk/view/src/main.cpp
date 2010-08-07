#include "windows.h"

#include "utilities.h"
#include "../include/debugging.h"

//#include "../include/registry.h"
//#include "displayer/display.h"





BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, LPVOID lpReserved)
{
	#if(DEBUG_GENERAL_LEVEL >= 2)
		prt("Inside DllMain");
	#endif

    return TRUE;
}












