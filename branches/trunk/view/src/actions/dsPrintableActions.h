#ifndef DS_PRINTABLE_ACTIONS_H
#define DS_PRINTABLE_ACTIONS_H

#include "dsActions.h"

// Contains ValueID struct and DS_CreateSP, DS_Assigned, DS_Modified and DS_HighlightOperands classes.
namespace Algovis_Viewer
{

	class DS_CreateSP : public DS_Action
	{
	public:
		DS_CreateSP(World*, const ID id, const void* dsAddress, const std::string& value);
		DS_CreateSP(const DS_CreateSP&);
		virtual Action* Clone() const;

		virtual void UpdateHistory(HistoryManager& historyManager);

		//virtual void Complete(bool displayed);

	private:
		const ID id;
		const void* dsAddress;
		std::string value;
	};


	// Action class for printable being assigned 
	class DS_Assigned : public DS_DataFlowAction
	{
	public:
		DS_Assigned(World* world, ID dsAssigned, ID dsSource, std::string value, bool tracked);
		DS_Assigned(const DS_Assigned& other);
		virtual Action* Clone() const;

		virtual std::set<ID> GetSubjects() { std::set<ID> r; r.insert(dsAssigned); return r; }

		virtual void UpdateHistory(HistoryManager& historyManager);

		virtual void PrepareToPerform();
		virtual void Perform(float progress, QPainter*);
		virtual void Complete(bool displayed);

	protected:
		ID dsAssigned;
		ID dsSource;

		std::string value;
		VO_SinglePrintable* subject;
		bool tracked;

		// Animation stuff
		QRect subjectDimensions;
		std::vector<SourceData> sources;
	};

	// Action class for printable being modified. Depressingly similar to assigned (Nathan LOL'd at this)
	class DS_Modified : public DS_DataFlowAction
	{
	public:
		DS_Modified(World* world, ID dsModified, ID dsSource, std::string value, bool tracked);
		DS_Modified(const DS_Modified& other);
		virtual Action* Clone() const;

		virtual std::set<ID> GetSubjects() { std::set<ID> r; r.insert(dsModified); return r; }

		virtual void UpdateHistory(HistoryManager& historyManager);

		virtual void PrepareToPerform();
		virtual void Perform(float progress, QPainter*);
		virtual void Complete(bool displayed);

	protected:
		ID dsModified, dsSource;

		std::string value;
		VO_SinglePrintable* subject;
		bool tracked;

		// Animation stuff
		QRect subjectDimensions;
		std::vector<SourceData> sources;
	};

	class DS_HighlightOperands : public DS_Action
	{

	public:
		DS_HighlightOperands(World*, std::vector<ID> operands);
		DS_HighlightOperands(const DS_HighlightOperands&);
		virtual Action* Clone() const;
		virtual void PrepareToPerform();
		virtual void Perform(float progress, QPainter*);
		virtual void Complete(bool displayed);

	private:
		std::vector<ID> operands;
		std::vector<VO_SinglePrintable*> operandPtrs;

		QColor originalBBColour;
	};

}

#endif // DS_PRINTABLE_ACTIONS_H
