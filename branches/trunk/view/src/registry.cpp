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
#include "actions/dsArrayActions.h"
#include "actions/dsPrintableActions.h"

using namespace std;



namespace Algovis_Viewer
{


Registry* Registry::instance(NULL);


///////////////////////// Private methods

Registry::Registry()
	: world(NULL), displayerShuttingDown(false), actionBuffer(3)
{ 
	#if (DEBUG_GENERAL_LEVEL >= 2)
		prt("REGISTRY CTOR");
	#endif

	world = Displayer::GetInstance()->GetWorld();
	UL_ASSERT(world);

	time = 0;
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


	#if (DEBUG_ACTION_LEVEL >= 2)
		prt("\tFinishing Registry::AddActionToBuffer()");
	#endif
}


void Registry::RegisterArray
		(unsigned id, const void* dsArrayAddress, ViewableObjectType elementType, const std::vector<ID>& elements)
{
	boost::unique_lock<boost::mutex> lock(registryMutex);
	#if (DEBUG_REGISTRATION_LEVEL >= 2)
		std::cout << "Registering array with ID " << id << std::endl;
	#endif

	
	DS_CreateArray creationAction(world, id, dsArrayAddress, elementType, elements);
	creationAction.SuppressAnimation();

	AddActionToBuffer(&creationAction);
}


void Registry::RegisterSinglePrintable(unsigned id, const void* dsSinglePrintableAddress, const std::string& value)
{
	boost::unique_lock<boost::mutex> lock(registryMutex);
	#if (DEBUG_REGISTRATION_LEVEL >= 3)
		std::cout << "Registering SP with value " << value << " and id " << id<< std::endl;
	#endif

	// Create action
	DS_CreateSP creationAction(world, id, dsSinglePrintableAddress, value);
	creationAction.SuppressAnimation();

	AddActionToBuffer(&creationAction);

}

void Registry::AddressChanged(ID id, const void* newAddress)
{
	boost::unique_lock<boost::mutex> lock(registryMutex);

	UL_ASSERT(IsRegistered(id));
	const void* oldAddress = registeredViewables[id]->GetDSAddress();
	DS_AddressChanged action(world,id,newAddress,oldAddress);

	AddActionToBuffer(&action);
}


bool Registry::DeregisterObject(ID id)
{
	boost::unique_lock<boost::mutex> lock(registryMutex);
	#if (DEBUG_REGISTRATION_LEVEL >= 1)
		cout << "Deregistering " << id << endl;
	#endif


	// Create event
	DS_Deleted* deleteAction = new DS_Deleted(world, id);
	deleteAction->SuppressAnimation();	

	AddActionToBuffer(deleteAction);

	return true;
}

void Registry::AddElementToArray(ID dsArray, ID dsElement, unsigned position)
{
	boost::unique_lock<boost::mutex> lock(registryMutex);
	//cout << "Inside Registry::AddElementToArray - adding dsElement @ " << dsElement << endl;


	// Create event
	DS_AddElementToArray* addAction = new DS_AddElementToArray(world, dsArray, dsElement, position);
	//addAction->SuppressAnimation(); // This has an animation now
	
	AddActionToBuffer(addAction);
}

/*
void Registry::ClearArray(const void* dsArray)
{
	boost::unique_lock<boost::mutex> lock(registryMutex);
	prt("UNIMPLEMENTED!!!!!!!!!!!!!!!!!!!!!!!");
}*/

void Registry::PrintableAssigned(ID dsAssigned, ID dsSource, const std::string& newValue)
{
	boost::unique_lock<boost::mutex> lock(registryMutex);
 
	#ifdef DEBUG_SP_CHANGES
		std::cout << "Registry::PrintableAssigned for " << dsAssigned << std::endl;
	#endif

	if (dsSource == INVALID)
		cout << "Warning! dsSource for PrintableAssigned is = " << INVALID << endl;


	DS_Assigned action(world, dsAssigned, dsSource, newValue, true);
	AddActionToBuffer(&action);
}

// TODO: This is really similar to above
void Registry::PrintableModified(ID dsModified, ID dsSource, const std::string& newValue)
{
	// Don't expect this method to work atm

	// TODO: This will not include itself as a source in the animation. As far as I know thats the only issue with using an assigned action
	DS_Modified action(world, dsModified, dsSource, newValue, true);
	AddActionToBuffer(&action);
}

void Registry::HighlightOperands(const std::vector<ID>& operands)
{
	DS_HighlightOperands action(world, operands);
	AddActionToBuffer(&action);
}




//////////// Registration methods
bool Registry::IsRegistered(ID id) const
{
	util::ReaderLock<util::LockManager<1>,1> lock(*this);

	return registeredViewables.count(id) > 0;
}

bool Registry::IsRegistered(ID id, ViewableObjectType voType)
{
	util::ReaderLock<util::LockManager<1>,1> lock(*this);

	if (registeredViewables.count(id) == 0)
		return false;

	return registeredViewables[id]->GetType() == voType;
}

ViewableObject* Registry::GetRepresentation(ID id)
{
	util::ReaderLock<util::LockManager<1>,1> lock(*this);

	if (!IsRegistered(id))
		return NULL;

	return registeredViewables[id];
}

void Registry::Register(ID id, ViewableObject* viewable)
{
	util::WriterLock<util::LockManager<1>,1> lock(*this);

	UL_ASSERT(viewable);
	UL_ASSERT(!IsRegistered(id));
	UL_ASSERT(id != INVALID);
	UL_ASSERT(id < INT_MAX);

	registeredViewables[id] = viewable;
}


bool Registry::Deregister(ID id)
{
	util::WriterLock<util::LockManager<1>,1> lock(*this);

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


