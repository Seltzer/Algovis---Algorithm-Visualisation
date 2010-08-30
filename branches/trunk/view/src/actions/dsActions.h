#ifndef DS_ACTIONS_H
#define DS_ACTIONS_H

#include <set>
#include <QRect>
#include <QColor>
#include "utilities.h"
#include "action.h"
#include "../../include/common.h"



// Contains ValueID struct and DS_Action, DS_Deleted and DS_CompositeAction classes.
namespace Algovis_Viewer
{
	class ViewableObject;
	class VO_SinglePrintable;
	class VO_Array;

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

	// Handy struct to represent where data for an action came from
	struct SourceData {
		QRect dimensions;
		VO_SinglePrintable* source;
		bool isSibling;
	};

	// Set up source data for animation using current state of world
	SourceData ValueIDToSourceData(ValueID id, ViewableObject* subject);

	// Same as above but for entire history
	std::vector<SourceData> historyToSources(const std::set<ValueID>& history, ViewableObject* subject);

	//enum DS_ActionType { DAT_Insert, DAT_Erase, DAT_Assign, DAT_BeingDestroyed };

	class DS_Action : public Action
	{
	protected:
		std::set<ViewableObject*> subjects;
		//DS_ActionType actionType;
		//std::string value;
		//std::set<ValueID> history;

	public:
		DS_Action(World*, bool animationSuppressed = false);
		DS_Action(World* world, std::set<ViewableObject*> subjects, bool suppressAnimation = false);
		DS_Action(const DS_Action&);
		/*DS_Action(DS_ActionType type, ViewableObject* subject, std::string value, std::set<ValueID> history)
			: actionType(type), subject(subject), value(value), history(history)
		{ }*/

		std::set<ViewableObject*> GetSubjects() { return subjects; }

		virtual Action* Clone() const;
	};



	// Action class for deleting a VO (TODO add deletion animation?)
	class DS_Deleted : public DS_Action
	{
	public:
		DS_Deleted(World*, ID dsSubject);
		DS_Deleted(const DS_Deleted&);
		virtual Action* Clone() const;

		virtual void Complete(bool displayed);

	private:
		const ID dsSubject;
	};

	
	class DS_CompositeAction : public DS_Action
	{
	public:
		DS_CompositeAction(World* world);
		DS_CompositeAction(const DS_CompositeAction&);
		~DS_CompositeAction();
		virtual Action* Clone() const;

		void PrepareToPerform();
		void Perform(float progress, QPainter* painter);
		virtual void Complete(bool displayed);

		void AddAction(DS_Action* action);

	private:
		std::vector<DS_Action*> subActions; // In order of excecution
	};

}

#endif
