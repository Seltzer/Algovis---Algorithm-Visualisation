#include "boost/foreach.hpp"
#include "utilities.h"

#include "../include/registry.h"
#include "world.h"
#include "viewableObjects/viewableObject.h"
#include "viewableObjects/vo_array.h"
#include "viewableObjects/vo_singlePrintable.h"

using namespace std;




namespace Algovis_Viewer
{

	
Registry* Registry::instance(NULL);


///////////////////////// Private methods

Registry::Registry()
{ 
	#if (DEBUG_GENERAL_LEVEL >= 2)
		prt("REGISTRY CTOR");
	#endif

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
	#if (DEBUG_REGISTRATION_LEVEL >= 2)
		std::cout << "Registered array @ " << dsArrayAddress << std::endl;
	#endif


	VO_Array* newArray = currentWorld->RegisterArray(dsArrayAddress, elementType, elements);

	Displayer::GetInstance()->AddToDrawingList(newArray);
}


void Registry::RegisterSinglePrintable(const void* dsSinglePrintableAddress, const std::string& value)
{
	VO_SinglePrintable* newSP = currentWorld->RegisterSinglePrintable(dsSinglePrintableAddress, value);
	
	#if (DEBUG_REGISTRATION_LEVEL >= 3)
		std::cout << "Registered SP with value " << value << " @ " << dsSinglePrintableAddress	<< std::endl;
	#endif

	// For now, arrays draw their child elements, and elements outside arrays don't get drawn
	// TODO: Remove this hack
	//Displayer::GetInstance()->AddToDrawingList(newSP);
}


bool Registry::DeregisterObject(const void* dsAddress)
{
	currentWorld->AcquireWriterLock();
	#if (DEBUG_REGISTRATION_LEVEL >= 1)
		cout << "Deregistering " << dsAddress << endl;
	#endif

	ViewableObject* voToBeDeleted = currentWorld->GetRepresentation(dsAddress);
	
	if (!voToBeDeleted)
	{
		currentWorld->ReleaseWriterLock();
		return false;
	}
		
	Displayer::GetInstance()->RemoveFromDrawingList(voToBeDeleted);
	bool result = currentWorld->DeregisterObject(dsAddress);
	currentWorld->ReleaseWriterLock();

	return result;
}

void Registry::AddElementToArray(const void* dsArray, void* dsElement, unsigned position)
{
	currentWorld->AddElementToArray(dsArray, dsElement, position);
}

void Registry::SwapElementsInArray(const void* dsArray, unsigned firstElementIndex, unsigned secondElementIndex)
{
	currentWorld->SwapElementsInArray(dsArray, firstElementIndex, secondElementIndex);
}

void Registry::ArrayResized(const void* dsArray, const std::vector<void*>& elements, unsigned newCapacity)
{
	#ifdef DEBUG_ARRAY_CHANGES
		prt("Vector resize registered");
	#endif
	currentWorld->ArrayResized(dsArray, elements, newCapacity);
}

void Registry::ClearArray(const void* dsArray)
{
	currentWorld->ClearArray(dsArray);
}

/*void Registry::UpdateSinglePrintable(const void* dsSinglePrintableAddress, const std::string& newValue)
{
	// acquire lock
	currentWorld->UpdateSinglePrintable(dsSinglePrintableAddress, newValue);
}*/

void Registry::PrintableAssigned(const void* dsAssigned, const void* dsSource, const std::string& newValue)
{
	UL_ASSERT(IsRegistered(dsAssigned, SINGLE_PRINTABLE));
	VO_SinglePrintable* sp = currentWorld->GetRepresentation<VO_SinglePrintable>(dsAssigned);
	UL_ASSERT(sp);

	if (IsRegistered(dsSource, SINGLE_PRINTABLE))
	{
		VO_SinglePrintable* source = currentWorld->GetRepresentation<VO_SinglePrintable>(dsSource);
		UL_ASSERT(source);

		DS_Assigned action(currentWorld, sp, source->GetHistory(), newValue);
		 // TODO: Better way of determining if we care about action
		if (sp->GetParent() != NULL)
		{
			//currentWorld->PerformDSAction(&action);
			action.Complete();

		}
		else
		{
			currentWorld->AcquireWriterLock();
			action.Complete(); // Just do it without flair and drama
			currentWorld->ReleaseWriterLock();
		}
	}
	else
	{
		currentWorld->AcquireWriterLock();
		sp->AssignedUntracked(dsSource, newValue);
		currentWorld->ReleaseWriterLock();
	}
		

	
}

// TODO: This is really similar to above
void Registry::PrintableModified(const void* dsModified, const void* dsSource, const std::string& newValue)
{
	currentWorld->AcquireWriterLock();

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

	currentWorld->ReleaseWriterLock();
}


void Registry::TestMethod()
{
	// TODO: Finish working on DS_TestAction
	DS_TestAction testAction(currentWorld);
	currentWorld->PerformDSAction(&testAction); 
}



}


