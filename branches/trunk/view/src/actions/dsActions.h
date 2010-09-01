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

	// Handy struct to represent a source in an animation
	struct SourceID {
		SourceID(ValueID vid, bool locationKnown)
			: vid(vid), locationKnown(locationKnown) {}
		ValueID vid;
		bool locationKnown;
	};

	// Handy struct to represent data needed to animate a source
	struct SourceData {
		QRect dimensions;
		VO_SinglePrintable* source;
		bool isSibling;
	};

	SourceID ValueIDToSourceID(ValueID id, HistoryManager& manager);
	std::vector<SourceID> HistoryToSourceIDs(const std::set<ValueID>& history, HistoryManager& manager);

	// Set up source data for animation using current state of world
	SourceData SourceIDToSourceData(SourceID id, ViewableObject* subject);

	// Lightweight method for converting a list of IDs to VO pointers
	// 
	// Will throw an assertion if, for any specified ID, there isn't a Viewable with that ID.
	std::vector<ViewableObject*> ConvertIdsToViewablePtrs(const std::vector<ID>&);

	// Overload of the above which throws an assertion if any of the Viewables specified by ID aren't 
	// of the specified type
	std::vector<ViewableObject*> ConvertIdsToViewablePtrs(const std::vector<ID>&, ViewableObjectType expectedType);
				

	// Same as above but for entire history
	std::vector<SourceData> SourceIDsToSources(const std::vector<SourceID>& sourceIDs, ViewableObject* subject);

	//enum DS_ActionType { DAT_Insert, DAT_Erase, DAT_Assign, DAT_BeingDestroyed };

	class DS_Action : public Action
	{
	protected:
		//std::set<ViewableObject*> subjects;
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

		virtual void UpdateHistory(HistoryManager& historyManager);

		virtual Action* Clone() const;
	};


	class DS_DataFlowAction : public DS_Action
	{
	public:
		DS_DataFlowAction(World* world, bool animationSuppressed = false)
			: DS_Action(world, animationSuppressed) {}
		virtual std::set<ID> GetSubjects() { return std::set<ID>(); }
		virtual std::vector<SourceID> GetSources() { return std::vector<SourceID>(); }
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

		void AddAction(Action* action);

	private:
		std::vector<Action*> subActions; // In order of excecution
	};

}

#endif
