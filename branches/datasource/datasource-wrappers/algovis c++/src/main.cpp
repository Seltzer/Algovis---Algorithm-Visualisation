#include "userFunctions.h"

#include "utilities.h"
#include "registry.h"
#include "idManager.h"


bool drawingEnabled = false;

void EnableDrawing(bool enabled)
{
	drawingEnabled = enabled;
}

void Shutdown()
{
	Algovis_Viewer::Registry::DestroyInstance();
	Algovis::IdManager::DestroyInstance();
}

void TestMethod()
{
	Algovis_Viewer::Registry::GetInstance()->TestMethod();
}

