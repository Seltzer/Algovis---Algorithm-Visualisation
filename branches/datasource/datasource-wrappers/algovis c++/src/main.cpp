#include "userFunctions.h"

#include "utilities.h"
#include "registry.h"


bool drawingEnabled = false;

void EnableDrawing(bool enabled)
{
	drawingEnabled = enabled;
}



void TestMethod()
{
	Algovis_Viewer::Registry::GetInstance()->TestMethod();
}

