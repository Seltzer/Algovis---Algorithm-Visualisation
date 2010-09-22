#include <iostream>
#include "qt/qapplication.h"
#include "boost/foreach.hpp"
#include "utilities.h"

#include "../include/registry.h"
#include "displayer/displayer.h"
#include "viewableObjects/viewableObject.h"
#include "actions/action.h"
#include "actions/dsArrayActions.h"
#include "actions/dsPrintableActions.h"
#include "actions/dsMatrixActions.h"

using namespace std;



namespace Algovis_Viewer
{


Registry* Registry::instance(NULL);


///////////////////////// Private methods

Registry::Registry()
	: world(NULL), displayerShuttingDown(false), actionBuffer(3), ensureNextIsDisplayed(false)
{ 
	#if (DEBUG_GENERAL_LEVEL >= 2)
		prt("REGISTRY CTOR");
	#endif

	world = Displayer::GetInstance()->GetWorld();
	UL_ASSERT(world);
}

Registry::~Registry()
{
	#if (DEBUG_GENERAL_LEVEL >= 2)
		prt("REGISTRY DTOR");
	#endif

	displayerShuttingDown = true;
	Displayer::DestroyInstance();
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


void Registry::AddActionToBuffer(DS_Action* dsAction)
{
	if (displayerShuttingDown)
		return;

	#if (DEBUG_ACTION_LEVEL >= 1)
		prt("\tStarting Registry::AddActionToBuffer()");
	#endif
	
	UL_ASSERT(dsAction);
	actionBuffer.PushBack(dsAction);

	#if (DEBUG_ACTION_LEVEL >= 1)
		prt("\tFinishing Registry::AddActionToBuffer()");
	#endif
}


void Registry::RegisterArray
		(ID id, const void* dsArrayAddress, ViewableObjectType elementType, const vector<ID>& elements)
{
	boost::unique_lock<boost::mutex> lock(registryMutex);
	#if (DEBUG_REGISTRATION_LEVEL >= 2)
		cout << "Registering array with ID " << id << std::endl;
	#endif

	
	DS_CreateArray creationAction(world, id, dsArrayAddress, elementType, elements);

	if (!createViewablesOnSameLine.empty())
	{	
		if (createViewablesOnSameLine.front())
			creationAction.PlaceOnSameLine();
		createViewablesOnSameLine.pop_front();
	}

	AddActionToBuffer(&creationAction);
}

void Registry::RegisterMatrix(ID id, const void* dsMatrixAddress, ViewableObjectType elementType,
					unsigned rows, unsigned cols, const std::vector<ID>& initElements)
{
	boost::unique_lock<boost::mutex> lock(registryMutex);
	#if (DEBUG_REGISTRATION_LEVEL >= 2)
		cout << "Registering matrix with ID " << id << std::endl;
	#endif

	DS_CreateMatrix creationAction(world, id, dsMatrixAddress, elementType, rows, cols, initElements);

	if (!createViewablesOnSameLine.empty())
	{
		if (createViewablesOnSameLine.front())
			creationAction.PlaceOnSameLine();

		createViewablesOnSameLine.pop_front();
	}

	AddActionToBuffer(&creationAction);
}



void Registry::RegisterSinglePrintable(ID id, const void* dsSinglePrintableAddress, const string& value)
{
	boost::unique_lock<boost::mutex> lock(registryMutex);
	#if (DEBUG_REGISTRATION_LEVEL >= 3)
		cout << "Registering SP with value " << value << " and id " << id<< std::endl;
	#endif

	// Create action
	DS_CreateSP creationAction(world, id, dsSinglePrintableAddress, value);

	if (ensureNextIsDisplayed)
	{
		ensureNextIsDisplayed = false;
		creationAction.CreateAndDisplayASAP();

		if (!createViewablesOnSameLine.empty())
		{
			if (createViewablesOnSameLine.front())
				creationAction.PlaceOnSameLine();
			createViewablesOnSameLine.pop_front();
		}
	}
	else
	{
		creationAction.SuppressAnimation();
	}


	AddActionToBuffer(&creationAction);

}

void Registry::AddressChanged(ID id, const void* newAddress)
{
	boost::unique_lock<boost::mutex> lock(registryMutex);

	DS_AddressChanged action(world,id,newAddress);

	AddActionToBuffer(&action);
}


bool Registry::DeregisterObject(ID id, bool suppressAnimation)
{
	boost::unique_lock<boost::mutex> lock(registryMutex);
	#if (DEBUG_REGISTRATION_LEVEL >= 1)
		cout << "Deregistering " << id << endl;
	#endif

	// Create event
	DS_Deleted* deleteAction = new DS_Deleted(world, id);

	
	if (suppressAnimation)
	{
		// TODO We want to perform deleteAction but not animate it
		// But suppressing it will ensure that it's never performed. Oh noes!
		deleteAction->SuppressAnimation();
	}

	AddActionToBuffer(deleteAction);
	

	return true;
}

void Registry::AddElementToArray(ID dsArray, ID dsElement, unsigned position)
{
	#ifdef DEBUG_ARRAY_CHANGES
		cout << "Adding element with ID " << dsElement << " to array with ID " << dsArray << endl;
	#endif

	boost::unique_lock<boost::mutex> lock(registryMutex);


	// Create event
	DS_AddElementToArray* addAction = new DS_AddElementToArray(world, dsArray, dsElement, position);
	//addAction->SuppressAnimation(); // This has an animation now
	
	AddActionToBuffer(addAction);
}


void Registry::AddElementsToArray(ID dsArray, const std::vector<ID>& elements, unsigned startIndex)
{
	#ifdef DEBUG_ARRAY_CHANGES
		cout << "Adding " << elements.size() << " elements to array with ID " << dsArray << endl;
	#endif

	// commented out to prevent recursive acquisition when we call AddElementToArray
	//boost::unique_lock<boost::mutex> lock(registryMutex);

	// For now, just call AddElementsToArray elements.size() times
	
	// Later on, we could possibly make this a CompositeAction containing AddElementToArray actions
	// which all are animated at the same time???
	
	DS_CompositeAction action(world);
	
	for (int i = 0; i < elements.size(); i++)
	{
		DS_AddElementToArray* subaction = new DS_AddElementToArray(world, dsArray, elements[i], startIndex + i);
		action.AddAction(subaction);
	}

	AddActionToBuffer(&action);

		
	//unsigned index = startIndex;

	//BOOST_FOREACH(ID element, elements)
	//	AddElementToArray(dsArray, element, index++);
}

void Registry::RemoveElementsFromArray(ID dsArray, const vector<ID>& elements, 
											unsigned startIndex, unsigned endIndex)
{
	DS_RemoveElementsFromArray* removeAction = 
				new DS_RemoveElementsFromArray(world, dsArray, elements, startIndex, endIndex);

	AddActionToBuffer(removeAction);
}

/*
void Registry::ClearArray(const void* dsArray)
{
	boost::unique_lock<boost::mutex> lock(registryMutex);
	prt("UNIMPLEMENTED!!!!!!!!!!!!!!!!!!!!!!!");
}*/

void Registry::PrintableAssigned(ID dsAssigned, ID dsSource, const string& newValue)
{
	boost::unique_lock<boost::mutex> lock(registryMutex);
 
	#ifdef DEBUG_SP_CHANGES
		std::cout << "Registry::PrintableAssigned for " << dsAssigned << std::endl;
	#endif

	//if (dsSource == INVALID)
	//	cout << "Warning! dsSource for PrintableAssigned is = " << INVALID << endl;

	DS_Assigned action(world, dsAssigned, dsSource, newValue, true);
	AddActionToBuffer(&action);
}

// TODO: This is really similar to above
void Registry::PrintableModified(ID dsModified, ID dsSource, const string& newValue)
{
	boost::unique_lock<boost::mutex> lock(registryMutex);
	// Don't expect this method to work atm

	// TODO: This will not include itself as a source in the animation. As far as I know thats the only issue with using an assigned action
	DS_Modified action(world, dsModified, dsSource, newValue, true);
	AddActionToBuffer(&action);
}

void Registry::HighlightOperands(const vector<ID>& operands, ComparisonOps opType)
{
	DS_HighlightOperands action(world, operands, opType);
	AddActionToBuffer(&action);
}

void Registry::SetCaption(const std::string& newCaption)
{
	DS_SetCaption captionAction(world, newCaption);
	AddActionToBuffer(&captionAction);
}

void Registry::PlaceNextOnSameLineAsLast()
{
	createViewablesOnSameLine.push_back(true);
//	createNextViewableOnSameLine = true;
}

void Registry::PlaceNextTwoOnSameLine()
{
	// If there isn't a layout policy for the next element, insert the default
	if (createViewablesOnSameLine.empty())
		createViewablesOnSameLine.push_back(false);

	// Add layout policy for next next
	createViewablesOnSameLine.push_back(true);
}

void Registry::EnsureNextIsDisplayed()
{
	ensureNextIsDisplayed = true;
}

void Registry::DisplayThis(ID id)
{
	// Don't trust the user
	if (IsRegistered(id))
		return;
	

	// Create action
	DS_EnsureDisplayed action(world, false, id);

	if (!createViewablesOnSameLine.empty())
	{
		if (createViewablesOnSameLine.front())
			action.PlaceOnSameLine();
		createViewablesOnSameLine.pop_front();
	}


	AddActionToBuffer(&action);

}


void Registry::FlushAllActions()
{
	if (displayerShuttingDown)
		return;

	actionBuffer.Flush();
}




//////////// Registration methods
bool Registry::IsRegistered(ID id) const
{
	ReaderLock<LockManager<1>,1> lock(*this);

	return registeredViewables.count(id) > 0;
}

bool Registry::IsRegistered(ID id, ViewableObjectType voType) const
{
	ReaderLock<LockManager<1>,1> lock(*this);

	if (registeredViewables.count(id) == 0)
		return false;

	map<ID, ViewableObject*>::const_iterator it = registeredViewables.find(id);
	
	return (*it).second->GetType() == voType;
}

ViewableObject* Registry::GetRepresentation(ID id)
{
	ReaderLock<LockManager<1>,1> lock(*this);

	if (!IsRegistered(id))
		return NULL;

	return registeredViewables[id];
}

void Registry::Register(ID id, ViewableObject* viewable)
{
	WriterLock<LockManager<1>,1> lock(*this);

	UL_ASSERT(viewable);
	UL_ASSERT(!IsRegistered(id));
	UL_ASSERT(id != INVALID);
	UL_ASSERT(id < INT_MAX);

	registeredViewables[id] = viewable;
}


bool Registry::Deregister(ID id)
{
	cout << "Registry::Deregister for ID" << id << endl;
	WriterLock<LockManager<1>,1> lock(*this);

	UL_ASSERT(IsRegistered(id));
	registeredViewables.erase(registeredViewables.find(id));

	return true;
}


void Registry::DisplayerIsShuttingDown()
{
	displayerShuttingDown = true;
	world = NULL;
}


void Registry::TestMethod()
{
	Displayer::GetInstance();
}







}


