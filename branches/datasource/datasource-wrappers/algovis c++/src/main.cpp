#include "userFunctions.h"

#include "utilities.h"
#include "registry.h"
#include "idManager.h"


bool drawingEnabled = false;
bool highlightOperandsEnabled = true;

void EnableDrawing(bool enabled)
{
	drawingEnabled = enabled;
}

void EnableOperandsHighlighting(bool enabled)
{
	highlightOperandsEnabled = enabled;
}

void SetCaption(std::string& newCaption)
{
	Algovis_Viewer::Registry::GetInstance()->SetCaption(newCaption);
}


void FlushAllActions()
{
	Algovis_Viewer::Registry::GetInstance()->FlushAllActions();
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

