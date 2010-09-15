
#include "historyManager.h"

#include "utilities.h"

#include "../viewableObjects/viewableObject.h"

namespace Algovis_Viewer
{

HistoryManager::HistoryManager()
{
	time = 0;
}

void HistoryManager::AddRecord(ID id)
{
	HistoryData h;
	h.visible = false;
	h.modifiedTime = time;
	h.factory = NULL;
	h.viewable = NULL;
	data[id] = h;
}

std::set<ValueID> HistoryManager::GetHistory(ID id)
{
	UL_ASSERT(data.find(id) != data.end());
	return data[id].history;
}

bool HistoryManager::IsVisible(ID id)
{
	UL_ASSERT(data.find(id) != data.end());
	return data[id].visible;
}

void HistoryManager::SetVisible(ID id, bool visible)
{
	UL_ASSERT(data.find(id) != data.end());
	data[id].visible = visible;
}

void HistoryManager::SetHistory(ID id, std::set<ValueID> history)
{
	UL_ASSERT(data.find(id) != data.end());
	data[id].history = history;
	data[id].modifiedTime = time;
}

void HistoryManager::ResetHistory(ID id)
{
	ValueID vid(id, time);
	std::set<ValueID> resetHistory;
	resetHistory.insert(vid);
	SetHistory(id, resetHistory);
}

void HistoryManager::SetValue(ID id, std::string value)
{
	UL_ASSERT(data.find(id) != data.end());
	data[id].value = value;
}

std::string HistoryManager::GetValue(ID id)
{
	UL_ASSERT(data.find(id) != data.end());
	return data[id].value;
}

void HistoryManager::SetFactory(ID id, ViewableObjectFactory* factory)
{
	UL_ASSERT(data.find(id) != data.end());
	data[id].factory = factory;
}

ViewableObjectFactory* HistoryManager::GetFactory(ID id)
{
	UL_ASSERT(data.find(id) != data.end());
	UL_ASSERT(data[id].factory != NULL);
	return data[id].factory;
}

int HistoryManager::GetModifiedTime(ID id)
{
	UL_ASSERT(data.find(id) != data.end());
	return data[id].modifiedTime;
}

int HistoryManager::GetTime()
{
	return time; 
}

void HistoryManager::ActionProcessed() 
{
	++time;
}

}