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


////////////////////// Utility functions ////////////////////////////

SourceData ValueIDToSourceData(ValueID id, ViewableObject* subject)
{
	Registry* registry = Registry::GetInstance();

	SourceData sourceData;
	sourceData.dimensions = QRect(0,0,0,0);
	sourceData.source = NULL;
	sourceData.isSibling = false;

	if (registry->IsRegistered(id.id, SINGLE_PRINTABLE))
	{
		VO_SinglePrintable* source = registry->GetRepresentation<VO_SinglePrintable>(id.id);

		if (source != NULL)
		{
			// TODO: Take time into account. Value may have changed.
			sourceData.source = source;
			if (sourceData.source->ModifiedTime() <= id.time) // If value has not been modified since it was accessed
			{
				sourceData.dimensions = QRect(source->GetPositionInWorld(), source->size());
				sourceData.isSibling = source->parent() != NULL && source->parent() == subject->parent();
			}
		}
	}
	return sourceData;
}




std::vector<ViewableObject*> ConvertIdsToViewablePtrs(const std::vector<ID>& ids)
{
	std::vector<ViewableObject*> viewablePtrs;

	BOOST_FOREACH(ID id, ids)
	{
		ViewableObject* voPtr = Registry::GetInstance()->GetRepresentation(id);
		UL_ASSERT(voPtr);

		viewablePtrs.push_back(voPtr);
	}

	return viewablePtrs;
}



std::vector<ViewableObject*> ConvertIdsToViewablePtrs(
									const std::vector<ID>& ids, ViewableObjectType expectedType)
{
	std::vector<ViewableObject*> viewablePtrs;

	BOOST_FOREACH(ID id, ids)
	{
		UL_ASSERT(Registry::GetInstance()->IsRegistered(id, expectedType));

		ViewableObject* voPtr = Registry::GetInstance()->GetRepresentation(id);
		UL_ASSERT(voPtr);

		viewablePtrs.push_back(voPtr);
	}

	return viewablePtrs;
}


std::vector<SourceData> historyToSources(const std::set<ValueID>& history, ViewableObject* subject)
{
	std::vector<SourceData> data;
	for (std::set<ValueID>::const_iterator i = history.begin(); i != history.end(); i++)
	{
		data.push_back(ValueIDToSourceData(*i, subject));
	}
	return data;
}

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






////////////////////// DS_Deleted implementation ////////////////////////////
DS_Deleted::DS_Deleted(World*, ID dsSubject)
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
	
	registry->Deregister(voToBeDeleted->GetId());
	voToBeDeleted->deleteLater();
}


////////////////////// DS_Composite implementation ////////////////////////////
DS_CompositeAction::DS_CompositeAction(World* world)
	: DS_Action(world, true)
{
}

DS_CompositeAction::DS_CompositeAction(const DS_CompositeAction& other)
	: DS_Action(other)
{
	for (unsigned i = 0; i < other.subActions.size(); i++)
		subActions.push_back((DS_Action*)other.subActions[i]->Clone());
}

DS_CompositeAction::~DS_CompositeAction()
{
	for (unsigned i = 0; i < subActions.size(); i++)
		delete subActions[i];
}

Action* DS_CompositeAction::Clone() const
{
	return new DS_CompositeAction(*this);
}

void DS_CompositeAction::PrepareToPerform()
{
	suppressAnimation = true; // Assume suppressed unless a sub animation is not suppressed after preparation
	for (unsigned i = 0; i < subActions.size(); i++)
	{
		subActions[i]->PrepareToPerform();
		if (!subActions[i]->AnimationSuppressed())
			suppressAnimation = false;
	}
}

void DS_CompositeAction::Perform(float progress, QPainter* painter)
{
	for (unsigned i = 0; i < subActions.size(); i++)
		subActions[i]->Perform(progress, painter);
}

void DS_CompositeAction::Complete(bool displayed)
{
	for (unsigned i = 0; i < subActions.size(); i++)
		subActions[i]->Complete(displayed);
}


void DS_CompositeAction::AddAction(DS_Action* action)
{
	subActions.push_back((DS_Action*)action->Clone()); // Cast ok, guaranteed to be a DS_Action
	if (!action->AnimationSuppressed())
		suppressAnimation = false;
}




}