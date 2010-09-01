#include "actionBuffer.h"
#include "action.h"
#include "dsActions.h"
#include "../displayer/displayer.h"

namespace Algovis_Viewer
{


ActionBuffer::ActionBuffer(unsigned capacity)
	: bufferCapacity(capacity), currentComposite(NULL)
{
}


void ActionBuffer::PushBack(Action* action)
{
	if (Full())
	{

		// action a, b
		// if a modifies some source s of b after b reads s (and s has not already been modified)
		//		want to combine
		// if a modifies some source s of b before b reads s
		//		can't combine
		int combineCount = 1; // Always include the first action (duh)
		for (unsigned i = 1; i < buffer.size(); i++)
		{
			Action* current = buffer[i];
			bool canCombine = true;
			bool desireCombine = false;

			// Go through previous actions checking if we can combine with them
			// And also whether it would be a good thing to do
			for (unsigned j = 0; j < i; j++)
			{
				int otherTime = historyManager.GetTime() - buffer.size() + j; // bleh
				if (!CanCombine(current, buffer[j], otherTime))
					canCombine = false;
				if (DesireCombine(current, buffer[j], otherTime))
					desireCombine = true;
			}

			if (desireCombine && !canCombine)
				std::cout << "Uncooperative actions :(" << std::endl; // Doesn't matter that much, just curious if this comes up often

			if (!canCombine) // Can't combine this action
				break; // So quit and leave our combineCount as is (don't bother combining anything which could be but doesn't need to)
			
			if (desireCombine) // If we want to combine this
				combineCount = i + 1; // Then also combine all previous actions, even if they don't desire it (but we know they can)
		}

		DS_CompositeAction* composite = new DS_CompositeAction(Displayer::GetInstance()->GetWorld());
		for (unsigned i = 0; i < combineCount; i++)
		{
			composite->AddAction(buffer.front());
			//delete buffer.front(); // TODO
			buffer.pop_front();
		}
		Displayer::GetInstance()->PerformAndAnimateActionAsync(composite);
	}

	Action* copy = action->Clone(); // Make a copy to protect original from UpdateHistory side effects (is there a point?)

	copy->UpdateHistory(historyManager);

	if (!copy->AnimationSuppressed())
		buffer.push_back(copy);
}

// Could stl this, just lazy
static std::vector<SourceID>::iterator FindID(std::vector<SourceID>& sources, ID id)
{
	for (std::vector<SourceID>::iterator i = sources.begin(); i != sources.end(); i++)
		if (i->vid.id == id)
			return i;
	return sources.end();
}

bool ActionBuffer::CanCombine(Action* tested, Action* other, int otherTime)
{
	DS_DataFlowAction* dfAction = dynamic_cast<DS_DataFlowAction*>(tested);
	DS_DataFlowAction* dfOther = dynamic_cast<DS_DataFlowAction*>(other);
	if (dfAction == NULL || dfOther == NULL) // Not even a dataflow action, don't worry
		return true;

	std::vector<SourceID> sources = dfAction->GetSources();
	std::set<ID> otherSubjects = dfOther->GetSubjects();
	for (std::set<ID>::iterator sub = otherSubjects.begin(); sub != otherSubjects.end(); sub++)
	{
		std::vector<SourceID>::iterator sid = FindID(sources, *sub);
		if (sid != sources.end()) // If this action relies on an element a previous action modifies
		{
			if (sid->vid.time > otherTime) // If tested action reads element after it is modified
				return false; // We can not combine these actions as tested depends on other being complete
		}
	}
	return true;
}

// This is so similar to above it's kinda retarded
bool ActionBuffer::DesireCombine(Action* tested, Action* other, int otherTime)
{
	DS_DataFlowAction* dfAction = dynamic_cast<DS_DataFlowAction*>(tested);
	DS_DataFlowAction* dfOther = dynamic_cast<DS_DataFlowAction*>(other);
	if (dfAction == NULL || dfOther == NULL) // Not even a dataflow action, don't worry
		return false;

	std::vector<SourceID> sources = dfAction->GetSources();
	std::set<ID> otherSubjects = dfOther->GetSubjects();
	for (std::set<ID>::iterator sub = otherSubjects.begin(); sub != otherSubjects.end(); sub++)
	{
		std::vector<SourceID>::iterator sid = FindID(sources, *sub);
		if (sid != sources.end()) // If this action relies on an element a previous action modifies
		{
			if (sid->vid.time < otherTime) // If tested action reads element after it is modified
				return true; // We can not combine these actions as tested depends on other being complete
		}
	}
	return false;
}

}