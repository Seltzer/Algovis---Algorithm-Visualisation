#include <iostream>
#include <cmath>
#include "boost/foreach.hpp"
#include "dsactions.h"
#include "../../include/registry.h"
#include "../displayer/world.h"
#include "../viewableObjects/vo_singlePrintable.h"
#include "../viewableObjects/vo_array.h"

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

DS_Assigned::DS_Assigned(World* world, const void* dsAssigned, const void* dsSource, 
							std::string value, bool tracked)
	: DS_Action(world), value(value), dsAssigned(dsAssigned), dsSource(dsSource), tracked(tracked),
			sourceDimensions(0,0,0,0), source(NULL)
{
	//ViewableObject* viewable = (ViewableObject*)subject;
	//subjects.insert(viewable); // May need to keep the original printable pointer?
}

DS_Assigned::DS_Assigned(const DS_Assigned& other)
	: DS_Action(other), dsAssigned(other.dsAssigned), dsSource(other.dsSource), value(other.value),
	subject(other.subject), history(other.history), sourceDimensions(other.sourceDimensions),
	source(other.source), sourceIsSibling(other.sourceIsSibling)
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

	Registry* registry = Registry::GetInstance();

	UL_ASSERT(registry->IsRegistered(dsAssigned, SINGLE_PRINTABLE));
	subject = registry->GetRepresentation<VO_SinglePrintable>(dsAssigned);
	UL_ASSERT(subject);

	if (registry->IsRegistered(dsSource, SINGLE_PRINTABLE))
	{
		source = registry->GetRepresentation<VO_SinglePrintable>(dsSource);
		sourceIsSibling = source->parent() == subject->parent();
		UL_ASSERT(source);
		history = source->GetHistory();

		// TODO: Currently we only want an animation for assignments to SPs which are owned by an array
		if (subject->parent())
		{
			if (typeid(*subject->parent()) != typeid(VO_Array))
				suppressAnimation = true;
		}
		else
			suppressAnimation = true;
	}
	else
	{
		// The only history we have is that the value was untracked, so chuck in an element to represent that
		history.clear();
		// Time -1 denotes elements that aren't actually tracked (TODO: That sucks)
		// dsSource or dsAssigned??? check repository TODO
		history.insert(ValueID(dsAssigned, -1)); 

		suppressAnimation = true;

	}

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

	float lerp = 1 - (cos(progress * 3.14159265358) + 1) / 2;
	
	if (sourceIsSibling)
	{
		// Assume source and subject have the same y for now
		x = sourceDimensions.x() + lerp * (subjectDimensions.x() - sourceDimensions.x());
		//y = sourceDimensions.y() - 5 * sin(3.14 * progress) * (subjectStart.y() - sourceDimensions.y());
		y = sourceDimensions.y() - 30 * sin(3.14 * progress);
	}
	else
	{
		x = sourceDimensions.x() + lerp * (subjectDimensions.x() - sourceDimensions.x());
		y = sourceDimensions.y() + lerp * (subjectDimensions.y() - sourceDimensions.y());
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
}


////////////////////// DS_Modified implementation ////////////////////////////

DS_Modified::DS_Modified(World* world) 
	: DS_Action(world), sourceDimensions(0,0,0,0), source(NULL)
{
	UL_ASSERT(world);
}

DS_Modified::DS_Modified(World* world, const void* dsModified, const void* dsSource, 
							std::string value, bool tracked)
	: DS_Action(world), value(value), dsModified(dsModified), dsSource(dsSource), tracked(tracked),
			sourceDimensions(0,0,0,0), source(NULL)
{
	//ViewableObject* viewable = (ViewableObject*)subject;
	//subjects.insert(viewable); // May need to keep the original printable pointer?
}

DS_Modified::DS_Modified(const DS_Modified& other)
	: DS_Action(other), dsModified(other.dsModified), dsSource(other.dsSource), value(other.value),
	subject(other.subject), history(other.history), sourceDimensions(other.sourceDimensions),
	source(other.source), sourceIsSibling(other.sourceIsSibling)
{
}


Action* DS_Modified::Clone() const
{
	return new DS_Modified(*this);
}

void DS_Modified::SetSource(VO_SinglePrintable* source)
{
	this->source = source;
	sourceIsSibling = source->parent() == subject->parent();
}


void DS_Modified::PrepareToPerform()
{
	//subject->EnableDrawing(false);

	Registry* registry = Registry::GetInstance();

	UL_ASSERT(registry->IsRegistered(dsModified, SINGLE_PRINTABLE));
	subject = registry->GetRepresentation<VO_SinglePrintable>(dsModified);
	UL_ASSERT(subject);

	if (registry->IsRegistered(dsSource, SINGLE_PRINTABLE))
	{
		VO_SinglePrintable* source = registry->GetRepresentation<VO_SinglePrintable>(dsSource);
		UL_ASSERT(source);
		history = subject->GetHistory();
		history.insert(source->GetHistory().begin(), source->GetHistory().end());

		// TODO: Currently we only want an animation for assignments to SPs which are owned by an array
		if (subject->parent())
		{
			if (typeid(*subject->parent()) != typeid(VO_Array))
				suppressAnimation = true;
		}
		else
			suppressAnimation = true;
	}
	else
	{
		// The only history we have is that the value was untracked, so chuck in an element to represent that
		history = subject->GetHistory();
		// Time -1 denotes elements that aren't actually tracked (TODO: That sucks)
		// dsSource or dsAssigned??? check repository TODO
		history.insert(ValueID(dsModified, -1)); 

		suppressAnimation = true;

	}

	// Set subjectStart to have abs position
	subjectDimensions = QRect(subject->GetPositionInWorld(), subject->size());

	if (source != NULL)
	{
		sourceDimensions = QRect(source->GetPositionInWorld(), source->size());
		
	//	source->EnableDrawing(false);
	}
		
}

void DS_Modified::Perform(float progress, QPainter* painter)
{
	if (source == NULL)
		return;

	float x,y;

	float lerp = 1 - (cos(progress * 3.14159265358) + 1) / 2;
	
	if (sourceIsSibling)
	{
		// Assume source and subject have the same y for now
		x = sourceDimensions.x() + lerp * (subjectDimensions.x() - sourceDimensions.x());
		//y = sourceDimensions.y() - 5 * sin(3.14 * progress) * (subjectStart.y() - sourceDimensions.y());
		y = sourceDimensions.y() - 30 * sin(3.14 * progress);
	}
	else
	{
		x = sourceDimensions.x() + lerp * (subjectDimensions.x() - sourceDimensions.x());
		y = sourceDimensions.y() + lerp * (subjectDimensions.y() - sourceDimensions.y());
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

void DS_Modified::Complete(bool displayed)
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
}



////////////////////// DS_Deleted implementation ////////////////////////////

DS_Deleted::DS_Deleted(World* world) 
	: DS_Action(world) 
{
}

DS_Deleted::DS_Deleted(World* world, const void* dsSubject)
	: DS_Action(world), dsSubject(dsSubject)
{
	//subjects.insert(subject);  // TODO: Sort out whether subjects is needed
}


DS_Deleted::DS_Deleted(const DS_Deleted& other)
	: DS_Action(other), dsSubject(other.dsSubject)
{
}


Action* DS_Deleted::Clone() const
{
	return new DS_Deleted(*this);
}

void DS_Deleted::Complete(bool displayed)
{
	Registry* registry = Registry::GetInstance();

	ViewableObject* voToBeDeleted = registry->GetRepresentation(dsSubject);
	UL_ASSERT(voToBeDeleted);
	
	registry->Deregister(voToBeDeleted->GetId(), voToBeDeleted->GetDSAddress());
	delete voToBeDeleted;
}




//////////////// DS_CreateArray
DS_CreateArray::DS_CreateArray(World* world, ID id, 
	const void* dsArrayAddress, ViewableObjectType elementType, std::vector<void*> elements)
		: DS_Action(world, true), id(id), dsArrayAddress(dsArrayAddress), elementType(elementType), elements(elements)
{
}

DS_CreateArray::DS_CreateArray(const DS_CreateArray& other)
	: DS_Action(other), id(other.id),dsArrayAddress(other.dsArrayAddress), 
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
	UL_ASSERT(!registry->IsRegistered(dsArrayAddress));


	// ViewableObject equivalents of elements
	vector<ViewableObject*> arrayElements;

	// Iterate over elements, verify that they are all registered and populate arrayElements
	BOOST_FOREACH(void* dsElement, elements)
	{
		// TODO: change behaviour when above registration condition is violated (i.e. throw exception)
		UL_ASSERT(registry->IsRegistered(dsElement));
		arrayElements.push_back(registry->GetRepresentation(dsElement));
	}


	VO_Array* newArray = new VO_Array(id, dsArrayAddress, world, elementType, arrayElements);
	

	newArray->move(world->GetArrayPosition());
		
	// TODO implement const sizeHint
	newArray->resize(newArray->sizeHint());
	//newArray->setMinimumSize(newArray->sizeHint());
	// MIGRATION assume top level
	newArray->setParent(world);
	newArray->setVisible(true);
	//newViewable->SetupLayout();

	Registry::GetInstance()->Register(id, dsArrayAddress, newArray);
	
	// hrmmm
	//world->setMinimumSize(world->sizeHint());
}





//////////////// DS_CreateSP
DS_CreateSP::DS_CreateSP(World* world, ID id, const void* dsAddress, const std::string& value)
		: DS_Action(world, true), id(id), dsAddress(dsAddress), value(value)
{
}

DS_CreateSP::DS_CreateSP(const DS_CreateSP& other)
	: DS_Action(other), id(other.id), dsAddress(other.dsAddress), value(other.value)
{
}

Action* DS_CreateSP::Clone() const
{
	return new DS_CreateSP(*this);
}

void DS_CreateSP::Complete(bool displayed)
{
	Registry* registry = Registry::GetInstance();

	// Verify that array hasn't already been registered
	UL_ASSERT(!registry->IsRegistered(dsAddress));

	VO_SinglePrintable* newSP = new VO_SinglePrintable(id,dsAddress, world, value);
	registry->Register(id, dsAddress, newSP);

	UL_ASSERT(registry->IsRegistered(dsAddress,SINGLE_PRINTABLE));

	//cout << "\tInside DS_CreateSP::Complete - callback for " << dsAddress << "\\" << newSP << endl
}








//////////////// DS_AddElementToArray
DS_AddElementToArray::DS_AddElementToArray
		(World* world, const void* dsArray, const void* dsElement, unsigned position)
			: DS_Action(world, true), dsArray(dsArray), dsElement(dsElement), position(position)
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

void DS_AddElementToArray::Complete(bool displayed)
{
	Registry* registry = Registry::GetInstance();

	UL_ASSERT(registry->IsRegistered(dsArray,ARRAY));
	UL_ASSERT(registry->IsRegistered(dsElement,SINGLE_PRINTABLE));

	VO_Array* voArray = registry->GetRepresentation<VO_Array>(dsArray);
	UL_ASSERT(position <= voArray->GetSize());

	VO_SinglePrintable* element = registry->GetRepresentation<VO_SinglePrintable>(dsElement);

	voArray->AddElement(element, position);
	//voArray->SetupLayout();
	voArray->adjustSize();
	element->SetSizeControlledByParentArray(true);

	//cout << "\tInside DS_AddElementToArray::Complete - callback for array " << voArray << endl;
}

	



//////////////// DS_ArrayResize
DS_ArrayResize::DS_ArrayResize(World* world, const void* dsArray, 
								std::vector<void*> elements, unsigned newCapacity)
	: DS_Action(world, true), dsArray(dsArray), elements(elements), newCapacity(newCapacity)
{
}

DS_ArrayResize::DS_ArrayResize(const DS_ArrayResize& other)
		: DS_Action(other), dsArray(other.dsArray), elements(other.elements), newCapacity(other.newCapacity)
{
}

Action* DS_ArrayResize::Clone() const
{
	return new DS_ArrayResize(*this);
}

void DS_ArrayResize::Complete(bool displayed)
{
	Registry* registry = Registry::GetInstance();

	UL_ASSERT(registry->IsRegistered(dsArray, ARRAY));
	VO_Array* voArray = registry->GetRepresentation<VO_Array>(dsArray);
	UL_ASSERT(voArray);

	UL_ASSERT(newCapacity >= elements.size());
	vector<ViewableObject*> elementsToAdd;
	
	BOOST_FOREACH(void* dsElement, elements)
	{
		UL_ASSERT(registry->IsRegistered(dsElement));
		elementsToAdd.push_back(registry->GetRepresentation(dsElement));
	}


	voArray->ClearArray(newCapacity);
	
	unsigned position = 0;

	BOOST_FOREACH(ViewableObject* element, elementsToAdd)
		voArray->AddElement(element, position++);
}

}