#ifndef DS_ACTION_H
#define DS_ACTION_H

#include "Action.h"
#include <set>
#include "SFML/Graphics.hpp"

namespace Algovis_Viewer
{
	class ViewableObject;
	class VO_SinglePrintable;

	struct ValueID
	{
		ValueID(const void* address, int time) : address(address), time(time) {}
		bool operator<(const ValueID& rhs) const
		{
			if (time == rhs.time)
				return address < rhs.address;
			return time < rhs.time;
		}
		const void* address;
		int time;
	};

	//enum DS_ActionType { DAT_Insert, DAT_Erase, DAT_Assign, DAT_BeingDestroyed };

	class DS_Action : public Action
	{
	protected:
		//DS_ActionType actionType;
		std::set<ViewableObject*> subjects;
		//std::string value;
		//std::set<ValueID> history;

	public:
		DS_Action(World* world) : Action(world) {}
		DS_Action(World* world, std::set<ViewableObject*> subjects) : Action(world), subjects(subjects) {}
		/*DS_Action(DS_ActionType type, ViewableObject* subject, std::string value, std::set<ValueID> history)
			: actionType(type), subject(subject), value(value), history(history)
		{ }*/

		std::set<ViewableObject*> GetSubjects() { return subjects; }

		virtual Action* Clone() const;
	};

	// Action class for printable being assigned 
	class DS_Assigned : public DS_Action
	{
	protected:
		std::string value;
		VO_SinglePrintable* subject;
		std::set<ValueID> history;

		// Animation stuff
		sf::FloatRect subjectStart;

	public:
		DS_Assigned(World* world) : DS_Action(world) {}
		DS_Assigned(World* world, VO_SinglePrintable* subject, std::set<ValueID> history, std::string value);
		virtual Action* Clone() const;

		virtual void Perform(float progress);
		virtual void Complete(bool displayed);
	};


	class DS_TestAction : public DS_Action
	{
	public:
		DS_TestAction(World* world) : DS_Action(world) {}

		virtual Action* Clone() const;
		virtual void Complete(bool displayed);
	};
}

#endif //DS_ACTION_H
