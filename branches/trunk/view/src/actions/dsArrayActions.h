#ifndef DS_ARRAY_ACTIONS_H
#define DS_ARRAY_ACTIONS_H

#include "dsActions.h"


// Contains ValueID struct and DS_CreateArray, DS_AddressChanged and DS_AddElementToArray classes.
namespace Algovis_Viewer
{

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

	// TODO implement animation?
	class DS_AddElementToArray : public DS_DataFlowAction
	{
	public:
		DS_AddElementToArray(World*, ID dsArray, ID dsElement, unsigned position);
		DS_AddElementToArray(const DS_AddElementToArray&);
		virtual Action* Clone() const;

		virtual std::set<ID> GetSubjects() { std::set<ID> r; r.insert(dsElement); return r; }
		virtual std::vector<SourceID> GetSources() { return sourceIDs; }

		virtual void UpdateHistory(HistoryManager& historyManager);

		void PrepareToPerform();
		void Perform(float progress, QPainter* painter);
		virtual void Complete(bool displayed);

	private:
		ID dsArray, dsElement;
		
		// Insert elements before this position
		unsigned position;

		VO_Array* voArray;
		VO_SinglePrintable* element;
		std::vector<SourceID> sourceIDs;
		std::string value;

		QRect subjectDimensions;
		std::vector<SourceData> sources;
	};

	class DS_RemoveElementsFromArray : public DS_DataFlowAction
	{
	public:
		DS_RemoveElementsFromArray(World*, const ID arrayId, const std::vector<ID>& elements, 
										unsigned startIndex, unsigned endIndex);
		DS_RemoveElementsFromArray(const DS_RemoveElementsFromArray&);
		virtual Action* Clone() const;

		virtual std::set<ID> GetSubjects() { return std::set<ID>(elements.begin(), elements.end()); }

		virtual void PrepareToPerform();
		virtual void Complete(bool displayed);
		
	private:
		const ID arrayId;
		const std::vector<ID> elements;
		
		VO_Array* dsArray;
		
		const unsigned startIndex, endIndex;
	};




	class DS_AddressChanged : public DS_Action
	{
	public:
		DS_AddressChanged(World*, const ID id, const void* newAddress);
		DS_AddressChanged(const DS_AddressChanged&);

		virtual Action* Clone() const;
		virtual void Complete(bool displayed);

	private:
		const ID id;
		const void* newAddress;
	};

}

#endif // DS_ARRAY_ACTIONS_H
