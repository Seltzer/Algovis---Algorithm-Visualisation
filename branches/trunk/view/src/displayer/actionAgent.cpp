#include <QPainter>
#include "utilities.h"

#include "actionAgent.h"
#include "../actions/action.h"
#include "../include/registry.h"

using namespace std;



namespace Algovis_Viewer
{




ActionAgent::ActionAgent(QWidget* parent, World* world, QPoint& position, QSize& dimensions)
	: Component(parent, position, dimensions), world(world), actionPending(false), 
			actionPrepared(false), mode(AgentMode::ON_DEMAND),
			animLength(0.5f), animationsSuppressed(false), animationsPaused(false),
				performActionCount(0), currentAction(INVALID), shuttingDown(false)
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

	//if ( != NULL)
	//	delete actionToBePerformed;
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

	Action* clone = newAction->Clone();
	actionHistory.push_back(clone);
	actionPrepared = false;
	currentAction = actionHistory.size() - 1;

	// Do this last
	actionPending = true;


	--performActionCount;
}


void ActionAgent::paintEvent(QPaintEvent*)
{
	boost::unique_lock<boost::mutex> lock(pausingMutex);

	if (!actionPending)
		return;


	// Prepare to perform/unperform action if it isn't already prepared
	if (!actionPrepared)
	{
		if (mode != AgentMode::BACKTRACKING)
		{
			actionHistory[currentAction]->PrepareToPerform();
			animStartTime = QTime::currentTime();
		}
		else
		{
			actionHistory[currentAction]->PrepareToUnperform();
			
			// what if action has already started???
			animStartTime = QTime::currentTime();
			actionHistory[currentAction]->SetProgress(0.00f);
		}

		actionPrepared = true;
	}


	// Advance animation if animations are not paused
	if (!animationsPaused)
		actionHistory[currentAction]->SetProgress(AnimDuration() / animLength);


	// Decide whether to animate another frame of the action or complete it
	if (animationsSuppressed || actionHistory[currentAction]->AnimationSuppressed() || 
			actionHistory[currentAction]->IsFinishedAnimating())
	{
		// Animation is suppressed for this action or all actions, or the animation is finished
		// So complete action and clean it up

		if (mode == AgentMode::ON_DEMAND)
		{
			#if(DEBUG_ACTION_LEVEL >=1)
				prt("\tAbout to complete action in ON_DEMAND mode");		
				actionHistory.back()->Complete(true);
				prt("\tCompleted action");		
			#else			
				actionHistory[currentAction]->Complete(!actionHistory[currentAction]->AnimationSuppressed());
			#endif

			// Clean up action
			animStartTime = QTime();
			currentAction = INVALID;
			actionPending = false;

			performActionMutex.unlock();
			actionPendingCondVar.notify_all();
		}
		else if (mode == AgentMode::FORWARDTRACKING)
		{
			#if(DEBUG_ACTION_LEVEL >=1)
				prt("\tAbout to complete action in FORWARDTRACKING mode");		
				actionHistory.back()->Complete(true);
				prt("\tCompleted action");		
			#else			
				actionHistory[currentAction]->Complete(!actionHistory[currentAction]->AnimationSuppressed());
			#endif

			// Clean up action
			animStartTime = QTime();

			if (currentAction < actionHistory.size() - 1)
			{
				// More actions to forwardtrack
				++currentAction;
				actionPrepared = false;

				cout << "\tMore actions to forwardtrack - forwardtracking Action #" << currentAction << endl;
			}
			else
			{
				// Finished forwardtracking
				cout << "\tNo more actions to forwardtracking" << endl;
				
				currentAction = INVALID;

				mode = AgentMode::ON_DEMAND;
				actionPending = false;
				performActionMutex.unlock();
				actionPendingCondVar.notify_all();
			}
		}
		else
		{
			#if(DEBUG_ACTION_LEVEL >=1)
				prt("\tAbout to uncomplete action in BACKTRACKING mode");		
				actionHistory.back()->Uncomplete(true);
				prt("\tUncompleted action");		
			#else			
				actionHistory[currentAction]->Uncomplete(!actionHistory[currentAction]->AnimationSuppressed());
			#endif

			// Clean up action - TODO
			animStartTime = QTime();
			
			cout << "Finished backtracking Action #" << currentAction << endl;

			if (currentAction > 0)
			{
				// More actions to backtrack
				--currentAction;
				actionPrepared = false;

				cout << "\tMore actions to backtrack - backtracking Action #" << currentAction << endl;
			}
			else
			{
				// Finished backtracking
				cout << "\tNo more actions to backtrack" << endl;
				
				currentAction = INVALID;
				//backtracking = false;
				
				
				//actionPending = false;
				//performActionMutex.unlock();
				//actionPendingCondVar.notify_all();
			}
		}
	}
	else
	{
		// Perform action
		QPainter painter(this);

		if (mode != AgentMode::BACKTRACKING)
			actionHistory[currentAction]->Perform(actionHistory[currentAction]->GetProgress(), &painter);
		else
			actionHistory[currentAction]->Unperform(actionHistory[currentAction]->GetProgress(), &painter);
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
	boost::unique_lock<boost::mutex> lock(pausingMutex);

	// animStartTime is null if there isn't a pending action, or there is a pending action but it hasn't begun
	if (animStartTime.isNull())
		return;

	if (!animationsPaused)
		animsPauseTime = QTime::currentTime();
	else
		animStartTime = animStartTime.addMSecs(animsPauseTime.msecsTo(QTime::currentTime()));
	
	animationsPaused = !animationsPaused;
}

float ActionAgent::AnimDuration()
{
	return animStartTime.msecsTo(QTime::currentTime()) / 1000.0f;
}

// TODO what if user is inside PerformActionAndAnimateAsync and has performActionMutex already???
void ActionAgent::backtrack()
{
	boost::unique_lock<boost::mutex> lock(pausingMutex);
	prt("backtracking");

	UL_ASSERT(mode != AgentMode::BACKTRACKING);
	UL_ASSERT(!animationsPaused);

	if (currentAction = INVALID)
	{
		// No action is being performed, so the first action to backtrack is the last one in ActionHistory
		currentAction = actionHistory.size() - 1;
		actionPrepared = false;
	}
	else
	{
		// Action is currently being performed - should be the last one
		UL_ASSERT(currentAction == actionHistory.size() - 1);
		actionHistory[currentAction]->SetProgress(1.00f - actionHistory[currentAction]->GetProgress());
	}

	mode = AgentMode::BACKTRACKING;
	actionPending = true;
}

void ActionAgent::forwardtrack()
{
	boost::unique_lock<boost::mutex> lock(pausingMutex);
	prt("forward tracking");

	UL_ASSERT(mode == AgentMode::BACKTRACKING);
	UL_ASSERT(!animationsPaused);

	//UL_ASSERT(currentAction == INVALID);
	UL_ASSERT(actionHistory.size() > 0);

	if (currentAction == INVALID)
		currentAction = 0;


	actionHistory[currentAction]->SetProgress(0);

	mode = AgentMode::FORWARDTRACKING;

	// Should already be true...
	actionPending = true;
}



}