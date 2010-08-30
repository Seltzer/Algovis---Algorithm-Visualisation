#include <QPainter>
#include "utilities.h"

#include "actionAgent.h"
#include "../actions/action.h"
#include "world.h"

#include "../include/registry.h"



namespace Algovis_Viewer
{



ActionAgent::ActionAgent(QWidget* parent, World* world, QPoint& position, QSize& dimensions)
	: Component(parent, position, dimensions), world(world), actionToBePerformed(NULL), actionPending(false), 
			duration(0), shuttingDown(false)
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
 *		- We wait some time for a current invocation which is inside PrepareToPerform to leave (hacky!)
 */
ActionAgent::~ActionAgent() 
{ 
	shuttingDown = true;

	actionPending = false;
	actionPendingCondVar.notify_all();
	performActionMutex.unlock();
	// 0.5s should be more than enough time for a current invocation to finish - HACK HACK HACK TODO TODO TODO
	util::PlatformSleep(0.5f);
	
	if (actionToBePerformed != NULL)
		delete actionToBePerformed;
}


// See comments about shutting down in ~ActionAgent
void ActionAgent::PerformAndAnimateActionAsync(const Action* newAction)
{
	if (shuttingDown)
		return;

	// Matched by unlock inside paintEvent()
	performActionMutex.lock();

	while(actionPending)
		actionPendingCondVar.wait<boost::mutex>(performActionMutex);

	if (shuttingDown)
		return;

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
				actionToBePerformed->Complete(!actionToBePerformed->AnimationSuppressed());
			#endif

			Registry::GetInstance()->ActionCompleted();
			
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