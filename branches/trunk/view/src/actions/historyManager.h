#ifndef HISTORY_MANAGER_H
#define HISTORY_MANAGER_H

#include <set>
#include <map>
#include "../../include/common.h"

namespace Algovis_Viewer
{

	struct ValueID
	{
		ValueID(ID id, int time) : id(id), time(time) {}
		bool operator<(const ValueID& rhs) const
		{
			if (time == rhs.time)
				return id < rhs.id;
			return time < rhs.time;
		}
		ID id;
		int time;
	};

	struct HistoryData
	{
		bool visible;
		std::set<ValueID> history;
		std::string value; // Well... It seemed neccessary
	};

	class HistoryManager
	{
	public:
		HistoryManager();
		void AddRecord(ID id);
		std::set<ValueID> GetHistory(ID id);
		bool IsVisible(ID id);
		void SetVisible(ID id, bool visible);
		void SetHistory(ID id, std::set<ValueID> history); // Separate functions because generally you want to change them separately
		void ResetHistory(ID id);
		void SetValue(ID id, std::string value);
		std::string GetValue(ID id);

		int GetTime();
		void ActionProcessed();
	private:
		std::map<ID, HistoryData> data;
		int time;
	};

}

#endif // HISTORY_MANAGER_H
