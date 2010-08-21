#include <iostream>
#include <cmath>
#include "boost/foreach.hpp"
#include "dsaction.h"
#include "viewableObjects/vo_singlePrintable.h"
#include "viewableObjects/vo_array.h"
#include "../include/registry.h"
#include "displayer/world.h"

using namespace std;


namespace Algovis_Viewer
{

int time = 0; // TODO: Seriously?... Seriously guys.



////////////////////// DS_Action implementation ////////////////////////////

DS_Action::DS_Action(World* world, bool suppressAnimation)
	: Action(world, suppressAnimation)
{
}


DS_Action::DS_Action(World* world, std::set<ViewableObject*> subjects, bool suppressAnimation)
	: Action(world, suppressAnimation), subjects(subjects)
{
}

DS_Action::DS_Action(const DS_Action& other)
	: Action(other), subjects(other.subjects)
{
	UL_ASSERT(world);
}


Action* DS_Action::Clone() const
{
	return new DS_Action(*this);
}



////////////////////// DS_Assigned implementation ////////////////////////////

DS_Assigned::DS_Assigned(World* world) 
	: DS_Action(world), sourceDimensions(0,0,0,0), source(NULL)
{
	UL_ASSERT(world);
}

DS_Assigned::DS_Assigned(World* world, VO_SinglePrintable* subject, std::set<ValueID> history, 
							std::string value, bool tracked)
	: DS_Action(world), value(value), subject(subject), history(history), tracked(tracked),
			sourceDimensions(0,0,0,0), source(NULL)
{
	ViewableObject* viewable = (ViewableObject*)subject;
	subjects.insert(viewable); // May need to keep the original printable pointer?
}

DS_Assigned::DS_Assigned(const DS_Assigned& other)
	: DS_Action(other), value(other.value), subject(other.subject), history(other.history), 
			sourceDimensions(other.sourceDimensions), source(other.source), sourceIsSibling(other.sourceIsSibling)
{
}


Action* DS_Assigned::Clone() const
{
	return new DS_Assigned(*this);
}

void DS_Assigned::SetSource(VO_SinglePrintable* source)
{
	this->source = source;
	sourceIsSibling = source->parent() == subject->parent();
}


void DS_Assigned::PrepareToPerform()
{
	//subject->EnableDrawing(false);

	// Set subjectStart to have abs position
	subjectDimensions = QRect(subject->GetPositionInWorld(), subject->size());

	if (source != NULL)
	{
		sourceDimensions = QRect(source->GetPositionInWorld(), source->size());
		
	//	source->EnableDrawing(false);
	}
		
}

void DS_Assigned::Perform(float progress, QPainter* painter)
{
	if (source == NULL)
		return;

	float x,y;
	
	if (sourceIsSibling)
	{
		// Assume source and subject have the same y for now
		x = sourceDimensions.x() + progress * (subjectDimensions.x() - sourceDimensions.x());
		//y = sourceDimensions.y() - 5 * sin(3.14 * progress) * (subjectStart.y() - sourceDimensions.y());
		y = sourceDimensions.y() - 30 * sin(3.14 * progress);
	}
	else
	{
		x = sourceDimensions.x() + progress * (subjectDimensions.x() - sourceDimensions.x());
		y = sourceDimensions.y() + progress * (subjectDimensions.y() - sourceDimensions.y());
	}

	source->DrawValue(QRect(QPoint(x,y),QSize(sourceDimensions.width(), sourceDimensions.height())),painter);
	//source->DrawWithoutValue(sourceDimensions, painter);
	//subject->DrawWithoutValue(subjectDimensions, painter);

	/* Bulging is commented out until DrawValue can draw to a specified dimension properly

	QRect updatedBounds = subjectStart;
	int bulge = int(progress * 100);

	updatedBounds.Left -= bulge;
	updatedBounds.Right += bulge;
	updatedBounds.Top -= bulge;
	updatedBounds.Bottom += bulge;*/
	
	// MIGRATION
	//subject->SetBoundingBox(updatedBounds);


	//subject->DrawWithoutValue(QRect(
	//subject->DrawWithoutValue(
	//subject->DrawWithoutValue(renderWindow, defaultFont);
	//subject->DrawValue(updatedBounds,renderWindow, defaultFont);


	/*for (std::set<ValueID>::iterator i = history.begin(); i != history.end(); i++)
	{
		Registry* reg = Registry::GetInstance();
		if (i->address != 
		if (reg->IsRegistered(i->address, SINGLE_PRINTABLE))
		{
			VO_SinglePrintable* printable = 
		}
	}*/
}

void DS_Assigned::Complete(bool displayed)
{
	subject->Assigned(history, value);
	
	if (displayed)
	{
		// As we have completed this animation and presumably displayed the history
		// and will reset the current history to be the value we just displayed
		// That way future things will have the just-displayed-element in their history, instead of everything
		// that was used to produce is.
		//int time = Algovis_Viewer::Registry::GetInstance()->GetTime();
		subject->ResetHistory(ValueID(subject->GetDSAddress(), time));

		++time;
		subject->EnableDrawing(true);
		if (source != NULL)
			source->EnableDrawing(true);
	}
	Registry::GetInstance()->ActionAgentCallback(NULL);
}




////////////////////// DS_Deleted implementation ////////////////////////////

DS_Deleted::DS_Deleted(World* world) 
	: DS_Action(world) 
{
}

DS_Deleted::DS_Deleted(World* world, ViewableObject* subject)
	: DS_Action(world), subject(subject)
{
	subjects.insert(subject); 
}


DS_Deleted::DS_Deleted(const DS_Deleted& other)
	: DS_Action(other), subject(other.subject)
{
}


Action* DS_Deleted::Clone() const
{
	return new DS_Deleted(*this);
}

void DS_Deleted::Complete(bool displayed)
{
	delete subject;
	Registry::GetInstance()->ActionAgentCallback(subject);
}




//////////////// DS_CreateArray
DS_CreateArray::DS_CreateArray(World* world, 
	const void* dsArrayAddress, ViewableObjectType elementType, std::vector<ViewableObject*> elements)
		: DS_Action(world, true), dsArrayAddress(dsArrayAddress), elementType(elementType), elements(elements)
{
}

DS_CreateArray::DS_CreateArray(const DS_CreateArray& other)
	: DS_Action(other), dsArrayAddress(other.dsArrayAddress), 
		elementType(other.elementType), elements(other.elements)
{
}

Action* DS_CreateArray::Clone() const
{
	return new DS_CreateArray(*this);
}

void DS_CreateArray::Complete(bool displayed)
{
	VO_Array* newArray = new VO_Array(dsArrayAddress, world, elementType, elements);
	newArray->move(world->GetArrayPosition());
		
	// TODO implement const sizeHint
	newArray->resize(newArray->sizeHint());
	//newArray->setMinimumSize(newArray->sizeHint());
	// MIGRATION assume top level
	newArray->setParent(world);
	newArray->setVisible(true);
	//newViewable->SetupLayout();
	
	// hrmmm
	//world->setMinimumSize(world->sizeHint());
	newArray->GetType();
	Registry::GetInstance()->ActionAgentCallback(newArray);
}





//////////////// DS_CreateSP
DS_CreateSP::DS_CreateSP(World* world, const void* dsAddress, const std::string& value)
		: DS_Action(world, true), dsAddress(dsAddress), value(value)
{
}

DS_CreateSP::DS_CreateSP(const DS_CreateSP& other)
	: DS_Action(other), dsAddress(other.dsAddress), value(other.value)
{
}

Action* DS_CreateSP::Clone() const
{
	return new DS_CreateSP(*this);
}

void DS_CreateSP::Complete(bool displayed)
{
	VO_SinglePrintable* newSP = new VO_SinglePrintable(dsAddress, world, value);
	newSP->GetType();

	//cout << "\tInside DS_CreateSP::Complete - callback for " << dsAddress << "\\" << newSP << endl;
	Registry::GetInstance()->ActionAgentCallback(newSP);
}








//////////////// DS_AddElementToArray
DS_AddElementToArray::DS_AddElementToArray
		(World* world, VO_Array* voArray, ViewableObject* element, unsigned position)
			: DS_Action(world, true), voArray(voArray), element(element), position(position)
{
}

DS_AddElementToArray::DS_AddElementToArray(const DS_AddElementToArray& other)
		: DS_Action(other), voArray(other.voArray), element(other.element), position(other.position)
{
}

Action* DS_AddElementToArray::Clone() const
{
	return new DS_AddElementToArray(*this);
}

void DS_AddElementToArray::Complete(bool displayed)
{
	voArray->AddElement(element, position);
	voArray->resize(voArray->sizeHint());
	element->SetSizeControlledByParentArray(true);

	//cout << "\tInside DS_AddElementToArray::Complete - callback for array " << voArray << endl;
	Registry::GetInstance()->ActionAgentCallback(voArray);
}

	



//////////////// DS_ArrayResize
DS_ArrayResize::DS_ArrayResize(World* world, VO_Array* voArray, 
								std::vector<ViewableObject*> elementsToAdd, unsigned newCapacity)
	: DS_Action(world, true), voArray(voArray), elementsToAdd(elementsToAdd), newCapacity(newCapacity)
{
}

DS_ArrayResize::DS_ArrayResize(const DS_ArrayResize& other)
		: DS_Action(other), voArray(other.voArray), elementsToAdd(other.elementsToAdd), newCapacity(other.newCapacity)
{
}

Action* DS_ArrayResize::Clone() const
{
	return new DS_ArrayResize(*this);
}

void DS_ArrayResize::Complete(bool displayed)
{
	voArray->ClearArray(newCapacity);
	
	unsigned position = 0;

	BOOST_FOREACH(ViewableObject* element, elementsToAdd)
		voArray->AddElement(element, position++);

	Registry::GetInstance()->ActionAgentCallback(NULL);
}

}