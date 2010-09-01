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


void ActionBuffer::PushBack(DS_Action* action)
{
	if (Full())
	{
		DS_Action* top = buffer.front();
		Displayer::GetInstance()->PerformAndAnimateActionAsync(top);
		buffer.pop_front();
		// TODO: Delete action
	}

	DS_Action* copy = (DS_Action*)action->Clone(); // Make a copy to protect original from UpdateHistory side effects (is there a point?)

	copy->UpdateHistory(historyManager);

	if (!copy->AnimationSuppressed())
		buffer.push_back(copy);

	//delete copy; // Composite does it's own copying.
}

}