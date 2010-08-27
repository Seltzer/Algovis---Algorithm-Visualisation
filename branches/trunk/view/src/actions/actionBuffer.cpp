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

	// TODO: Collaps actions if appropriate.

	Action* copy = action->Clone();
//	buffer.push_back(action);
	buffer.push_back((DS_Action*) copy);

}

}