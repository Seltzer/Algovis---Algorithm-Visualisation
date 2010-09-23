#include "userFunctions.h"

#include "utilities.h"
#include "registry.h"
#include "idManager.h"


bool communicationWithViewEnabled = false;
bool highlightOperandsEnabled = true;

void EnableCommunicationWithView(bool enabled)
{
	communicationWithViewEnabled = enabled;
}

void EnableOperandsHighlighting(bool enabled)
{
	highlightOperandsEnabled = enabled;
}

void PlaceNextOnSameLineAsLast()
{
	Algovis_Viewer::Registry::GetInstance()->PlaceNextOnSameLineAsLast();
}

void PlaceNextTwoOnSameLine()
{
	Algovis_Viewer::Registry::GetInstance()->PlaceNextTwoOnSameLine();
}

void EnsureNextIsDisplayed()
{
	Algovis_Viewer::Registry::GetInstance()->EnsureNextIsDisplayed();
}

void SetCaption(std::string& newCaption)
{
	Algovis_Viewer::Registry::GetInstance()->SetCaption(newCaption);
}

void EnsureThisIsDisplayed(Algovis::Wrapper* wrapper)
{
	Algovis::ID id = Algovis::IdManager::GetInstance()->GetId(wrapper);
	Algovis_Viewer::Registry::GetInstance()->DisplayThis(id);
}

void FlushAllActions()
{
	if (communicationWithViewEnabled)
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

