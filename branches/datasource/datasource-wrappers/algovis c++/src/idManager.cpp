#include <algorithm>
#include "utilities.h"
#include "registry.h"

#include "userFunctions.h"
#include "idManager.h"
#include "settingsManager.h"



using namespace std;


namespace Algovis
{


IdManager* IdManager::idMgrInstance(NULL);


IdManager* IdManager::GetInstance()
{
	if (!IdManager::idMgrInstance)
		IdManager::idMgrInstance = new IdManager();

	return IdManager::idMgrInstance;

}

void IdManager::DestroyInstance()
{
	if (IdManager::idMgrInstance)
	{
		delete IdManager::idMgrInstance;
		IdManager::idMgrInstance = NULL;
	}
}
	
IdManager::IdManager()
	: currentId(0), transplantModeEnabled(false)
{
	// Construct early
	SettingsManager::GetInstance();
}


ID IdManager::GetId(const Wrapper* wrapper)
{
	if (idMapping.count(wrapper))
		return idMapping[wrapper];
	else
		return INVALID;
}

ID IdManager::GetIdForConstruction(const Wrapper* wrapperAddress)
{
	idMapping[wrapperAddress] = currentId;

	#if (ID_DEBUGGING_LEVEL >= 1)
		cout << "Construction of newWrapper - allocated id " << currentId << endl;
	#endif

	RemoveWrapperFromTransplantSources(wrapperAddress);


	return currentId++;
}

ID IdManager::GetIdForCopyConstruction(const Wrapper* newWrapper, const Wrapper* originalWrapper)
{
	bool originalIsException = IsWrapperAnExceptionBySource(originalWrapper);
	bool newIsException = IsWrapperAnExceptionByDest(newWrapper);	

	#if (ID_DEBUGGING_LEVEL >= 1)
		cout << "Copy construction of oldWrapper with id = " << idMapping[originalWrapper] << endl;
	#endif

	// If originalWrapper is exception to current mode, then so is newWrapper
	if (originalIsException)
		modeExceptionsBySource.push_back(newWrapper);


	//RemoveWrapperFromTransplantSources(newWrapper);

	// Figure out whether this should be a transplant or not
	if ( (transplantModeEnabled && !originalIsException && !newIsException) || 
		 (!transplantModeEnabled && (originalIsException || newIsException)) )
	{
		// Transplant
		idMapping[newWrapper] = idMapping[originalWrapper];
		transplantSources.insert(originalWrapper);
		idMapping[originalWrapper] = INVALID_ID;

		#if (ID_DEBUGGING_LEVEL >=2)
			cout << "\tPerformed transplant - newWrapper id = " << idMapping[newWrapper] << endl;
		#endif
		
		return idMapping[newWrapper];
	}
	else
	{
		// Don't transplant
		idMapping[newWrapper] = currentId;

		#if (ID_DEBUGGING_LEVEL >=2)
			cout << "\tRegular copy construction - newWrapper id = " << idMapping[newWrapper] << endl;
		#endif

		return currentId++;
	}
}

ID IdManager::GetIdForCopyAssignment(const Wrapper* newWrapper, const Wrapper* originalWrapper)
{
	UL_ASSERT(idMapping.count(newWrapper));
	UL_ASSERT(idMapping.count(originalWrapper));

	#if (ID_DEBUGGING_LEVEL >= 1)
		cout << "Copy assignment to newWrapper with id = " << idMapping[newWrapper];
		cout << " from originalWrapper with id = " << idMapping[originalWrapper] << endl;
	#endif


	if (transplantSources.count(newWrapper))
		RemoveWrapperFromTransplantSources(newWrapper);


	bool originalIsException = IsWrapperAnExceptionBySource(originalWrapper);
	bool newIsException = IsWrapperAnExceptionByDest(newWrapper);	

	// If originalWrapper is exception to current mode, then so is newWrapper
	if (originalIsException)
		modeExceptionsBySource.push_back(newWrapper);

	// Figure out whether this should be a transplant or not
	if ( (transplantModeEnabled && !originalIsException && !newIsException) || 
		 (!transplantModeEnabled && (originalIsException || newIsException)) )
	{
		// Transplant originalWrapper to newWrapper


		// Allocate newWrapper the same id as originalWrapper
		idMapping[newWrapper] = idMapping[originalWrapper];
		
		if (transplantSources.count(originalWrapper) == 0)
			RemoveWrapperFromTransplantSources(newWrapper);

		transplantSources.insert(originalWrapper);
		idMapping[originalWrapper] = INVALID_ID;
		
		// TODO: what do we do with original ID of newWrapper - alert view that it no longer exists????
		// In the case of a vector::erase-->Registry::RemoveElements, the Reg knows to remove 
		// erased elements which have have IDs in this situation
		
		#if (ID_DEBUGGING_LEVEL >=2)
			cout << "\toldWrapper transplanted to newWrapper" << endl;
		#endif
				
		return idMapping[newWrapper];
	}
	else
	{
		// This is a regular copy assignment (not a transplant), so newWrapper should retain its current id
		// (unless it was a transplant source)
		
		// New id
		if (idMapping[newWrapper] == INVALID_ID)
		{
			#if (ID_DEBUGGING_LEVEL >=2)
				cout << "\tnewWrapper got a new id (weird copy assignment)" << endl;
			#endif

			if (transplantSources.count(originalWrapper) == 0)
				RemoveWrapperFromTransplantSources(newWrapper);

			return GetIdForConstruction(newWrapper);
		}
		
		#if (ID_DEBUGGING_LEVEL >=2)
			cout << "\tnewWrapper retained its id (regular copy assignment)" << endl;
		#endif


		return idMapping[newWrapper];
	}
}

void IdManager::ReportDestruction(const Wrapper* wrapperAddress)
{
	if (!idMapping.count(wrapperAddress))
		UL_ASSERT(false);

	ID id = idMapping[wrapperAddress];

	#if (ID_DEBUGGING_LEVEL >= 1)
		cout << "Performing destruction of wrapper with id = " << id << endl;
	#endif


	// Remove wrapper from id mapping
	idMapping.erase(idMapping.find(wrapperAddress));
		

	if (transplantSources.count(wrapperAddress) > 0)
	{
		// Wrapper has been transplanted in the past so don't report its destruction to the Registry
		transplantSources.erase(transplantSources.find(wrapperAddress));
	}
	else
	{
		// Wrapper was not the source of a transplant, so deregister it with the Registry
		if (drawingEnabled && SettingsManager::GetInstance()->DestructionReportingEnabled())
			Algovis_Viewer::Registry::GetInstance()->DeregisterObject(id);
	}
}


void IdManager::EnableTransplantMode(
		std::vector<const Wrapper*>& transplantModeExceptionsBySource,
		std::vector<const Wrapper*>& transplantModeExceptionsByDest)
{
	modeExceptionsBySource = transplantModeExceptionsBySource;
	modeExceptionsByDestination = transplantModeExceptionsByDest;
	transplantModeEnabled = true;
}

void IdManager::EnableTransplantMode(std::vector<const ID> modeExceptionsBySourceId)
{
	this->modeExceptionsBySourceId = modeExceptionsBySourceId;
	modeExceptionsBySource.clear();
	modeExceptionsByDestination.clear();
	transplantModeEnabled = true;
}

void IdManager::DisableTransplantMode(
				std::vector<const Wrapper*>& nonTransplantModeExceptionsBySource,
				std::vector<const Wrapper*>& nonTransplantModeExceptionsByDest)
{
	modeExceptionsBySourceId.clear();
	modeExceptionsBySource = nonTransplantModeExceptionsBySource;
	modeExceptionsByDestination = nonTransplantModeExceptionsByDest;
	transplantModeEnabled = false;
}


void IdManager::DisableTransplantMode(std::vector<const ID> modeExceptionsBySourceId)
{
	this->modeExceptionsBySourceId = modeExceptionsBySourceId;
	modeExceptionsBySource.clear();
	modeExceptionsByDestination.clear();
	transplantModeEnabled = false;
}

void IdManager::EnableTransplantMode(const Wrapper* transplantModeException)
{
	modeExceptionsBySourceId.clear();
	modeExceptionsBySource.clear();
	modeExceptionsBySource.push_back(transplantModeException);
	modeExceptionsByDestination.clear();
	transplantModeEnabled = true;
}

void IdManager::DisableTransplantMode(const Wrapper* nonTransplantModeException)
{
	modeExceptionsBySourceId.clear();
	modeExceptionsBySource.clear();
	modeExceptionsBySource.push_back(nonTransplantModeException);
	modeExceptionsByDestination.clear();
	transplantModeEnabled = false;
}




bool IdManager::IsWrapperAnExceptionBySource(const Wrapper* wrapper)
{
	bool exceptionByAddress = find(modeExceptionsBySource.begin(), modeExceptionsBySource.end(), wrapper) != modeExceptionsBySource.end();
	
	if (idMapping.count(wrapper) == 0)
		return exceptionByAddress;

	bool exceptionById = find(modeExceptionsBySourceId.begin(), modeExceptionsBySourceId.end(), idMapping[wrapper]) != modeExceptionsBySourceId.end();


	return exceptionByAddress || exceptionById;
}

bool IdManager::IsWrapperAnExceptionByDest(const Wrapper* wrapper)
{
	return find(modeExceptionsByDestination.begin(), modeExceptionsByDestination.end(), wrapper) != modeExceptionsByDestination.end();
}


void IdManager::RemoveWrapperFromTransplantSources(const Wrapper* wrapper)
{
	if (transplantSources.count(wrapper))
		transplantSources.erase(transplantSources.find(wrapper));

}



}

