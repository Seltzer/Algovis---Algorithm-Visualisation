#include <QPainter>
#include "utilities.h"

#include "actionAgent.h"
#include "../actions/action.h"
#include "world.h"





namespace Algovis_Viewer
{



ActionAgent::ActionAgent(QWidget* parent, World* world, QPoint& position, QSize& dimensions)
	: Component(parent, position, dimensions), world(world), 
			actionToBePerformed(NULL), actionPending(false), duration(0)
{
	setAttribute(Qt::WA_TransparentForMouseEvents);
}


void ActionAgent::PerformAndAnimateActionAsync(const Action* newAction)
{
	// Matched by unlock inside paintEvent()
	performActionMutex.lock();

	while(actionPending)
		actionPendingCondVar.wait<boost::mutex>(performActionMutex);

	duration = 0;
	actionToBePerformed = newAction->Clone();
	actionToBePerformed->PrepareToPerform();
	actionPending = true;
}


void ActionAgent::paintEvent(QPaintEvent*)
{
	if (actionPending)
	{
		// Animation for the action is suppressed or animation has finished
		if (actionToBePerformed->AnimationSuppressed() || ++duration > 60)
		{
			#if(DEBUG_ACTION_LEVEL >=1)
				prt("\tAbout to complete action");		
				actionToBePerformed->Complete(true);
				prt("\tCompleted action");		
			#else			
				actionToBePerformed->Complete(true);
			#endif
			
			// Clean up action
			delete actionToBePerformed;
			actionToBePerformed = NULL;
			actionPending = false;

			performActionMutex.unlock();
			actionPendingCondVar.notify_all();
		}
		else
		{
			QPainter painter(this);
			actionToBePerformed->Perform((float) duration / 60, &painter);
		}
	}
}

QSize ActionAgent::sizeHint() const 
{ 
	return size();
}

void ActionAgent::skipAnimation()
{
	if (actionPending)
		duration = 60;
}


}