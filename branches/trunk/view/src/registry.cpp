#include <QObject>
#include "qt/qapplication.h"
#include "boost/foreach.hpp"
#include "utilities.h"

#include "../include/registry.h"
#include "world.h"
#include "displayer/displayer.h"
#include "viewableObjects/viewableObject.h"
#include "viewableObjects/vo_array.h"
#include "viewableObjects/vo_singlePrintable.h"

#include "action.h"
#include "dsAction.h"

using namespace std;



namespace Algovis_Viewer
{


Registry* Registry::instance(NULL);


///////////////////////// Private methods

Registry::Registry()
	: actionBuffer(1)
{ 
	#if (DEBUG_GENERAL_LEVEL >= 2)
		prt("REGISTRY CTOR");
	#endif

	world = Displayer::GetInstance()->GetWorld();
	UL_ASSERT(world);
}

Registry::~Registry()
{
	typedef std::map<const void*,VO_Array*> ArrayMap;
	typedef std::map<const void*,VO_SinglePrintable*> SPMap;

	// Delete arrays first, as their destruction currently relies on their child elements 
	// being alive (since they deregister themselves from their elements as observers
	BOOST_FOREACH(ArrayMap::value_type arrayPair, registeredArrays)
		delete arrayPair.second;

	BOOST_FOREACH(SPMap::value_type spPair, registeredSinglePrintables)
		delete spPair.second;
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
	#if (DEBUG_ACTION_LEVEL >= 1)
		prt("Starting Registry::AddActionToBuffer()");
	#endif

	boost::unique_lock<boost::mutex> lock(bufferMutex);
	
	
	UL_ASSERT(dsAction);

/*
	#if (DEBUG_ACTION_LEVEL >= 2)
		prt("\tchecking if buffer is full")
	#endif

	while(actionBuffer.IsFull())
	{
		#if (DEBUG_ACTION_LEVEL >= 2)
			prt("\t\tbuffer is full");
		#endif
	
		// Attempt to combine actions
		//actionBuffer.LockBuffer();
		// *** Attempt ****
		// If several actions were combined, we have room in the buffer to add dsAction, so let's do it!
		// break 
		//actionBuffer.UnlockBuffer();
		
		
		// Pop front action and give it to the ActionAgent to perform (via the Displayer)
		//actionBuffer.LockBuffer();
		Action* original = actionBuffer.at(0);

		original->SuppressAnimation();
		
		original->Clone();
		original->Clone();
		original->Clone();
		original->Clone();

		Action* actionClone = original->Clone();
		Displayer::GetInstance()->PerformAndAnimateActionAsync(actionClone);
		//Displayer::GetInstance()->PerformAndAnimateActionAsync(actionBuffer.front()->Clone());
		// NB: popFront forcibly unlocks actionBuffer for us
		// TODO: delete action
		actionBuffer.PopFront();
	}

	// Buffer shouldn't be full by this point, so add dsAction to the back
	UL_ASSERT(!actionBuffer.IsFull())
	actionBuffer.PushBack(dsAction);*/

	Displayer::GetInstance()->PerformAndAnimateActionAsync(dsAction);

	
	#if (DEBUG_ACTION_LEVEL >= 2)
		prt("\tadded action to buffer");
		prt("\tFinishing Registry::AddActionToBuffer()");
	#endif
}


void Registry::RegisterArray
		(const void* dsArrayAddress, ViewableObjectType elementType, const std::vector<void*>& elements)
{
	#if (DEBUG_REGISTRATION_LEVEL >= 2)
		std::cout << "Registering array @ " << dsArrayAddress << std::endl;
	#endif


	// Verify that array hasn't already been registered
	UL_ASSERT(!IsRegistered(dsArrayAddress));


	// ViewableObject equivalents of elements
	vector<ViewableObject*> arrayElements;

	// Iterate over elements, verify that they are all registered and populate arrayElements
	BOOST_FOREACH(void* dsElement, elements)
	{
		// TODO: change behaviour when above registration condition is violated (i.e. throw exception)
		UL_ASSERT(IsRegistered(dsElement));
					
		arrayElements.push_back(GetRepresentation(dsElement));
	}

	// Assuming VO_SinglePrintable elements - TODO change this
	//VO_Array* newArray = new VO_Array(dsArrayAddress, this, elementType, arrayElements);
	
	DS_CreateArray creationAction(world, dsArrayAddress, elementType, arrayElements);

	waitingOnCallback = true;
	lastViewableCreatedOrDestroyed = NULL;
	AddActionToBuffer(&creationAction);

	while(waitingOnCallback);
	registeredArrays[dsArrayAddress] = (VO_Array*) lastViewableCreatedOrDestroyed;

}


void Registry::RegisterSinglePrintable(const void* dsSinglePrintableAddress, const std::string& value)
{
	#if (DEBUG_REGISTRATION_LEVEL >= 3)
		std::cout << "Registering SP with value " << value << " @ " << dsSinglePrintableAddress	<< std::endl;
	#endif

	// Verify that array hasn't already been registered
	UL_ASSERT(!IsRegistered(dsSinglePrintableAddress));

	// Create action
	DS_CreateSP creationAction(world, dsSinglePrintableAddress, value);

	// Wait on callback
	waitingOnCallback = true;
	lastViewableCreatedOrDestroyed = NULL;
	AddActionToBuffer(&creationAction);

	while(waitingOnCallback);
	registeredSinglePrintables[dsSinglePrintableAddress] = (VO_SinglePrintable*) lastViewableCreatedOrDestroyed;
}


bool Registry::DeregisterObject(const void* dsAddress)
{
	#if (DEBUG_REGISTRATION_LEVEL >= 1)
		cout << "Deregistering " << dsAddress << endl;
	#endif

	
	ViewableObject* voToBeDeleted = GetRepresentation(dsAddress);
	UL_ASSERT(voToBeDeleted);
	ViewableObjectType voType = voToBeDeleted->GetType();

	// Create event
	DS_Deleted* deleteAction = new DS_Deleted(world, voToBeDeleted);
		
	waitingOnCallback = true;
	lastViewableCreatedOrDestroyed = NULL;
	AddActionToBuffer(deleteAction);
	
	while(waitingOnCallback);

	if (voType == ARRAY)
		registeredArrays.erase(registeredArrays.find(dsAddress));
	else if (voType == SINGLE_PRINTABLE)
		registeredSinglePrintables.erase(registeredSinglePrintables.find(dsAddress));
	
	return true;
}

void Registry::AddElementToArray(const void* dsArray, void* dsElement, unsigned position)
{
	UL_ASSERT(IsRegistered(dsArray,ARRAY));
	UL_ASSERT(IsRegistered(dsElement,SINGLE_PRINTABLE));

	VO_Array* voArray = GetRepresentation<VO_Array>(dsArray);
	UL_ASSERT(position <= voArray->GetSize());

	VO_SinglePrintable* element = GetRepresentation<VO_SinglePrintable>(dsElement);

	// Create event
	DS_AddElementToArray* addAction = new DS_AddElementToArray(world, voArray, element, position);
	AddActionToBuffer(addAction);
}

void Registry::SwapElementsInArray(const void* dsArray, unsigned firstElementIndex, unsigned secondElementIndex)
{
	/*
	UL_ASSERT(>IsRegistered(dsArray, ARRAY));

	VO_Array* arrayAddress = GetRepresentation<VO_Array>(dsArray);
	UL_ASSERT(arrayAddress);

	arrayAddress->SwapElements(firstElementIndex, secondElementIndex);*/
}

void Registry::ArrayResized(const void* dsArray, const std::vector<void*>& elements, unsigned newCapacity)
{
	#ifdef DEBUG_ARRAY_CHANGES
		prt("Registering array resize");
	#endif

	//boost::unique_lock<boost::mutex> lock(registryMutex);

	world->ArrayResized(dsArray, elements, newCapacity);
}

void Registry::ClearArray(const void* dsArray)
{
	#ifdef DEBUG_ARRAY_CHANGES
		prt("Registry::ClearArray");
	#endif

	//boost::unique_lock<boost::mutex> lock(registryMutex);

	world->ClearArray(dsArray);
}

void Registry::PrintableAssigned(const void* dsAssigned, const void* dsSource, const std::string& newValue)
{
	#ifdef DEBUG_SP_CHANGES
		std::cout << "Registry::PrintableAssigned for " << dsAssigned << std::endl;
	#endif


	UL_ASSERT(IsRegistered(dsAssigned, SINGLE_PRINTABLE));
	VO_SinglePrintable* sp = GetRepresentation<VO_SinglePrintable>(dsAssigned);
	UL_ASSERT(sp);

	if (IsRegistered(dsSource, SINGLE_PRINTABLE))
	{
		VO_SinglePrintable* source = GetRepresentation<VO_SinglePrintable>(dsSource);
		UL_ASSERT(source);

		DS_Assigned action(world, sp, source->GetHistory(), newValue);
		
		
		// TODO: Currently we only want an animation for assignments to SPs which are owned by an array
		if (sp->parent())
		{
			if (typeid(*sp->parent()) != typeid(VO_Array))
				action.SuppressAnimation();
		}
		else
		{
			action.SuppressAnimation();
		}

		AddActionToBuffer(&action);
	}
	else
	{
		world->AcquireWriterLock();
		sp->AssignedUntracked(dsSource, newValue);
		world->ReleaseWriterLock();
	}
}

// TODO: This is really similar to above
void Registry::PrintableModified(const void* dsModified, const void* dsSource, const std::string& newValue)
{
	//world->AcquireWriterLock();

	UL_ASSERT(IsRegistered(dsModified, SINGLE_PRINTABLE));

	VO_SinglePrintable* sp = GetRepresentation<VO_SinglePrintable>(dsModified);
	UL_ASSERT(sp);

	if (IsRegistered(dsSource, SINGLE_PRINTABLE))
	{
		VO_SinglePrintable* source = GetRepresentation<VO_SinglePrintable>(dsSource);
		UL_ASSERT(sp);
		sp->Modified(source, newValue);
	}
	else
		sp->ModifiedUntracked(dsSource, newValue);

	//world->ReleaseWriterLock();
}

void Registry::ActionAgentCallback(ViewableObject* lastViewableCreatedOrDestroyed)
{
	this->lastViewableCreatedOrDestroyed = lastViewableCreatedOrDestroyed;
	waitingOnCallback = false;
}

void Registry::TestMethod()
{
	Displayer::GetInstance();
}




//////////// Registration verification methods
bool Registry::IsRegistered(const void* dsAddress) const
{
	return ( (registeredArrays.count(dsAddress)) || 
		(registeredSinglePrintables.count(dsAddress)) );
}

bool Registry::IsRegistered(const void* dsAddress, ViewableObjectType voType) const
{
	switch(voType)
	{
		case ARRAY:
			return (registeredArrays.count(dsAddress) > 0);
			break;

		case SINGLE_PRINTABLE:
			return (registeredSinglePrintables.count(dsAddress) > 0);
			break;
	}

	return false;	
}

ViewableObject* Registry::GetRepresentation(const void* dsAddress)
{
	if (registeredArrays.count(dsAddress) > 0)
		return (ViewableObject*) registeredArrays[dsAddress];

	if (registeredSinglePrintables.count(dsAddress) > 0)
		return (ViewableObject*) registeredSinglePrintables[dsAddress];

	return NULL;
}



}


