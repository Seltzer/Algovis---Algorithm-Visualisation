#ifndef DS_ACTIONS_H
#define DS_ACTIONS_H

#include <set>
#include <QRect>
#include <QColor>
#include "utilities.h"
#include "action.h"
#include "../../include/common.h"


// Contains ValueID struct and DS_Action, DS_CreateAction, DS_Deleted and DS_CompositeAction classes.
namespace Algovis_Viewer
{
	class ViewableObject;
	class VO_SinglePrintable;
	class VO_Array;
	class ViewableObjectFactory;

	// Handy struct to represent data needed to animate a source
	struct SourceData 
	{
		SourceData(ValueID id) 
			: valueId(id) 
		{
		}
		
		ValueID valueId;
		QRect dimensions;
		VO_SinglePrintable* source;
		bool isSibling;
	};

	// Set up source data for animation using current state of world
	SourceData ValueIDToSourceData(ValueID id, ViewableObject* subject);

	// For updating sourceData
	// This is a bit of a hacky method so DON'T use it unless you know exactly how it works
	// Currently used for updating source data which was calculated earlier on while backtracking
	std::vector<SourceData> UpdateSources(std::vector<SourceData>& sourceData);


	// Lightweight method for converting a list of IDs to VO pointers
	// 
	// Will throw an assertion if, for any specified ID, there isn't a Viewable with that ID.
	std::vector<ViewableObject*> ConvertIdsToViewablePtrs(const std::vector<ID>&);

	// Overload of the above which throws an assertion if any of the Viewables specified by ID aren't 
	// of the specified type
	std::vector<ViewableObject*> ConvertIdsToViewablePtrs(const std::vector<ID>&, ViewableObjectType expectedType);
				

	// Same as above but for entire history
	std::vector<SourceData> HistoryToSources(const std::set<ValueID>& history, ViewableObject* subject);


	class DS_Action : public Action
	{

	public:
		DS_Action(World*, bool animationSuppressed = false);
		DS_Action(World*, std::set<ViewableObject*> subjects, bool suppressAnimation = false);
		DS_Action(const DS_Action&);

	protected:
		virtual Action* Clone() const;
		virtual void UpdateHistory(HistoryManager& historyManager);

		int completeTime;
	};


	class DS_CreateAction : public DS_Action
	{

	public:
		DS_CreateAction(World*, bool animationSuppressed, bool createOnSameLine = false);
		DS_CreateAction(World*, std::set<ViewableObject*> subjects, bool animationSuppressed, 
							bool createOnSameLine = false);
		DS_CreateAction(const DS_CreateAction&);

		void PlaceOnSameLine();
		bool BeingCreatedOnSameLine();
		void CreateAndDisplayASAP();

	protected:
		ViewableObjectFactory* myFactory;

		bool createOnSameLine;
		bool createAndDisplayASAP;
	};


	class DS_DataFlowAction : public DS_Action
	{
	protected:
		std::set<ValueID> history; // They all need it
	public:
		DS_DataFlowAction(World* world, bool animationSuppressed = false)
			: DS_Action(world, animationSuppressed) {}
		DS_DataFlowAction(const DS_DataFlowAction& other)
			: DS_Action(other), history(other.history) {}
		virtual std::set<ID> GetSubjects() { return std::set<ID>(); }
		virtual std::set<ValueID> GetSources() { return history; }
	};


	// Action class for deleting a VO
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

		virtual void PrepareToPerform();
		virtual void PrepareToUnperform();
		virtual void Perform(float progress, QPainter* painter);
		virtual void Unperform(float progress, QPainter* painter);
		virtual void Complete(bool displayed);
		virtual void Uncomplete(bool displayed);

		void AddAction(Action* action);

	private:
		std::vector<Action*> subActions; // In order of excecution
	};


	class DS_SetCaption : public DS_Action
	{

	public:
		DS_SetCaption(World*, const std::string&);
		DS_SetCaption(const DS_SetCaption&);
		virtual Action* Clone() const;
		virtual void Complete(bool displayed);

	private:
		std::string newCaption;
	};

	class DS_EnsureDisplayed : public DS_CreateAction
	{
	public:
		DS_EnsureDisplayed(World*, bool animationSuppressed, ID id);
		DS_EnsureDisplayed(const DS_EnsureDisplayed&);
		virtual Action* Clone() const;

		// Note, this action doesn't actually update the history
		virtual void UpdateHistory(HistoryManager&);
		virtual void PrepareToPerform();
		virtual void Complete(bool displayed);
	
	private:
		ID id;
		ViewableObjectFactory* voFactory;
	};

	

}

#endif
