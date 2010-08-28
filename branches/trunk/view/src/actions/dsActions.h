#ifndef DS_ACTIONS_H
#define DS_ACTIONS_H

#include <set>
#include <QRect>
#include "utilities.h"
#include "action.h"
#include "../../include/common.h"



// Contains ValueID struct and DS_Action, DS_CreateArray, DS_CreateSP, DS_AddressChanged, DS_Deleted,
//							   DS_Assigned, DS_Modified and DS_AddElementToArray classes.
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



	class DS_CreateArray : public DS_Action
	{
	public:
		DS_CreateArray(World*, ID arrayId, const void* arrayAddress, 
					ViewableObjectType elementType, std::vector<ID> elements);
		DS_CreateArray(const DS_CreateArray&);
		virtual Action* Clone() const;

		virtual void Complete(bool displayed);

	private:
		const ID arrayId;
		const void* arrayAddress;
		ViewableObjectType elementType;
		std::vector<ID> elements;
	};




	class DS_CreateSP : public DS_Action
	{
	public:
		DS_CreateSP(World*, const ID id, const void* dsAddress, const std::string& value);
		DS_CreateSP(const DS_CreateSP&);
		virtual Action* Clone() const;

		virtual void Complete(bool displayed);

	private:
		const ID id;
		const void* dsAddress;
		std::string value;
	};


	class DS_AddressChanged : public DS_Action
	{
	public:
		DS_AddressChanged(World*, const ID id, const void* newAddress, const void* oldAddress);
		DS_AddressChanged(const DS_AddressChanged&);

		virtual Action* Clone() const;
		virtual void Complete(bool displayed);

	private:
		const ID id;
		const void* newAddress;
		const void* oldAddress;
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



	// Action class for printable being assigned 
	class DS_Assigned : public DS_Action
	{
	public:
		DS_Assigned(World* world, ID dsAssigned, ID dsSource, std::string value, bool tracked);
		DS_Assigned(const DS_Assigned& other);
		virtual Action* Clone() const;

		void SetSource(VO_SinglePrintable* source);

		virtual void PrepareToPerform();
		virtual void Perform(float progress, QPainter*);
		virtual void Complete(bool displayed);

	protected:
		ID dsAssigned;
		ID dsSource;

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

	// Action class for printable being modified. Depressingly similar to assigned (Nathan LOL'd at this)
	class DS_Modified : public DS_Action
	{
	public:
		DS_Modified(World* world, ID dsModified, ID dsSource, std::string value, bool tracked);
		DS_Modified(const DS_Modified& other);
		virtual Action* Clone() const;

		void SetSource(VO_SinglePrintable* source);

		virtual void PrepareToPerform();
		virtual void Perform(float progress, QPainter*);
		virtual void Complete(bool displayed);

	protected:
		ID dsModified, dsSource;

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


	// TODO implement animation?
	class DS_AddElementToArray : public DS_Action
	{
	public:
		DS_AddElementToArray(World*, ID dsArray, ID dsElement, unsigned position);
		DS_AddElementToArray(const DS_AddElementToArray&);
		virtual Action* Clone() const;

		virtual void Complete(bool displayed);

	private:
		ID dsArray, dsElement;
		unsigned position;
	};
	

}

#endif
