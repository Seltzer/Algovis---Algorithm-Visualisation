#include "windows.h"
#include "boost/foreach.hpp"
#include "utilities.h"
#include "world.h"
#include "viewableObjects/viewableObject.h"
#include "viewableObjects/vo_array.h"
#include "viewableObjects/vo_singlePrintable.h"


using namespace std;


namespace Algovis_Viewer
{



World::World() 
	: lastActionPerformed(INVALID), voActionPending(false), writerLockOwner(INVALID)
{
}


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


// TODO: Verify that synchronisation is done properly
bool World::PerformDSAction(DS_Action* dsAction)
{
	#if (DEBUG_ACTION_LEVEL >= 1)
		prt("Starting World::PerformDSAction()");
	#endif
	UL_ASSERT(dsAction);
	
	// Wait for the current action to finish
	boost::unique_lock<boost::shared_mutex> lock(voAccessMutex);

	while(voActionPending)
		voActionPendingCondVar.wait<boost::shared_mutex>(voAccessMutex);
	
	// TODO: Handle this properly once we can go back through history
	UL_ASSERT(lastActionPerformed == actionsPerformed.size() - 1);
	
	voActionPending = true;
	#if (DEBUG_ACTION_LEVEL >= 1)
		prt("\tWorld::voActionPending set to true");
	#endif
	actionsPerformed.push_back(dsAction);

	// Ask Displayer to perform dsAction
	Displayer::GetInstance()->PerformAndAnimateActionAsync(dsAction);
	
	#if (DEBUG_ACTION_LEVEL >= 1)
		prt("Finishing World::PerformDSAction()");
	#endif

	return true;
}

void World::CompletedDSAction()
{
	#if (DEBUG_ACTION_LEVEL >= 1)
		prt("Starting World::CompletedDSAction()");
	#endif

	++lastActionPerformed;

	voActionPending = false;
	// TODO: Should we only be releasing the write lock here?
	voActionPendingCondVar.notify_all();
		
	#if (DEBUG_ACTION_LEVEL >= 1)
		prt("Finishing World::CompletedDSAction()");
	#endif
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
	AcquireWriterLock();

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
	VO_Array* newArray = new VO_Array(dsArrayAddress, this, elementType, arrayElements);
	
	registeredArrays[dsArrayAddress] = newArray;

	ReleaseWriterLock();
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
	AcquireWriterLock();

	// Verify that SINGLE_PRINTABLE hasn't already been registered
	UL_ASSERT(!IsRegistered(dsSinglePrintableAddress));

	VO_SinglePrintable* newSP = new VO_SinglePrintable(dsSinglePrintableAddress, this, value);
	registeredSinglePrintables[dsSinglePrintableAddress] = newSP;

	ReleaseWriterLock();
	return newSP;
}


bool World::DeregisterObject(const void* dsAddress)
{
	AcquireWriterLock();

	if (registeredArrays.count(dsAddress))
	{
		VO_Array* voArray = registeredArrays[dsAddress];
		delete voArray;

		registeredArrays.erase(registeredArrays.find(dsAddress));

		ReleaseWriterLock();
		return true;
	}

	if (registeredSinglePrintables.count(dsAddress))
	{
		VO_SinglePrintable* voSP = registeredSinglePrintables[dsAddress];
		delete voSP;

		registeredSinglePrintables.erase(registeredSinglePrintables.find(dsAddress));

		ReleaseWriterLock();
		return true;
	}

	ReleaseWriterLock();
	return false;
}




void World::AddElementToArray(const void* dsArray, void* dsElement, unsigned position)
{
	AcquireWriterLock();

	UL_ASSERT(IsRegistered(dsArray,ARRAY));
	UL_ASSERT(IsRegistered(dsElement,SINGLE_PRINTABLE));

	VO_Array* arrayAddress = GetRepresentation<VO_Array>(dsArray);
	UL_ASSERT(position <= arrayAddress->GetSize());

	VO_SinglePrintable* element = GetRepresentation<VO_SinglePrintable>(dsElement);

	arrayAddress->AddElement((ViewableObject*)element, position);
	element->SetParent(arrayAddress);

	ReleaseWriterLock();
}

void World::SwapElementsInArray(const void* dsArray, unsigned firstElementIndex, unsigned secondElementIndex)
{
	AcquireWriterLock();

	UL_ASSERT(IsRegistered(dsArray, ARRAY));

	VO_Array* arrayAddress = GetRepresentation<VO_Array>(dsArray);
	UL_ASSERT(arrayAddress);

	arrayAddress->SwapElements(firstElementIndex, secondElementIndex);

	ReleaseWriterLock();
}

void World::ArrayResized(const void* dsArray, const std::vector<void*>& elements, unsigned newCapacity)
{
	AcquireWriterLock();

	UL_ASSERT(IsRegistered(dsArray));
	VO_Array* voArray = GetRepresentation<VO_Array>(dsArray);

	// Clear out old elements
	voArray->ClearArray(newCapacity);

	// Add new elements
	unsigned position = 0;
	BOOST_FOREACH(void* dsElement, elements)
		AddElementToArray(dsArray, dsElement, position++);

	ReleaseWriterLock();
}

void World::ClearArray(const void* dsArray)
{
	AcquireWriterLock();

	UL_ASSERT(IsRegistered(dsArray));
	VO_Array* voArray = GetRepresentation<VO_Array>(dsArray);
	
	// Clear out old elements - TODO fix the INVALID argument
	voArray->ClearArray(INVALID);

	ReleaseWriterLock();
}

void World::UpdateSinglePrintable(const void* dsSinglePrintableAddress, const std::string& newValue)
{
	AcquireWriterLock();

	UL_ASSERT(IsRegistered(dsSinglePrintableAddress, SINGLE_PRINTABLE));

	VO_SinglePrintable* sp = GetRepresentation<VO_SinglePrintable>(dsSinglePrintableAddress);
	UL_ASSERT(sp);

	sp->UpdateValue(newValue);

	ReleaseWriterLock();
}


void World::AcquireReaderLock(bool skipConditionVarCheck)
{
	#if(DEBUG_THREADING_LEVEL >= 3)
		prt("PRE-ACQUISITION OF READER LOCK");
	#endif

	// There's no need to guard against recursive reader lock acquisition (shared_mutex allows it without blcoking)
	// But we need to guard against a thread with a writer lock trying to acquire a reader lock
	if (GetCurrentThreadId() == writerLockOwner)
	{
		#if(DEBUG_THREADING_LEVEL >= 3)
			prt("\tTHREAD ALREADY HAS WRITER LOCK");
		#endif
		return;
	}
		
	voAccessMutex.lock_shared();

	if (!skipConditionVarCheck)
	{
		while (voActionPending)
			voActionPendingCondVar.wait<boost::shared_mutex>(voAccessMutex);
	}
	

	#if(DEBUG_THREADING_LEVEL >= 2)
		prt("POST-ACQUISITION OF READER LOCK");
	#endif
}

void World::ReleaseReaderLock()
{
	#if(DEBUG_THREADING_LEVEL >= 3)
		prt("PRE-RELEASING READER LOCK");
	#endif

	voAccessMutex.unlock_shared();

	#if(DEBUG_THREADING_LEVEL >= 2)
		prt("POST-RELEASING READER LOCK");
	#endif
}

void World::AcquireWriterLock()
{
	if (GetCurrentThreadId() == writerLockOwner)
		return;

	#if(DEBUG_THREADING_LEVEL >= 3)
		prt("PRE-ACQUISITION OF WRITER LOCK");
	#endif

	// Acquire upgrade lock, then upgrade it rather going straight for the writer lock
	voAccessMutex.lock_upgrade();
	voAccessMutex.unlock_upgrade_and_lock();

	#if(DEBUG_THREADING_LEVEL >= 2)
		prt("POST-ACQUISITION OF WRITER LOCK");
	#endif

	
	writerLockOwner = GetCurrentThreadId();
}

void World::ReleaseWriterLock()
{
	#if(DEBUG_THREADING_LEVEL >= 3)
		prt("PRE-RELEASING WRITER LOCK");	
	#endif

	writerLockOwner = NULL;
	voAccessMutex.unlock();

	#if(DEBUG_THREADING_LEVEL >= 2)
		prt("POST-RELEASING WRITER LOCK");	
	#endif
}



}