
#include "dsPrintableActions.h"

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
	UL_ASSERT(!registry->IsRegistered(id));

	VO_SinglePrintable* newSP = new VO_SinglePrintable(id,dsAddress, world, value);
	registry->Register(id, newSP);

	// TODO hack
	world->adjustSize();


	UL_ASSERT(registry->IsRegistered(id,SINGLE_PRINTABLE));

	if (displayed)
	{
		int time = Registry::GetInstance()->CurrentTime();
		newSP->ResetHistory(ValueID(newSP->GetId(), time));
	}
}


////////////////////// DS_Assigned implementation ////////////////////////////
DS_Assigned::DS_Assigned(World* world, ID dsAssigned, ID dsSource, std::string value, bool tracked)
	: DS_Action(world), value(value), dsAssigned(dsAssigned), dsSource(dsSource), tracked(tracked)
{
	//ViewableObject* viewable = (ViewableObject*)subject;
	//subjects.insert(viewable); // May need to keep the original printable pointer?
}

DS_Assigned::DS_Assigned(const DS_Assigned& other)
	: DS_Action(other), dsAssigned(other.dsAssigned), dsSource(other.dsSource), value(other.value),
	subject(other.subject), history(other.history), sources(other.sources)
{
}

Action* DS_Assigned::Clone() const 
{ 
	return new DS_Assigned(*this); 
}

void DS_Assigned::SetSource(VO_SinglePrintable* source)
{
	//this->source = source;
	//sourceIsSibling = source->parent() == subject->parent();
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
		VO_SinglePrintable* source = registry->GetRepresentation<VO_SinglePrintable>(dsSource);
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

	// TODO: There is only a point to this if animation is not supressed
	sources = historyToSources(history, subject);
}

void DS_Assigned::Perform(float progress, QPainter* painter)
{
	//if (source == NULL)
	//	return;

	float x,y;

	float lerp = 1 - (cos(progress * 3.14159265358) + 1) / 2;
	
	for (unsigned i = 0; i < sources.size(); i++)
	{
		SourceData& source = sources[i];
		if (source.source != NULL) // TODO: Need better value recording for untracked stuff
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
		int time = Algovis_Viewer::Registry::GetInstance()->CurrentTime();
		subject->ResetHistory(ValueID(subject->GetId(), time));

		subject->EnableDrawing(true);
		//if (source != NULL)
		//	source->EnableDrawing(true);
	}
}




////////////////////// DS_Modified implementation ////////////////////////////
DS_Modified::DS_Modified(World* world, ID dsModified, ID dsSource, std::string value, bool tracked)
	: DS_Action(world), value(value), dsModified(dsModified), dsSource(dsSource), tracked(tracked)
{
	//ViewableObject* viewable = (ViewableObject*)subject;
	//subjects.insert(viewable); // May need to keep the original printable pointer?
}

DS_Modified::DS_Modified(const DS_Modified& other)
	: DS_Action(other), dsModified(other.dsModified), dsSource(other.dsSource), value(other.value),
	subject(other.subject), history(other.history), sources(other.sources)
{
}


Action* DS_Modified::Clone() const
{
	return new DS_Modified(*this);
}

void DS_Modified::SetSource(VO_SinglePrintable* source)
{
	//this->source = source;
	//sourceIsSibling = source->parent() == subject->parent();
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

	// Set up data for all the sources
	sources = historyToSources(history, subject);
}

void DS_Modified::Perform(float progress, QPainter* painter)
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

void DS_Modified::Complete(bool displayed)
{
	subject->Assigned(history, value);
	
	if (displayed)
	{
		// As we have completed this animation and presumably displayed the history
		// and will reset the current history to be the value we just displayed
		// That way future things will have the just-displayed-element in their history, instead of everything
		// that was used to produce is.
		int time = Registry::GetInstance()->CurrentTime();
		subject->ResetHistory(ValueID(subject->GetId(), time));

		subject->EnableDrawing(true);
		//if (source != NULL)
		//	source->EnableDrawing(true);
	}
}

/////////////////// DS_HighlightOperands
DS_HighlightOperands::DS_HighlightOperands(World* world, std::vector<ID> operands)
	: DS_Action(world), operands(operands)
{
}

DS_HighlightOperands::DS_HighlightOperands(const DS_HighlightOperands& other)
	: DS_Action(other), operands(other.operands), operandPtrs(other.operandPtrs), 
				originalBBColour(other.originalBBColour)
{
}

Action* DS_HighlightOperands::Clone() const
{
	return new DS_HighlightOperands(*this);
}
	
void DS_HighlightOperands::PrepareToPerform()
{
	Registry* reg = Registry::GetInstance();

	BOOST_FOREACH(ID op, operands)
	{
		VO_SinglePrintable* vo = reg->GetRepresentation<VO_SinglePrintable>(op);
		UL_ASSERT(vo);		
		
		// Who cares if this is called multiple times? It's a hack!
		originalBBColour = vo->GetBoundingBoxColour();
		operandPtrs.push_back(vo);
	}

}
	
void DS_HighlightOperands::Perform(float progress, QPainter*)
{	
	BOOST_FOREACH(VO_SinglePrintable* vo, operandPtrs)
	{
		int intensity = (int) (progress * 255);
		vo->SetBoundingBoxColour(QColor(intensity, intensity, intensity));
		
		/*
		painter->setPen(QColor(Qt::white));
		QRect dim(vo->GetPositionInWorld(), vo->size());
		vo->DrawValue(dim, painter);

		int intensity = (int) (progress * 255);
		painter->setPen(QColor(intensity, intensity, intensity));
		vo->DrawWithoutValue(dim, painter);*/
	}
}

void DS_HighlightOperands::Complete(bool displayed)
{
	BOOST_FOREACH(VO_SinglePrintable* vo, operandPtrs)
		vo->SetBoundingBoxColour(originalBBColour);
}




}
