#include <QApplication>
#include <QPainter>
#include <QColor>
#include "boost/foreach.hpp"

#include "dsArrayActions.h"
#include "dsactions.h"
#include "../../include/registry.h"
#include "../displayer/world.h"
#include "../viewableObjects/vo_array.h"
#include "../viewableObjects/vo_singlePrintable.h"

using namespace std;




namespace Algovis_Viewer
{



//////////////// DS_CreateArray
DS_CreateArray::DS_CreateArray(World* world, ID arrayId, const void* arrayAddress, 
								ViewableObjectType elementType, vector<ID> elements)
		: DS_CreateAction(world, false), arrayId(arrayId), arrayAddress(arrayAddress), 
				elementType(elementType), voArray(NULL), elements(elements)
{
}

DS_CreateArray::DS_CreateArray(const DS_CreateArray& other)
	: DS_CreateAction(other), arrayId(other.arrayId),arrayAddress(other.arrayAddress), 
		elementType(other.elementType), voArray(other.voArray), elements(other.elements)
{
}

Action* DS_CreateArray::Clone() const
{
	return new DS_CreateArray(*this);
}

void DS_CreateArray::Complete(bool displayed)
{
	// Verify that array hasn't already been registered
	UL_ASSERT(!reg->IsRegistered(arrayId));


	if (!completedAtLeastOnce)
	{
		// Iterate over elements, verify that they are all registered and populate arrayElements
		vector<ViewableObject*> arrayElements;

		BOOST_FOREACH(ID dsElement, elements)
		{
			UL_ASSERT(reg->IsRegistered(dsElement));
			arrayElements.push_back(reg->GetRepresentation(dsElement));
		}

		// Instantiate newArray
		voArray = new VO_Array(arrayId, arrayAddress, world, elementType, arrayElements);
	}
	
	if (BeingCreatedOnSameLine())
		world->AddViewableOnSameRow(voArray);
	else
		world->AddViewableOnNewRow(voArray);
	
	
	voArray->adjustSize();
	voArray->setVisible(true);

	world->adjustSize();
	reg->Register(arrayId, voArray);
	
	Action::Complete(displayed);
}


void DS_CreateArray::Uncomplete(bool displayed)
{
	if (!completedAtLeastOnce)
		return;

	//UL_ASSERT(reg->IsRegistered(arrayId));
	//VO_Array* voArray = reg->GetRepresentation<VO_Array>(arrayId);
	
	// TODO actually delete voArray??? nahhh

	voArray->setVisible(false);
	world->RemoveViewable(voArray);	
	world->adjustSize();
	reg->Deregister(arrayId);

	Action::Uncomplete(displayed);
}








//////////////// DS_AddElementToArray
DS_AddElementToArray::DS_AddElementToArray
		(World* world, ID dsArray, ID dsElement, unsigned position)
			: DS_DataFlowAction(world, false), dsArray(dsArray), dsElement(dsElement), position(position),
				voArray(NULL), elementFactory(NULL), element(NULL)
{
}

DS_AddElementToArray::DS_AddElementToArray(const DS_AddElementToArray& other)
		: DS_DataFlowAction(other), dsArray(other.dsArray), dsElement(other.dsElement), position(other.position),
		value(other.value), voArray(other.voArray), elementFactory(other.elementFactory), element(other.element)
{
}

Action* DS_AddElementToArray::Clone() const
{
	return new DS_AddElementToArray(*this);
}

void DS_AddElementToArray::UpdateHistory(HistoryManager& historyManager)
{
	history = historyManager.GetHistory(dsElement);

	// TODO: This assumes all arrays are displayed.
	suppressAnimation = false;
	historyManager.SetVisible(dsElement, true);
	historyManager.ResetHistory(dsElement);
	value = historyManager.GetValue(dsElement);
	elementFactory = historyManager.GetFactory(dsElement);

	DS_Action::UpdateHistory(historyManager);
}

void DS_AddElementToArray::PrepareToPerform()
{
	UL_ASSERT(reg->IsRegistered(dsArray,ARRAY));
	voArray = reg->GetRepresentation<VO_Array>(dsArray);


	// Create a viewable for whatever element was added.
	if (!element)
		element = elementFactory->Create(); 

	
	// TODO: Do not assume array is shown (and therefore this is not suppressed)

	// Set subjectStart to have abs position
	QRect arrayGeom = QRect(voArray->GetPositionInWorld(), voArray->size());
	subjectDimensions = QRect(arrayGeom.topRight(), element->size());


	// Set up data for all the sources
	if (!completedAtLeastOnce)
		sources = HistoryToSources(history, element);
	else
		sources = UpdateSources(sources);

	Action::PrepareToPerform();
}


// See PrepareToPerform for TODOs/comments etc.
void DS_AddElementToArray::PrepareToUnperform()
{
	UL_ASSERT(reg->IsRegistered(dsArray,ARRAY));
	voArray = reg->GetRepresentation<VO_Array>(dsArray);

	
	//UL_ASSERT(reg->IsRegistered(dsElement));
	if (reg->IsRegistered(dsElement))
	{
		element = reg->GetRepresentation(dsElement);
		UL_ASSERT(element);

		// Set subjectStart to have abs position
		QRect arrayGeom = QRect(voArray->GetPositionInWorld(), voArray->size());
		subjectDimensions = QRect(arrayGeom.topRight(), element->size());
	}

	// Update, since dimensions and pointers may have changed
	sources = UpdateSources(sources);

	Action::PrepareToUnperform();
}

void DS_AddElementToArray::Perform(float progress, QPainter* painter)
{
	//if (source == NULL)
	//	return;

	float x,y;

	float lerp = 1 - (cos(progress * 3.14159265358) + 1) / 2;
	
	for (unsigned i = 0; i < sources.size(); i++)
	{
		SourceData& source = sources[i];
		if (source.source != NULL)
		{
			if (source.isSibling)
			{
				// Assume source and subject have the same y for now
				x = source.dimensions.x() + lerp * (subjectDimensions.x() - source.dimensions.x());
				//y = sourceDimensions.y() - 5 * sin(3.14 * progress) * (subjectStart.y() - sourceDimensions.y());
				y = source.dimensions.y() - 30 * sin(3.14 * progress);
			}
			else
			{
				x = source.dimensions.x() + lerp * (subjectDimensions.x() - source.dimensions.x());
				y = source.dimensions.y() + lerp * (subjectDimensions.y() - source.dimensions.y());
			}

			painter->setPen(QColor(Qt::white));
			source.source->DrawValue(false, QRect(QPoint(x,y),QSize(source.dimensions.width(), source.dimensions.height())),painter);
		}
	}
}

void DS_AddElementToArray::Unperform(float progress, QPainter* painter)
{
	Perform(1.0f - progress, painter);
}

void DS_AddElementToArray::Complete(bool displayed)
{
	// TODO: Do not assume array is shown, and therefore element is shown.
	//UL_ASSERT(!reg->IsRegistered(dsElement));
	//reg->Register(dsElement, element);
	if (!reg->IsRegistered(dsElement))
	{
		reg->Register(dsElement, element);
	}

	UL_ASSERT(reg->IsRegistered(dsArray,ARRAY));
	voArray = reg->GetRepresentation<VO_Array>(dsArray);

	voArray->AddElement(element, position);

	Action::Complete(displayed);

	/*if (displayed)
	{
		int time = reg->CurrentTime();
		element->ResetHistory(ValueID(element->GetId(), time));
	}*/
}

// See Complete for TODOs/comments etc.
void DS_AddElementToArray::Uncomplete(bool displayed)
{
	// TODO - Deregister?

	// Remove element from voArray
	vector<ViewableObject*> elementsToRemove;
	elementsToRemove.push_back(element);
	UL_ASSERT(reg->IsRegistered(dsArray,ARRAY));
	voArray = reg->GetRepresentation<VO_Array>(dsArray);

	voArray->RemoveElements(elementsToRemove, position, position);

	Action::Uncomplete(displayed);
}


//////////////// DS_RemoveElementsFromArray
DS_RemoveElementsFromArray::DS_RemoveElementsFromArray(World* world, const ID arrayId, 
								const vector<ID>& elements, unsigned startIndex, unsigned endIndex)
						: DS_DataFlowAction(world), arrayId(arrayId), 
								elements(elements), startIndex(startIndex), endIndex(endIndex)
{
}

DS_RemoveElementsFromArray::DS_RemoveElementsFromArray(const DS_RemoveElementsFromArray& other)
	: DS_DataFlowAction(other), arrayId(other.arrayId), elements(other.elements), dsArray(other.dsArray),
		startIndex(other.startIndex), endIndex(other.endIndex)
{
}

Action* DS_RemoveElementsFromArray::Clone() const
{
	return new DS_RemoveElementsFromArray(*this);
}

void DS_RemoveElementsFromArray::PrepareToPerform()
{
	UL_ASSERT(endIndex >= startIndex);
	UL_ASSERT(elements.size() == endIndex - startIndex + 1);

	dsArray = reg->GetRepresentation<VO_Array>(arrayId);
	UL_ASSERT(dsArray);

	// Don't grab pointers until we actually perform the action
	Action::PrepareToPerform();
}


void DS_RemoveElementsFromArray::Complete(bool displayed)
{
	vector<ViewableObject*> elementPtrs = ConvertIdsToViewablePtrs(elements,SINGLE_PRINTABLE);
	dsArray->RemoveElements(elementPtrs, startIndex, endIndex);

	Action::Complete(displayed);
}




//////////////// DS_AddressChanged
DS_AddressChanged::DS_AddressChanged(World* world, const ID id, const void* newAddress)
	: DS_Action(world, true), id(id), newAddress(newAddress)
{
}

DS_AddressChanged::DS_AddressChanged(const DS_AddressChanged& other)
	: DS_Action(other), id(other.id), newAddress(other.newAddress)
{
}


Action* DS_AddressChanged::Clone() const
{
	return new DS_AddressChanged(*this);
}


void DS_AddressChanged::Complete(bool displayed)
{
	UL_ASSERT(reg->IsRegistered(id));
	const void* oldAddress = reg->GetRepresentation(id)->GetDSAddress();

	ViewableObject* viewable = reg->GetRepresentation(id);
	UL_ASSERT(viewable);

	viewable->SetDSAddress(newAddress);
	Action::Complete(displayed);
}


}