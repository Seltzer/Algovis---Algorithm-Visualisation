#include "boost/foreach.hpp"
#include <QPainter>
#include <QColor>
#include "dsActions.h"
#include "../../include/registry.h"
#include "../viewableObjects/vo_singlePrintable.h"
#include "../displayer/displayer.h"			// used for DS_SetCaption

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
			sourceData.source = source;
			if (source->ModifiedTime() <= id.time)
			{
				sourceData.dimensions = QRect(source->GetPositionInWorld(), source->size());
				sourceData.isSibling = source->parent() != NULL && source->parent() == subject->parent();
			}
		}
	}
	return sourceData;
}




vector<ViewableObject*> ConvertIdsToViewablePtrs(const vector<ID>& ids)
{
	vector<ViewableObject*> viewablePtrs;

	BOOST_FOREACH(ID id, ids)
	{
		ViewableObject* voPtr = Registry::GetInstance()->GetRepresentation(id);
		UL_ASSERT(voPtr);

		viewablePtrs.push_back(voPtr);
	}

	return viewablePtrs;
}



vector<ViewableObject*> ConvertIdsToViewablePtrs(const vector<ID>& ids, ViewableObjectType expectedType)
{
	vector<ViewableObject*> viewablePtrs;

	BOOST_FOREACH(ID id, ids)
	{
		UL_ASSERT(Registry::GetInstance()->IsRegistered(id, expectedType));

		ViewableObject* voPtr = Registry::GetInstance()->GetRepresentation(id);
		UL_ASSERT(voPtr);

		viewablePtrs.push_back(voPtr);
	}

	return viewablePtrs;
}


vector<SourceData> HistoryToSources(const set<ValueID>& history, ViewableObject* subject)
{
	vector<SourceData> data;
	for (set<ValueID>::const_iterator i = history.begin(); i != history.end(); i++)
		data.push_back(ValueIDToSourceData(*i, subject));

	return data;
}








////////////////////// DS_Action implementation ////////////////////////////


DS_Action::DS_Action(World* world, bool suppressAnimation)
	: Action(world, suppressAnimation)
{
}


DS_Action::DS_Action(World* world, set<ViewableObject*> subjects, bool suppressAnimation)
	: Action(world, suppressAnimation)
{
}

DS_Action::DS_Action(const DS_Action& other)
	: Action(other), completeTime(other.completeTime)
{
	UL_ASSERT(world);
}

void DS_Action::UpdateHistory(HistoryManager& historyManager)
{
	completeTime = historyManager.GetTime();
	historyManager.ActionProcessed();
}

Action* DS_Action::Clone() const
{
	return new DS_Action(*this);
}


////////////////////// DS_CreateAction implementation ////////////////////////////
DS_CreateAction::DS_CreateAction(World* world, bool animationSuppressed, bool createOnSameLine)
	: DS_Action(world, animationSuppressed), createOnSameLine(createOnSameLine)
{
}

DS_CreateAction::DS_CreateAction(World*, std::set<ViewableObject*> subjects, bool animationSuppressed, 
					bool createOnNewLine)
	: DS_Action(world, subjects, animationSuppressed), createOnSameLine(createOnSameLine)
{
}

DS_CreateAction::DS_CreateAction(const DS_CreateAction& other)
	: DS_Action(other), createOnSameLine(other.createOnSameLine)
{
}

void DS_CreateAction::PlaceOnSameLine()
{
	createOnSameLine = true;
}

bool DS_CreateAction::BeingCreatedOnSameLine()
{
	return createOnSameLine;
}


////////////////////// DS_Deleted implementation ////////////////////////////
DS_Deleted::DS_Deleted(World*, ID dsSubject)
	: DS_Action(world), dsSubject(dsSubject)
{
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
		subActions.push_back(other.subActions[i]->Clone());
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
	for (unsigned i = 0; i < subActions.size(); i++)
	{
		if (!subActions[i]->AnimationSuppressed())
			subActions[i]->PrepareToPerform();
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


void DS_CompositeAction::AddAction(Action* action)
{
	subActions.push_back(action->Clone()); // Cast ok, guaranteed to be a DS_Action
	if (!action->AnimationSuppressed())
		suppressAnimation = false;
}



DS_SetCaption::DS_SetCaption(World* world, const string& newCaption)
	: DS_Action(world), newCaption(newCaption)
{
}

DS_SetCaption::DS_SetCaption(const DS_SetCaption& other)
	: DS_Action(other), newCaption(other.newCaption)
{
}


Action* DS_SetCaption::Clone() const
{
	return new DS_SetCaption(*this);
}


void DS_SetCaption::Complete(bool)
{
	Displayer::GetInstance()->SetCaption(newCaption);
}











}