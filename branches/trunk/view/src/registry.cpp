#include "qt/qapplication.h"
#include "boost/foreach.hpp"
#include "utilities.h"

#include "../include/registry.h"
#include "displayer/world.h"
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
	/*
	typedef std::map<const void*,VO_Array*> ArrayMap;
	typedef std::map<const void*,VO_SinglePrintable*> SPMap;

	// Delete arrays first, as their destruction currently relies on their child elements 
	// being alive (since they deregister themselves from their elements as observers
	BOOST_FOREACH(ArrayMap::value_type arrayPair, registeredArrays)
		delete arrayPair.second;

	BOOST_FOREACH(SPMap::value_type spPair, registeredSinglePrintables)
		delete spPair.second;*/
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
		prt("\tStarting Registry::AddActionToBuffer()");
	#endif
	
	
	
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
		//prt("\tAdded action to buffer");
		prt("\tFinishing Registry::AddActionToBuffer()");
	#endif
}


void Registry::RegisterArray
		(const void* dsArrayAddress, ViewableObjectType elementType, const std::vector<void*>& elements)
{
	boost::unique_lock<boost::mutex> lock(bufferMutex);
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
	creationAction.SuppressAnimation();

	waitingOnCallback = true;
	lastViewableCreatedOrDestroyed = NULL;
	AddActionToBuffer(&creationAction);

	while(waitingOnCallback);
	lastViewableCreatedOrDestroyed->GetType();
	Register(dsArrayAddress, lastViewableCreatedOrDestroyed);
}


void Registry::RegisterSinglePrintable(const void* dsSinglePrintableAddress, const std::string& value)
{
	boost::unique_lock<boost::mutex> lock(bufferMutex);
	#if (DEBUG_REGISTRATION_LEVEL >= 3)
		std::cout << "Registering SP with value " << value << " @ " << dsSinglePrintableAddress	<< std::endl;
	#endif

	// Verify that array hasn't already been registered
	UL_ASSERT(!IsRegistered(dsSinglePrintableAddress));

	// Create action
	DS_CreateSP creationAction(world, dsSinglePrintableAddress, value);
	creationAction.SuppressAnimation();

	// Wait on callback
	waitingOnCallback = true;
	lastViewableCreatedOrDestroyed = NULL;
	AddActionToBuffer(&creationAction);

	while(waitingOnCallback);
	//cout << "\tInside Registry::RegisterSP - GetType() on " << dsSinglePrintableAddress << "\\" << lastViewableCreatedOrDestroyed << endl;
	//lastViewableCreatedOrDestroyed->GetType();

	Register(dsSinglePrintableAddress, lastViewableCreatedOrDestroyed);
	//cerr.flush();
	UL_ASSERT(IsRegistered(dsSinglePrintableAddress,SINGLE_PRINTABLE));
}


bool Registry::DeregisterObject(const void* dsAddress)
{
	boost::unique_lock<boost::mutex> lock(bufferMutex);
	#if (DEBUG_REGISTRATION_LEVEL >= 1)
		cout << "Deregistering " << dsAddress << endl;
	#endif

	ViewableObject* voToBeDeleted = GetRepresentation(dsAddress);
	UL_ASSERT(voToBeDeleted);
	ViewableObjectType voType = voToBeDeleted->GetType();
	

	// Create event
	DS_Deleted* deleteAction = new DS_Deleted(world, voToBeDeleted);
	deleteAction->SuppressAnimation();	

	waitingOnCallback = true;
	lastViewableCreatedOrDestroyed = NULL;
	AddActionToBuffer(deleteAction);
	
	while(waitingOnCallback);
	Deregister(dsAddress);

	return true;
}

void Registry::AddElementToArray(const void* dsArray, void* dsElement, unsigned position)
{
	boost::unique_lock<boost::mutex> lock(bufferMutex);
	//cout << "Inside Registry::AddElementToArray - adding dsElement @ " << dsElement << endl;

	UL_ASSERT(IsRegistered(dsArray,ARRAY));
	UL_ASSERT(IsRegistered(dsElement,SINGLE_PRINTABLE));

	VO_Array* voArray = GetRepresentation<VO_Array>(dsArray);
	UL_ASSERT(position <= voArray->GetSize());

	VO_SinglePrintable* element = GetRepresentation<VO_SinglePrintable>(dsElement);

	// Create event
	DS_AddElementToArray* addAction = new DS_AddElementToArray(world, voArray, element, position);
	addAction->SuppressAnimation();
	
	waitingOnCallback = true;
	AddActionToBuffer(addAction);
	while(waitingOnCallback);
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
		boost::unique_lock<boost::mutex> lock(bufferMutex);
	#ifdef DEBUG_ARRAY_CHANGES
		prt("Registering array resize");
	#endif

	UL_ASSERT(IsRegistered(dsArray, ARRAY));
	VO_Array* voArray = GetRepresentation<VO_Array>(dsArray);
	UL_ASSERT(voArray);

	UL_ASSERT(newCapacity >= elements.size());
	vector<ViewableObject*> elementsToAdd;
	
	BOOST_FOREACH(void* dsElement, elements)
	{
		UL_ASSERT(IsRegistered(dsElement));
		elementsToAdd.push_back(GetRepresentation(dsElement));
	}

	DS_ArrayResize* action = new DS_ArrayResize(world, voArray, elementsToAdd, newCapacity);


	waitingOnCallback = true;
	AddActionToBuffer(action);
	while(waitingOnCallback);

}

void Registry::ClearArray(const void* dsArray)
{
	boost::unique_lock<boost::mutex> lock(bufferMutex);
	// TODO
	prt("UNIMPLEMENTED!!!!!!!!!!!!!!!!!!!!!!!");
}

void Registry::PrintableAssigned(const void* dsAssigned, const void* dsSource, const std::string& newValue)
{
	boost::unique_lock<boost::mutex> lock(bufferMutex);

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

		// TODO: do we need tracked bool anymore???
		DS_Assigned action(world, sp, source->GetHistory(), newValue, true);
		
		// TODO: Currently we only want an animation for assignments to SPs which are owned by an array
		if (sp->parent())
		{
			if (typeid(*sp->parent()) != typeid(VO_Array))
				action.SuppressAnimation();
			else
			{
				// VO belongs to array
				action.SetSource(source);
			}
		}
		else
		{
			action.SuppressAnimation();
		}

		
		waitingOnCallback = true;
				
		AddActionToBuffer(&action);
		while(waitingOnCallback);
	}
	else
	{
		// The only history we have is that the value was untracked, so chuck in an element to represent that
		set<ValueID> history;
		history.clear();
		// Time -1 denotes elements that aren't actually tracked (TODO: That sucks)
		// dsSource or dsAssigned??? check repository TODO
		history.insert(ValueID(dsAssigned, -1)); 

		DS_Assigned action(world,sp,history,newValue,false);
		action.SuppressAnimation();
		waitingOnCallback = true;
		AddActionToBuffer(&action);
		while(waitingOnCallback);

	}
}

// TODO: This is really similar to above
void Registry::PrintableModified(const void* dsModified, const void* dsSource, const std::string& newValue)
{
	UL_ASSERT(false);
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
	{
		sp->ModifiedUntracked(dsSource, newValue);
	}	

	
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
	util::ReaderLock<util::LockManager<1>,1> lock(*this);
	bool isRegistered = (registeredArrays.count(dsAddress) > 0) || 
						(registeredSinglePrintables.count(dsAddress) > 0);
	return isRegistered;
}

bool Registry::IsRegistered(const void* dsAddress, ViewableObjectType voType) const
{
	util::ReaderLock<util::LockManager<1>,1> lock(*this);

	bool isRegistered = false;

	switch(voType)
	{
		case ARRAY:
			isRegistered = registeredArrays.count(dsAddress) > 0;
			break;

		case SINGLE_PRINTABLE:
			isRegistered = registeredSinglePrintables.count(dsAddress) > 0;
			break;
		default:
			UL_ASSERT(false);
			break;
	}

	UL_ASSERT(isRegistered == IsRegistered(dsAddress));
	return isRegistered;
}

ViewableObject* Registry::GetRepresentation(const void* dsAddress)
{
	util::ReaderLock<util::LockManager<1>,1> lock(*this);

	if (registeredArrays.count(dsAddress) > 0)
		return (ViewableObject*) registeredArrays[dsAddress];

	if (registeredSinglePrintables.count(dsAddress) > 0)
		return (ViewableObject*) registeredSinglePrintables[dsAddress];

	UL_ASSERT(false);
	return NULL;
}

void Registry::Register(const void* dsAddress, ViewableObject* obj)
{
	util::WriterLock<util::LockManager<1>,1> lock(*this);
	UL_ASSERT(obj);
	
	switch(obj->GetType())
	{
		case ARRAY:
			registeredArrays[dsAddress] = (VO_Array*)obj;
			break;
		case SINGLE_PRINTABLE:
			registeredSinglePrintables[dsAddress] = (VO_SinglePrintable*) obj;
			break;
		default:
			UL_ASSERT(false);
			break;

	}
	UL_ASSERT(IsRegistered(dsAddress));
}


bool Registry::Deregister(const void* dsAddress)
{
	util::WriterLock<util::LockManager<1>,1> lock(*this);

	if (registeredArrays.count(dsAddress))
	{
		registeredArrays.erase(registeredArrays.find(dsAddress));
		return true;
	}
	else if (registeredSinglePrintables.count(dsAddress))
	{
		registeredSinglePrintables.erase(registeredSinglePrintables.find(dsAddress));
		return true;
	}
	else
	{
		return false;
	}
}


}


