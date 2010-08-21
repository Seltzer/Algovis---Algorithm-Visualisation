#ifndef DS_ACTION_H
#define DS_ACTION_H

#include <set>
#include <QRect>
#include "utilities.h"
#include "action.h"
#include "../include/common.h"




// Contains ValueID struct and DSAction, DS_AddElementToArray, DS_Assigned, DS_CreateArray, DS_CreateSP
// and DS_Deleted classes
namespace Algovis_Viewer
{
	class ViewableObject;
	class VO_SinglePrintable;
	class VO_Array;


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
		DS_Action(World*, bool animationSuppressed = false);
		DS_Action(World* world, std::set<ViewableObject*> subjects, bool suppressAnimation = false);
		DS_Action(const DS_Action&);
		/*DS_Action(DS_ActionType type, ViewableObject* subject, std::string value, std::set<ValueID> history)
			: actionType(type), subject(subject), value(value), history(history)
		{ }*/

		std::set<ViewableObject*> GetSubjects() { return subjects; }

		virtual Action* Clone() const;
	};



	// Action class for printable being assigned 
	class DS_Assigned : public DS_Action
	{
	public:
		DS_Assigned(World* world);
		DS_Assigned(World* world, VO_SinglePrintable* subject, 
						std::set<ValueID> history, std::string value, bool tracked);
		DS_Assigned(const DS_Assigned& other);
		virtual Action* Clone() const;

		void SetSource(VO_SinglePrintable* source);

		virtual void PrepareToPerform();
		virtual void Perform(float progress, QPainter*);
		virtual void Complete(bool displayed);

	protected:
		std::string value;
		VO_SinglePrintable* subject;
		std::set<ValueID> history;
		bool tracked;

		// Animation stuff
		QRect subjectDimensions;
		QRect sourceDimensions;
		VO_SinglePrintable* source;
		bool sourceIsSibling;
	};


	// Action class for deleting a VO (hacky atm)
	class DS_Deleted : public DS_Action
	{
	public:
		DS_Deleted(World*);
		DS_Deleted(World*, ViewableObject* subject);
		DS_Deleted(const DS_Deleted&);
		virtual Action* Clone() const;

		//virtual void PrepareToPerform();
		//virtual void Perform(float progress, QPainter*);
		virtual void Complete(bool displayed);

	private:
		ViewableObject* subject;

	};




	class DS_CreateArray : public DS_Action
	{
	public:
		DS_CreateArray(World*, const void* dsArrayAddress, ViewableObjectType elementType, 
				std::vector<ViewableObject*> elements);
		DS_CreateArray(const DS_CreateArray&);
		virtual Action* Clone() const;

		virtual void Complete(bool displayed);

	private:
		const void* dsArrayAddress;
		ViewableObjectType elementType;
		std::vector<ViewableObject*> elements;
	};




	class DS_CreateSP : public DS_Action
	{
	public:
		DS_CreateSP(World*, const void* dsAddress, const std::string& value);
		DS_CreateSP(const DS_CreateSP&);
		virtual Action* Clone() const;

		virtual void Complete(bool displayed);

	private:
		const void* dsAddress;
		std::string value;
	};


	// TODO implement animation?
	class DS_AddElementToArray : public DS_Action
	{
	public:
		DS_AddElementToArray(World*, VO_Array* voArray, ViewableObject* element, unsigned position);
		DS_AddElementToArray(const DS_AddElementToArray&);
		virtual Action* Clone() const;

		virtual void Complete(bool displayed);

	private:
		VO_Array* voArray;
		ViewableObject* element;
		unsigned position;
	};


		
	class DS_ArrayResize : public DS_Action
	{
	public:
		DS_ArrayResize(World*, VO_Array* voArray, std::vector<ViewableObject*> elementsToAdd, unsigned newCapacity);
		DS_ArrayResize(const DS_ArrayResize&);
		virtual Action* Clone() const;

		virtual void Complete(bool displayed);

	private:
		VO_Array* voArray;
		std::vector<ViewableObject*> elementsToAdd;
		unsigned newCapacity;
	};

}

#endif
