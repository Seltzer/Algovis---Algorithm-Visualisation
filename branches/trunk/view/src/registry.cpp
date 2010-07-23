#include "boost/foreach.hpp"
#include "utilities.h"

#include "../include/registry.h"
#include "world.h"
#include "viewableObjects/viewableObject.h"
#include "viewableObjects/vo_array.h"
#include "viewableObjects/vo_singlePrintable.h"

//temp - TODO: remove
#include "displayer/uiAction.h"

using namespace std;




namespace Algovis_Viewer
{

	
Registry* Registry::instance(NULL);


///////////////////////// Private methods

Registry::Registry()
{ 
	currentWorld = new World();
	Displayer::GetInstance()->SetWorld(currentWorld);
}

Registry::~Registry()
{
	delete currentWorld;	
}



///////////////////////// Public dll export methods




Registry* Registry::GetInstance() 
{
	if (!Registry::instance)
		Registry::instance = new Registry();

	return Registry::instance;
}

void Registry::DestroyInstance()
{
	if (Registry::instance)
	{
		delete Registry::instance;
		Registry::instance = NULL;
	}
}


bool Registry::IsRegistered(const void* dsAddress) const
{
	return currentWorld->IsRegistered(dsAddress);
}

bool Registry::IsRegistered(const void* dsAddress, ViewableObjectType voType) const
{
	return currentWorld->IsRegistered(dsAddress, voType);
}


void Registry::RegisterArray
		(const void* dsArrayAddress, ViewableObjectType elementType, const std::vector<void*>& elements)
{
	VO_Array* newArray = currentWorld->RegisterArray(dsArrayAddress, elementType, elements);

	Displayer::GetInstance()->AddToDrawingList(newArray);
}



void Registry::RegisterSinglePrintable(const void* dsSinglePrintableAddress, const std::string& value)
{
	VO_SinglePrintable* newSP = currentWorld->RegisterSinglePrintable(dsSinglePrintableAddress, value);
	
	#ifdef DEBUG_VERBOSE
		prt("registered")
	#endif

	// For now, arrays draw their child elements, and elements outside arrays don't get drawn
	// TODO: Remove this hack
	//Displayer::GetInstance()->AddToDrawingList(newSP);
}


bool Registry::DeregisterObject(const void* dsAddress)
{
	ViewableObject* voToBeDeleted = currentWorld->GetRepresentation(dsAddress);
	UL_ASSERT(voToBeDeleted);

	Displayer::GetInstance()->RemoveFromDrawingList(voToBeDeleted);

	return currentWorld->DeregisterObject(dsAddress);
}

void Registry::AddElementToArray(const void* dsArray, void* dsElement, unsigned position)
{
	currentWorld->AddElementToArray(dsArray, dsElement, position);
}

void Registry::SwapElementsInArray(const void* dsArray, unsigned firstElementIndex, unsigned secondElementIndex)
{
	currentWorld->SwapElementsInArray(dsArray, firstElementIndex, secondElementIndex);
}


/*void Registry::UpdateSinglePrintable(const void* dsSinglePrintableAddress, const std::string& newValue)
{
	// acquire lock
	currentWorld->UpdateSinglePrintable(dsSinglePrintableAddress, newValue);
}*/

void Registry::PrintableAssigned(const void* dsAssigned, const void* dsSource, const std::string& newValue)
{
	currentWorld->AcquireExclusiveLock();

	UL_ASSERT(IsRegistered(dsAssigned, SINGLE_PRINTABLE));

	VO_SinglePrintable* sp = currentWorld->GetRepresentation<VO_SinglePrintable>(dsAssigned);
	UL_ASSERT(sp);

	if (IsRegistered(dsSource, SINGLE_PRINTABLE))
	{
		VO_SinglePrintable* source = currentWorld->GetRepresentation<VO_SinglePrintable>(dsSource);
		UL_ASSERT(source);
		sp->Assigned(source, newValue);
	}
	else
		sp->AssignedUntracked(dsSource, newValue);

	currentWorld->ReleaseExclusiveLock();
}

// TODO: This is really similar to above
void Registry::PrintableModified(const void* dsModified, const void* dsSource, const std::string& newValue)
{
	currentWorld->AcquireExclusiveLock();

	UL_ASSERT(IsRegistered(dsModified, SINGLE_PRINTABLE));

	VO_SinglePrintable* sp = currentWorld->GetRepresentation<VO_SinglePrintable>(dsModified);
	UL_ASSERT(sp);

	if (IsRegistered(dsSource, SINGLE_PRINTABLE))
	{
		VO_SinglePrintable* source = currentWorld->GetRepresentation<VO_SinglePrintable>(dsSource);
		UL_ASSERT(sp);
		sp->Modified(source, newValue);
	}
	else
		sp->ModifiedUntracked(dsSource, newValue);

	currentWorld->ReleaseExclusiveLock();
}


void Registry::TestMethod()
{
	UI_Action testAction(MoveVO, currentWorld);
	currentWorld->PerformDSAction((Action*)&testAction);
}



}


