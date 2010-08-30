
#include "dsArrayActions.h"

#include <iostream>
#include <cmath>
#include "boost/foreach.hpp"
#include <QPainter>
#include <QColor>
#include "dsactions.h"
#include "../../include/registry.h"
#include "../displayer/world.h"
#include "../viewableObjects/vo_singlePrintable.h"
#include "../viewableObjects/vo_array.h"

using namespace std;

namespace Algovis_Viewer
{

//////////////// DS_CreateArray
DS_CreateArray::DS_CreateArray(World* world, ID arrayId, const void* arrayAddress, 
								ViewableObjectType elementType, std::vector<ID> elements)
		: DS_Action(world, true), arrayId(arrayId), arrayAddress(arrayAddress), 
				elementType(elementType), elements(elements)
{
}

DS_CreateArray::DS_CreateArray(const DS_CreateArray& other)
	: DS_Action(other), arrayId(other.arrayId),arrayAddress(other.arrayAddress), 
		elementType(other.elementType), elements(other.elements)
{
}

Action* DS_CreateArray::Clone() const
{
	return new DS_CreateArray(*this);
}

void DS_CreateArray::Complete(bool displayed)
{
	Registry* registry = Registry::GetInstance();

	// Verify that array hasn't already been registered
	UL_ASSERT(!registry->IsRegistered(arrayId));


	// ViewableObject equivalents of elements
	vector<ViewableObject*> arrayElements;

	// Iterate over elements, verify that they are all registered and populate arrayElements
	BOOST_FOREACH(ID dsElement, elements)
	{
		// TODO: change behaviour when above registration condition is violated (i.e. throw exception)
		UL_ASSERT(registry->IsRegistered(dsElement));
		arrayElements.push_back(registry->GetRepresentation(dsElement));
	}


	VO_Array* newArray = new VO_Array(arrayId, arrayAddress, world, elementType, arrayElements);
	

	newArray->move(world->GetArrayPosition());
	newArray->adjustSize();
	// TODO assuming top level
	newArray->setParent(world);
	newArray->setVisible(true);

	// TODO hack
	world->adjustSize();

	Registry::GetInstance()->Register(arrayId, newArray);
}

//////////////// DS_AddElementToArray
DS_AddElementToArray::DS_AddElementToArray
		(World* world, ID dsArray, ID dsElement, unsigned position)
			: DS_Action(world, false), dsArray(dsArray), dsElement(dsElement), position(position)
{
}

DS_AddElementToArray::DS_AddElementToArray(const DS_AddElementToArray& other)
		: DS_Action(other), dsArray(other.dsArray), dsElement(other.dsElement), position(other.position)
{
}

Action* DS_AddElementToArray::Clone() const
{
	return new DS_AddElementToArray(*this);
}

void DS_AddElementToArray::PrepareToPerform()
{
	Registry* registry = Registry::GetInstance();

	UL_ASSERT(registry->IsRegistered(dsArray,ARRAY));
	UL_ASSERT(registry->IsRegistered(dsElement,SINGLE_PRINTABLE));

	voArray = registry->GetRepresentation<VO_Array>(dsArray);
	UL_ASSERT(position <= voArray->GetSize());

	element = registry->GetRepresentation<VO_SinglePrintable>(dsElement);

	history = element->GetHistory();

	// TODO: Do not assume array is shown (and therefore this is not suppressed)

	// Set subjectStart to have abs position
	QRect arrayGeom = QRect(voArray->GetPositionInWorld(), voArray->size());
	subjectDimensions = QRect(arrayGeom.topRight(), element->size());

	// Set up data for all the sources
	sources = historyToSources(history, element);
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
			source.source->DrawValue(QRect(QPoint(x,y),QSize(source.dimensions.width(), source.dimensions.height())),painter);
		}
	}
}

void DS_AddElementToArray::Complete(bool displayed)
{
	Registry* registry = Registry::GetInstance();

	voArray->AddElement(element, position);

	if (displayed)
	{
		int time = Registry::GetInstance()->CurrentTime();
		element->ResetHistory(ValueID(element->GetId(), time));
	}
}

//////////////// DS_AddressChanged
DS_AddressChanged::DS_AddressChanged(World* world, const ID id, const void* newAddress, const void* oldAddress)
	: DS_Action(world, true), id(id), newAddress(newAddress), oldAddress(oldAddress)
{
}

DS_AddressChanged::DS_AddressChanged(const DS_AddressChanged& other)
	: DS_Action(other), id(other.id), newAddress(other.newAddress), oldAddress(other.oldAddress)
{
}


Action* DS_AddressChanged::Clone() const
{
	return new DS_AddressChanged(*this);
}


void DS_AddressChanged::Complete(bool displayed)
{
	ViewableObject* viewable = Registry::GetInstance()->GetRepresentation(id);
	UL_ASSERT(viewable);

	viewable->SetDSAddress(newAddress);
}


}