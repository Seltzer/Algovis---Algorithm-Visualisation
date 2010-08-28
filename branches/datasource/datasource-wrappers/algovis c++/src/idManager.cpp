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


unsigned IdManager::GetIdForConstruction(const void* wrapperAddress)
{
	idMapping[wrapperAddress] = currentId;
	return currentId++;
}

unsigned IdManager::GetIdForCopyConstruction(const void* newWrapper, const void* originalWrapper)
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

void IdManager::ReportDestruction(const void* wrapperAddress)
{
	// Remove wrapper from id mapping
	if (idMapping.count(wrapperAddress))
		idMapping.erase(idMapping.find(wrapperAddress));

	// If wrapper has been transplanted in the past, then don't report its destruction to the Registry
	if (transplantedWrappers.count(wrapperAddress) == 0)
	{
		// Wrapper was not the source of a transplant, so deregister it with the Registry
		if (drawingEnabled)
			Algovis_Viewer::Registry::GetInstance()->DeregisterObject(wrapperAddress);
	}
	else
	{
		transplantedWrappers.erase(transplantedWrappers.find(wrapperAddress));
	}
}


void IdManager::EnableTransplantMode(std::vector<const void*>& exceptions)
{
	prt("Transplant mode enabled");
	transplantExceptions = exceptions;
	transplantModeEnabled = true;
}


void IdManager::DisableTransplantMode()
{
	transplantExceptions.clear();
	transplantModeEnabled = false;
}








}

