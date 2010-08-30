#include "actionBuffer.h"
#include "action.h"
#include "dsActions.h"
#include "../displayer/displayer.h"

namespace Algovis_Viewer
{


ActionBuffer::ActionBuffer(unsigned capacity)
	: bufferCapacity(capacity)
{
}


void ActionBuffer::PushBack(DS_Action* action)
{
	if (Full())
	{
		DS_Action* top = buffer.front();
		Displayer::GetInstance()->PerformAndAnimateActionAsync(top);
		buffer.pop_front();
		// TODO: Delete action
	}

	DS_Action* copy = (DS_Action*)action->Clone();

	// Collapse actions if appropriate
	DS_CompositeAction* composite = new DS_CompositeAction(Displayer::GetInstance()->GetWorld());
	composite->AddAction(copy);

	buffer.push_back(composite);
}

}