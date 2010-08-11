#include "windows.h"

#include "../include/debugging.h"




BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, LPVOID lpReserved)
{
	#if(DEBUG_GENERAL_LEVEL >= 2)
		prt("Inside DllMain");
	#endif

    return TRUE;
}












