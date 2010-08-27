#include "qt/qapplication.h"
#include "boost/foreach.hpp"
#include "utilities.h"

#include "../include/registry.h"
#include "displayer/world.h"
#include "displayer/displayer.h"
#include "viewableObjects/viewableObject.h"
#include "viewableObjects/vo_array.h"
#include "viewableObjects/vo_singlePrintable.h"
#include "actions/action.h"
#include "actions/dsActions.h"

using namespace std;



namespace Algovis_Viewer
{


Registry* Registry::instance(NULL);


///////////////////////// Private methods

Registry::Registry()
	: actionBuffer(3)
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

	actionBuffer.PushBack(dsAction);
	
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
	
	DS_CreateArray creationAction(world, dsArrayAddress, elementType, elements);
	creationAction.SuppressAnimation();

	AddActionToBuffer(&creationAction);
}


void Registry::RegisterSinglePrintable(const void* dsSinglePrintableAddress, const std::string& value)
{
	boost::unique_lock<boost::mutex> lock(bufferMutex);
	#if (DEBUG_REGISTRATION_LEVEL >= 3)
		std::cout << "Registering SP with value " << value << " @ " << dsSinglePrintableAddress	<< std::endl;
	#endif

	// Create action
	DS_CreateSP creationAction(world, dsSinglePrintableAddress, value);
	creationAction.SuppressAnimation();


	AddActionToBuffer(&creationAction);
}


bool Registry::DeregisterObject(const void* dsAddress)
{
	boost::unique_lock<boost::mutex> lock(bufferMutex);
	#if (DEBUG_REGISTRATION_LEVEL >= 1)
		cout << "Deregistering " << dsAddress << endl;
	#endif

	// Create event
	DS_Deleted* deleteAction = new DS_Deleted(world, dsAddress);
	deleteAction->SuppressAnimation();	

	AddActionToBuffer(deleteAction);

	return true;
}

void Registry::AddElementToArray(const void* dsArray, void* dsElement, unsigned position)
{
	boost::unique_lock<boost::mutex> lock(bufferMutex);
	//cout << "Inside Registry::AddElementToArray - adding dsElement @ " << dsElement << endl;


	// Create event
	DS_AddElementToArray* addAction = new DS_AddElementToArray(world, dsArray, dsElement, position);
	addAction->SuppressAnimation();
	
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
		boost::unique_lock<boost::mutex> lock(bufferMutex);
	#ifdef DEBUG_ARRAY_CHANGES
		prt("Registering array resize");
	#endif

	DS_ArrayResize* action = new DS_ArrayResize(world, dsArray, elements, newCapacity);

	AddActionToBuffer(action);

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

	DS_Assigned action(world, dsAssigned, dsSource, newValue, true);
	AddActionToBuffer(&action);
}

// TODO: This is really similar to above
void Registry::PrintableModified(const void* dsModified, const void* dsSource, const std::string& newValue)
{
	UL_ASSERT(false);
	//world->AcquireWriterLock();

	// TODO: This will not include itself as a source in the animation. As far as I know thats the only issue with using an assigned action
	DS_Modified action(world, dsModified, dsSource, newValue, true);
	AddActionToBuffer(&action);
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


