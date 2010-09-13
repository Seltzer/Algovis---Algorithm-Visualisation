#include <algorithm>
#include "utilities.h"
#include "registry.h"

#include "userFunctions.h"
#include "idManager.h"
#include "settingsManager.h"

using namespace std;



namespace Algovis
{


//////////// Singleton / Initialisation Stuff
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
	: nextIdToAllocate(0), transplantModeEnabled(false)
{
	// Construct early
	SettingsManager::GetInstance();
}




//////////////// Id Allocation
ID IdManager::GetId(const Wrapper* wrapper)
{
	if (idMapping.count(wrapper))
		return idMapping[wrapper];
	else
		return INVALID;
}

ID IdManager::GetIdForConstruction(const Wrapper* wrapper)
{
	#if (ID_DEBUGGING_LEVEL >= 1)
		cout << "Wrapper construction - being allocated id " << nextIdToAllocate << endl;
	#endif

	idMapping[wrapper] = nextIdToAllocate++;
	RemoveFromOrphanList(wrapper);

	return idMapping[wrapper];
}

CopyConstructionInfo IdManager::GetIdForCopyConstruction(const Wrapper* newWrapper, const Wrapper* originalWrapper)
{
	#if (ID_DEBUGGING_LEVEL >= 1)
		cout << "Copy construction of originalWrapper ID" << idMapping[originalWrapper] << endl;
	#endif

	CopyConstructionInfo returnInfo;
	
	if (idMapping.count(originalWrapper))
		returnInfo.otherId = idMapping[originalWrapper];
	else
		returnInfo.otherId = INVALID;
	

	bool originalIsException = IsWrapperAnException(originalWrapper);

	// Figure out whether transplant conditions are met
	if ( (transplantModeEnabled && !originalIsException) || (!transplantModeEnabled && originalIsException) )
	{
		// Transplant
		TransplantWrapper(newWrapper, originalWrapper);
		returnInfo.result = CopyConstructionInfo::TRANSPLANT;
		returnInfo.newId = idMapping[newWrapper];

		// Let Registry know that the address of newWrapper has changed
		if (communicationWithViewEnabled)
			Algovis_Viewer::Registry::GetInstance()->AddressChanged(returnInfo.newId, newWrapper);
	}
	else
	{
		// Don't transplant
		idMapping[newWrapper] = nextIdToAllocate++;
		returnInfo.result = CopyConstructionInfo::NORMAL_CC;
		returnInfo.newId = idMapping[newWrapper];

		#if (ID_DEBUGGING_LEVEL >=2)
			cout << "\tRegular copy construction - newWrapper id = " << idMapping[newWrapper] << endl;
		#endif
	}

	// If originalWrapper is an exception to current mode, then so is newWrapper
	if (originalIsException)
		modeExceptions.push_back(idMapping[newWrapper]);

	return returnInfo;
}

CopyAssignmentInfo IdManager::GetIdForCopyAssignment(const Wrapper* wrapper, const Wrapper* originalWrapper)
{
	UL_ASSERT(idMapping.count(wrapper));
	UL_ASSERT(idMapping.count(originalWrapper));

	#if (ID_DEBUGGING_LEVEL >= 1)
		cout << "Copy assignment to ID" << idMapping[wrapper] << "from ID" << idMapping[originalWrapper] << endl;
	#endif


	CopyAssignmentInfo returnInfo;
	returnInfo.oldId = idMapping[wrapper];
	returnInfo.otherId = idMapping[originalWrapper];
	

	bool originalIsException = IsWrapperAnException(originalWrapper);

	// Figure out whether transplant conditions are met
	if ( (transplantModeEnabled && !originalIsException) || (!transplantModeEnabled && originalIsException) )
	{
		// Transplant originalWrapper to newWrapper
		TransplantWrapper(wrapper, originalWrapper);

		// If originalWrapper is an exception to current mode, then so is newWrapper
		if (originalIsException)
			modeExceptions.push_back(idMapping[wrapper]);

		returnInfo.result = CopyAssignmentInfo::TRANSPLANT;
	}
	else
	{
		// Non-transplant mode
		if (IsOrphan(wrapper))
		{
			RemoveFromOrphanList(wrapper);
			GetIdForConstruction(wrapper);
			returnInfo.result = CopyAssignmentInfo::ORPHAN_REBIRTH;

			#if (ID_DEBUGGING_LEVEL >=2)
				cout << "\twrapper was an orphan but was allocated a new ID" << idMapping[wrapper] << endl;
			#endif
		}
		else
		{
			returnInfo.result = CopyAssignmentInfo::NORMAL_ASSIGNMENT;
			#if (ID_DEBUGGING_LEVEL >=2)
				cout << "\tRegular copy assignment - ID" << idMapping[wrapper] << "retained its id" << endl;
			#endif
		}
	}


	// If originalWrapper is an exception to current mode, then so is newWrapper
	if (originalIsException)
		modeExceptions.push_back(idMapping[wrapper]);


	returnInfo.newId = idMapping[wrapper];
	return returnInfo;
}


void IdManager::ReportDestruction(const Wrapper* wrapper)
{
	UL_ASSERT(idMapping.count(wrapper));

	#if (ID_DEBUGGING_LEVEL >= 1)
		cout << "Performing destruction of wrapper with id = " << idMapping[wrapper] << endl;
	#endif


	// Remove wrapper from id mapping
	ID id = idMapping[wrapper];
	idMapping.erase(idMapping.find(wrapper));
		
	if (orphanedWrappers.count(wrapper) > 0)
	{
		#if (ID_DEBUGGING_LEVEL >=2)
			cout << "\tOrphans die quietly - don't report destruction to the Registry" << endl;
		#endif

		// Orphans die quietly - don't report destruction to the Registry
		RemoveFromOrphanList(wrapper);	
	}
	else
	{
		#if (ID_DEBUGGING_LEVEL >=2)
			cout << "\tReporting destruction to registry (not an orphan)" << endl;
		#endif

		// Wrapper is not an orphan, so deregister it with the Registry
		if (communicationWithViewEnabled && SettingsManager::GetInstance()->DestructionReportingEnabled())
			Algovis_Viewer::Registry::GetInstance()->DeregisterObject(id);
	}
}


//////////// Modes
void IdManager::EnableTransplantMode(bool enabled, ID modeException)
{
	modeExceptions.clear();
	modeExceptions.push_back(modeException);
	transplantModeEnabled = enabled;
}

void IdManager::EnableTransplantMode(bool enabled, vector<const ID> modeExceptions)
{
	this->modeExceptions = modeExceptions;
	transplantModeEnabled = enabled;
}

void IdManager::PurgeModeExceptions()
{
	modeExceptions.clear();
}

bool IdManager::IsWrapperAnException(const Wrapper* wrapper)
{
	if (idMapping.count(wrapper) == 0)
		return false;

	return (modeExceptions.end() != find(modeExceptions.begin(), modeExceptions.end(), idMapping[wrapper]));
}



void IdManager::TransplantWrapper(const Wrapper* newWrapper, const Wrapper* originalWrapper)
{
	// Make sure that originalWrapper hasn't already been transplanted
	UL_ASSERT(orphanedWrappers.count(originalWrapper) == 0);
	UL_ASSERT(idMapping[originalWrapper] != ORPHAN_ID);

	#if (ID_DEBUGGING_LEVEL >=2)
		cout << "\tNew wrapper being transplanted from original ID" << idMapping[originalWrapper] << endl;
	#endif

	RemoveFromOrphanList(newWrapper);
	idMapping[newWrapper] = idMapping[originalWrapper];
	orphanedWrappers.insert(originalWrapper);
	idMapping[originalWrapper] = ORPHAN_ID;
}


bool IdManager::IsOrphan(const Wrapper* wrapper)
{
	return orphanedWrappers.count(wrapper) > 0;
}

void IdManager::RemoveFromOrphanList(const Wrapper* wrapper)
{
	if (IsOrphan(wrapper))
		orphanedWrappers.erase(orphanedWrappers.find(wrapper));

}



}

