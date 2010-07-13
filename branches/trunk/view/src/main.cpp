#include "windows.h"

#include "../include/registry.h"
#include "displayer/display.h"

BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, LPVOID lpReserved)
{
	Algovis_Viewer::Displayer::GetInstance();

    return TRUE;

}












