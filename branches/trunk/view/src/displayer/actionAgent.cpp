#include <QPainter>
#include "utilities.h"

#include "actionAgent.h"
#include "../actions/action.h"
#include "../include/registry.h"



namespace Algovis_Viewer
{



ActionAgent::ActionAgent(QWidget* parent, World* world, QPoint& position, QSize& dimensions)
	: Component(parent, position, dimensions), world(world), actionToBePerformed(NULL), actionPending(false), 
			animLength(0.5f), animationsSuppressed(false), animationsPaused(false),
				performActionCount(0), shuttingDown(false)
{
	setAttribute(Qt::WA_TransparentForMouseEvents);
}



/* Since the view is shutting down, we need to stop the Registry on the main thread 
 * from touching GUI components through PerformAndAnimateAsync\actionToBePerformed->PrepareToPerform(), and
 * keep it out of PerformAndAnimateAsync in general, so that we can destruct. Note:
 *		- By this point, the Registry knows the view is shutting down and thus, will not send future 
 * 		  PerformAndAnimateAsync invocations our way 
 *						(Registry::AddActionToBuffer is where the gatekeeping happens)
 *		- There potentially exist 'future invocations' which had made it past AddActionToBuffer by the time
 *			the Registry found out about the view shutting down. These are staved off by the first 
 *			shuttingDown predicate.
 *		- We stave off a current invocation which is waiting on actionPending by releasing mutexes etc. to 
 *			coax it towards the second shuttingDown predicate which sends it packing.
 *		- We busy wait until performActionCount = 0 before we continue with destruction
 */
ActionAgent::~ActionAgent() 
{ 
	shuttingDown = true;

	actionPending = false;
	actionPendingCondVar.notify_all();
	performActionMutex.unlock();
	
	while(performActionCount > 0);

	if (actionToBePerformed != NULL)
		delete actionToBePerformed;
}


// See comments about shutting down in ~ActionAgent
void ActionAgent::PerformAndAnimateActionAsync(const Action* newAction)
{
	++performActionCount;

	if (shuttingDown)
	{
		--performActionCount;
		return;
	}
			

	// Matched by unlock inside paintEvent()
	performActionMutex.lock();


	while(actionPending)
		actionPendingCondVar.wait<boost::mutex>(performActionMutex);

	if (shuttingDown)
	{
		--performActionCount;
		return;
	}

	animStartTime = QTime::currentTime();
	actionToBePerformed = newAction->Clone();
	actionPending = true;
	actionPrepared = false;

	--performActionCount;
}


void ActionAgent::paintEvent(QPaintEvent*)
{
	if (actionPending)
	{
		if (!actionPrepared)
		{
			actionToBePerformed->PrepareToPerform();
			actionPrepared = true;
		}
		// Animation for the action is suppressed or animation has finished
		if (animationsSuppressed || actionToBePerformed->AnimationSuppressed() || AnimDuration() > animLength)
		{
			#if(DEBUG_ACTION_LEVEL >=1)
				prt("\tAbout to complete action");		
				actionToBePerformed->Complete(true);
				prt("\tCompleted action");		
			#else			
				actionToBePerformed->Complete(!actionToBePerformed->AnimationSuppressed());
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
			if (animationsPaused)
				return;

			QPainter painter(this);
			actionToBePerformed->Perform(AnimDuration() / animLength, &painter);
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
		animStartTime = animStartTime.addMSecs(-int(animLength*1000)); // Subtract enough off the start time that it is complete
}

void ActionAgent::toggleAnimations()
{
	animationsSuppressed = !animationsSuppressed;
}

void ActionAgent::pauseResumeAnimations()
{
	animationsPaused = !animationsPaused;
}


}