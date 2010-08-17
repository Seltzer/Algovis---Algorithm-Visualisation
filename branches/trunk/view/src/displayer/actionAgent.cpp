#include "actionAgent.h"

#include "../action.h"
#include "../world.h"





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
	actionPending = true;

	actionToBePerformed = newAction->Clone();
	actionToBePerformed->PrepareToPerform();
}


void ActionAgent::paintEvent(QPaintEvent*)
{
	if (actionPending)
	{
		//actionToBePerformed->Perform((float)duration / 60, *win, defaultFont);

		// Animation for the action is suppressed or animation has finished
		if (actionToBePerformed->AnimationSuppressed() || ++ duration == 60)
		{
			#if(DEBUG_ACTION_LEVEL >=1)
				prt("about to complete action");		
				actionToBePerformed->Complete(true);
				prt("completed action");		
			#else			
				actionToBePerformed->Complete(true);
			#endif
			
			// Clean up action
			delete actionToBePerformed;
			actionToBePerformed = NULL;
			actionPending = false;

			// Release locks etc.
			world->ReleaseWriterLock();
			performActionMutex.unlock();
			actionPendingCondVar.notify_all();
		}
	}
	
	world->ReleaseWriterLock();
}





}