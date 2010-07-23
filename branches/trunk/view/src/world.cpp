#define BOOST_ALL_NO_LIB
#include "boost/foreach.hpp"
#include "utilities.h"
#include "world.h"
#include "viewableObjects/viewableObject.h"
#include "viewableObjects/vo_array.h"
#include "viewableObjects/vo_singlePrintable.h"


using namespace std;


namespace Algovis_Viewer
{



World::~World()
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



bool World::PerformDSAction(Action* dsAction)
{
	UL_ASSERT(dsAction);
	UL_ASSERT(!voUpdatePending)
	UL_ASSERT(lastActionPerformed == actionsPerformed.size() - 1);
	
	voUpdatePending = true;
	actionsPerformed.push_back(dsAction);
	

	Displayer::GetInstance()->PerformAndAnimateActionAsync(dsAction);
	

	return true;
}

void World::CompletedDSAction()
{
	voUpdatePending = false;
	++lastActionPerformed;
	voUpdatePendingCondVar.notify_all();
}

void World::PerformUIAction(UI_ActionType actionType)
{
	// TODO update so that this asks the Displayer to perform/animate the action
	UI_Action* newAction = new UI_Action(actionType, this);
	actionsPerformed.push_back(newAction);
	
	++lastActionPerformed;
}


bool World::IsRegistered(const void* dsAddress) const
{
	return ( (registeredArrays.count(dsAddress) == 1) || 
				(registeredSinglePrintables.count(dsAddress) == 1) );
}

bool World::IsRegistered(const void* dsAddress, ViewableObjectType voType) const
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

ViewableObject* World::GetRepresentation(const void* dsAddress)
{
	if (registeredArrays.count(dsAddress))
		return (ViewableObject*) registeredArrays[dsAddress];

	if (registeredSinglePrintables.count(dsAddress))
		return (ViewableObject*) registeredSinglePrintables[dsAddress];

	return NULL;
}


VO_Array* World::RegisterArray
		(const void* dsArrayAddress, ViewableObjectType elementType, const std::vector<void*>& elements)
{
	AcquireExclusiveLock();

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
	VO_Array* newArray = new VO_Array(dsArrayAddress, elementType, arrayElements);
	
	registeredArrays[dsArrayAddress] = newArray;

	ReleaseExclusiveLock();
	return newArray;
}


vector<VO_Array*> World::GetRegisteredArrays()
{
	AcquireReaderLock();

	vector<VO_Array*> returnVector;


	map<const void*,VO_Array*>::iterator it;
	for (it = registeredArrays.begin(); it != registeredArrays.end(); it++)
	{
		returnVector.push_back((*it).second);
	}
	
	ReleaseReaderLock();
	return returnVector;
}


VO_SinglePrintable* World::RegisterSinglePrintable
			(const void* dsSinglePrintableAddress, const std::string& value)
{
	AcquireExclusiveLock();

	// Verify that SINGLE_PRINTABLE hasn't already been registered
	UL_ASSERT(!IsRegistered(dsSinglePrintableAddress));

	VO_SinglePrintable* newSP = new VO_SinglePrintable(dsSinglePrintableAddress, value);
	registeredSinglePrintables[dsSinglePrintableAddress] = newSP;

	ReleaseExclusiveLock();
	return newSP;
}


bool World::DeregisterObject(const void* dsAddress)
{
	AcquireExclusiveLock();

	if (registeredArrays.count(dsAddress))
	{
		VO_Array* voArray = registeredArrays[dsAddress];
		delete voArray;

		registeredArrays.erase(registeredArrays.find(dsAddress));

		ReleaseExclusiveLock();
		return true;
	}

	if (registeredSinglePrintables.count(dsAddress))
	{
		VO_SinglePrintable* voSP = registeredSinglePrintables[dsAddress];
		delete voSP;

		registeredSinglePrintables.erase(registeredSinglePrintables.find(dsAddress));

		ReleaseExclusiveLock();
		return true;
	}

	ReleaseExclusiveLock();
	return false;
}




void World::AddElementToArray(const void* dsArray, void* dsElement, unsigned position)
{
	AcquireExclusiveLock();

	UL_ASSERT(IsRegistered(dsArray,ARRAY));
	UL_ASSERT(IsRegistered(dsElement,SINGLE_PRINTABLE));

	VO_Array* arrayAddress = GetRepresentation<VO_Array>(dsArray);
	UL_ASSERT(position <= arrayAddress->GetSize());

	VO_SinglePrintable* element = GetRepresentation<VO_SinglePrintable>(dsElement);

	arrayAddress->AddElement((ViewableObject*)element, position);
	ReleaseExclusiveLock();
}

void World::SwapElementsInArray(const void* dsArray, unsigned firstElementIndex, unsigned secondElementIndex)
{
	AcquireExclusiveLock();

	UL_ASSERT(IsRegistered(dsArray, ARRAY));

	VO_Array* arrayAddress = GetRepresentation<VO_Array>(dsArray);
	UL_ASSERT(arrayAddress);

	arrayAddress->SwapElements(firstElementIndex, secondElementIndex);

	ReleaseExclusiveLock();
}


void World::UpdateSinglePrintable(const void* dsSinglePrintableAddress, const std::string& newValue)
{
	AcquireExclusiveLock();

	UL_ASSERT(IsRegistered(dsSinglePrintableAddress, SINGLE_PRINTABLE));

	VO_SinglePrintable* sp = GetRepresentation<VO_SinglePrintable>(dsSinglePrintableAddress);
	UL_ASSERT(sp);

	sp->UpdateValue(newValue);

	ReleaseExclusiveLock();
}


void World::AcquireReaderLock()
{
	voAccessMutex.lock_shared();

	while (voUpdatePending)
	{
		voUpdatePendingCondVar.wait<boost::shared_mutex>(voAccessMutex);
	}
}

void World::ReleaseReaderLock()
{
	voAccessMutex.unlock_shared();
}

void World::AcquireExclusiveLock()
{
	voAccessMutex.lock_upgrade();
	voAccessMutex.unlock_upgrade_and_lock();
}

void World::ReleaseExclusiveLock()
{
	voAccessMutex.unlock();
}



}