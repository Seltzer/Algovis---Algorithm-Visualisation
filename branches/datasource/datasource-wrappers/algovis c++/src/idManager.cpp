#include <algorithm>
#include "idManager.h"

#include "utilities.h"
#include "registry.h"
#include "userFunctions.h"

namespace Algovis
{


IdManager* IdManager::idMgrInstance(NULL);


IdManager* IdManager::GetInstance()
{
	if (!IdManager::idMgrInstance)
		IdManager::idMgrInstance = new IdManager();

	return IdManager::idMgrInstance;

}
	
IdManager::IdManager()
	: currentId(0), transplantModeEnabled(false)
{
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
	return currentId++;
}

ID IdManager::GetIdForCopyConstruction(const Wrapper* newWrapper, const Wrapper* originalWrapper)
{
	if (!transplantModeEnabled)
	{
		idMapping[newWrapper] = currentId;
		return currentId++;
	}
	else
	{
		if (std::find(transplantExceptions.begin(), transplantExceptions.end(), originalWrapper)
				!= transplantExceptions.end())
		{
			// originalWrapper is in the transplantExceptions, so give newWrapper a new ID and
			// add it to transplantExceptions
			transplantExceptions.push_back(newWrapper);
	
			idMapping[newWrapper] = currentId;
			return currentId++;
		}
		else
		{
			// originalWrapper is not in the exceptions, so perform a transplant
			transplantedWrappers.insert(originalWrapper);
			return (idMapping[newWrapper] = idMapping[originalWrapper]);
		}
	}
}

void IdManager::ReportDestruction(const Wrapper* wrapperAddress)
{
	// Remove wrapper from id mapping
	if (!idMapping.count(wrapperAddress))
	{
		UL_ASSERT(false);
		return;
	}
	

	ID id = idMapping[wrapperAddress];
	idMapping.erase(idMapping.find(wrapperAddress));
		

	// If wrapper has been transplanted in the past, then don't report its destruction to the Registry
	if (transplantedWrappers.count(wrapperAddress) == 0)
	{
		// Wrapper was not the source of a transplant, so deregister it with the Registry
		if (drawingEnabled)
			Algovis_Viewer::Registry::GetInstance()->DeregisterObject(id);
	}
	else
	{
		transplantedWrappers.erase(transplantedWrappers.find(wrapperAddress));
	}
}


void IdManager::EnableTransplantMode(std::vector<const Wrapper*>& exceptions)
{
	transplantExceptions = exceptions;
	transplantModeEnabled = true;
}


void IdManager::DisableTransplantMode()
{
	transplantExceptions.clear();
	transplantModeEnabled = false;
}








}

