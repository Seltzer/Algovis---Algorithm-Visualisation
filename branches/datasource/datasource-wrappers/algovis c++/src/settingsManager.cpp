#include "settingsManager.h"

#include <iostream>


namespace Algovis
{


SettingsManager* SettingsManager::settingsMgrInstance(NULL);


SettingsManager::SettingsManager()
	: constructionReportingEnabled(true), copyConstructionReportingEnabled(true),
		copyAssignmentReportingEnabled(true), destructionReportingEnabled(true)
{
}


SettingsManager* SettingsManager::GetInstance()
{
	if (!SettingsManager::settingsMgrInstance)
		SettingsManager::settingsMgrInstance = new SettingsManager();

	return SettingsManager::settingsMgrInstance;
}



void SettingsManager::EnableConstructionReporting(bool enabled)
{
	constructionReportingEnabled = enabled;

}

bool SettingsManager::ConstructionReportingEnabled()
{
	return constructionReportingEnabled;
}


void SettingsManager::EnableCopyConstructionReporting(bool enabled)
{
	copyConstructionReportingEnabled = enabled;
}

bool SettingsManager::CopyConstructionReportingEnabled()
{
	return copyConstructionReportingEnabled;
}

	
void SettingsManager::EnableCopyAssignmentReporting(bool enabled)
{
	copyAssignmentReportingEnabled = enabled;
}
		
bool SettingsManager::CopyAssignmentReportingEnabled()
{
	return copyAssignmentReportingEnabled;
}

		
void SettingsManager::EnableDestructionReporting(bool enabled)
{
	destructionReportingEnabled = enabled;
}

bool SettingsManager::DestructionReportingEnabled()
{
	return destructionReportingEnabled;
}



}
