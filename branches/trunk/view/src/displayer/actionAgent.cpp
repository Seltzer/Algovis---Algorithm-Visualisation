#include "boost/foreach.hpp"
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
			mode(AgentMode::ON_DEMAND), animLength(0.5f), animationsSuppressed(false), 
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

	BOOST_FOREACH(Action* action, actionHistory)
		delete action;
}


// See comments about shutting down in ~ActionAgent
void ActionAgent::PerformAndAnimateActionAsync(const Action* newAction)
{
	// First shuttingDown checkpoint
	if (shuttingDown)
		return;
	else
		--performActionCount;
			

	// Matched by unlock inside paintEvent()
	performActionMutex.lock();
	while(actionPending)
		actionPendingCondVar.wait<boost::mutex>(performActionMutex);

	if (shuttingDown)
	{
		--performActionCount;
		return;
	}

	#if(DEBUG_ACTION_LEVEL >=1)
		prt("About to lodge action in ActionAgent::actionHistory");		
	#endif


	Action* clone = newAction->Clone();
	actionHistory.push_back(clone);
	currentAction = actionHistory.size() - 1;

	// Do this last
	actionPending = true;


	--performActionCount;
}


void ActionAgent::paintEvent(QPaintEvent*)
{
	boost::unique_lock<boost::mutex> lock(modeMutex);

	// If there's no action pending, we have nothing to do
	if (!actionPending)
		return;

	// currentAction is INVALID if we're right at the front or back of actionHistory
	// So if currentAction == INVALID, we have nothing to do
	if (currentAction == INVALID)
		return;

	// Prepare action for performing/unperforming
	PrepareCurrentAction();
	

	bool paused = (mode & AgentMode::PAUSED) == AgentMode::PAUSED;

	// Advance animation if not in paused mode
	if (!paused)
		actionHistory[currentAction]->SetProgress(AnimDuration() / animLength);

	// Decide whether to animate another frame of the action or complete it
	if ( !paused && (animationsSuppressed || actionHistory[currentAction]->AnimationSuppressed() || 
			actionHistory[currentAction]->IsFinishedAnimating()) )
	{
		// Animation is suppressed for this action or all actions, or the animation is finished
		// So, complete the action
		FinishCurrentAction();
	}
	else
	{
		DrawCurrentFrame(&QPainter(this));
	}
}

void ActionAgent::PrepareCurrentAction()
{
	if ((mode & AgentMode::BACKTRACKING) == AgentMode::BACKTRACKING)
	{
		if (actionHistory[currentAction]->state != Action::UNPREPARED)
		{
			actionHistory[currentAction]->PrepareToUnperform();

			animStartTime = QTime::currentTime();
			actionHistory[currentAction]->SetProgress(0.00f);


			#if(DEBUG_ACTION_LEVEL >=1)
				prt("Unprepared current action inside ActionAgent");		
			#endif
		}
		
	}
	else
	{
		if (actionHistory[currentAction]->state != Action::PREPARED)
		{
			actionHistory[currentAction]->PrepareToPerform();
			actionHistory[currentAction]->state = Action::PREPARED;

			animStartTime = QTime::currentTime();

			#if(DEBUG_ACTION_LEVEL >=1)
				prt("Prepared current action inside ActionAgent");		
			#endif
		}
	}
}

void ActionAgent::DrawCurrentFrame(QPainter* painter)
{
	if ((mode & AgentMode::BACKTRACKING) == AgentMode::BACKTRACKING)
		actionHistory[currentAction]->Unperform(actionHistory[currentAction]->GetProgress(), painter);
	else
		actionHistory[currentAction]->Perform(actionHistory[currentAction]->GetProgress(), painter);
}

void ActionAgent::FinishCurrentAction()
{
	animStartTime = QTime();

	if ((mode & AgentMode::ON_DEMAND) == AgentMode::ON_DEMAND)
	{
		#if(DEBUG_ACTION_LEVEL >=1)
			prt("\tAbout to complete action in ON_DEMAND mode");		
			actionHistory[currentAction]->Complete(!actionHistory[currentAction]->AnimationSuppressed());
			prt("\tCompleted action");		
		#else			
			actionHistory[currentAction]->Complete(!actionHistory[currentAction]->AnimationSuppressed());
		#endif

		currentAction = INVALID;

		actionPending = false;
		performActionMutex.unlock();
		actionPendingCondVar.notify_all();
	}
	else if ((mode & AgentMode::BACKTRACKING) == AgentMode::BACKTRACKING)
	{
		#if(DEBUG_ACTION_LEVEL >= 1)
			cout << "\tAbout to uncomplete Action #" << currentAction << " in BACKTRACKING mode" << endl;
			actionHistory[currentAction]->Uncomplete(!actionHistory[currentAction]->AnimationSuppressed());
			prt("\tUncompleted action");		
		#else			
			actionHistory[currentAction]->Uncomplete(!actionHistory[currentAction]->AnimationSuppressed());
		#endif


		if (currentAction > 0)
		{
			// More actions left to backtrack
			--currentAction;
			#if(DEBUG_ACTION_LEVEL >=1)
				cout << "\tMore actions to backtrack - backtracking Action #" << currentAction << endl;
			#endif
		}
		else
		{
			currentAction = INVALID;

			// Finished backtracking
			#if(DEBUG_ACTION_LEVEL >=1)
				cout << "\tNo more actions to backtrack" << endl;
			#endif
		}
	}
	else if ((mode & AgentMode::FORWARDTRACKING) == AgentMode::FORWARDTRACKING)
	{
		#if(DEBUG_ACTION_LEVEL >=1)
			cout << "\tAbout to complete Action #" << currentAction << " in FORWARDTRACKING mode" << endl;
			actionHistory[currentAction]->Complete(!actionHistory[currentAction]->AnimationSuppressed());
			prt("\tCompleted action");		
		#else			
			actionHistory[currentAction]->Complete(!actionHistory[currentAction]->AnimationSuppressed());
		#endif

		if (currentAction < actionHistory.size() - 1)
		{
			// More actions left to forwardtrack
			++currentAction;

			#if(DEBUG_ACTION_LEVEL >=1)
				cout << "\tMore actions to forwardtrack - forwardtracking Action #" << currentAction << endl;
			#endif
		}
		else
		{
			// Finished forwardtracking
			#if(DEBUG_ACTION_LEVEL >=1)
				cout << "\tNo more actions to forwardtrack - setting mode to ON_DEMAND" << endl;
			#endif
			
			currentAction = INVALID;
			mode = AgentMode::ON_DEMAND;

			actionPending = false;
			performActionMutex.unlock();
			actionPendingCondVar.notify_all();
		}
	}
	else
	{
		UL_ASSERT(false);
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

float ActionAgent::AnimDuration()
{
	return animStartTime.msecsTo(QTime::currentTime()) / 1000.0f;
}

void ActionAgent::pauseResumeAnimations()
{
	boost::unique_lock<boost::mutex> lock(modeMutex);

	// animStartTime is null if there isn't a pending action, or there is a pending action but it hasn't begun
	if (animStartTime.isNull())
		return;

	if ((mode & AgentMode::PAUSED) == AgentMode::PAUSED)
		animStartTime = animStartTime.addMSecs(animsPauseTime.msecsTo(QTime::currentTime()));
	else
		animsPauseTime = QTime::currentTime();

	// Toggle PAUSED mode
	mode = (AgentMode) (mode ^ AgentMode::PAUSED);
}



// TODO what if user is inside PerformActionAndAnimateAsync and has performActionMutex already???
void ActionAgent::backtrack()
{
	boost::unique_lock<boost::mutex> lock(modeMutex);

	// Return if already backtracking
	if ((mode & AgentMode::BACKTRACKING) == AgentMode::BACKTRACKING)
		return;
	

	if (currentAction == INVALID)
	{
		// No action is being performed, so the first action to backtrack is the last one in ActionHistory
		currentAction = actionHistory.size() - 1;
	}
	else
	{
		// Action is currently being performed - should be the last one
		UL_ASSERT(currentAction == actionHistory.size() - 1);

		// This allows us to backtrack through the action currently being performed
		//actionHistory[currentAction]->SetProgress(1.00f - actionHistory[currentAction]->GetProgress());
		
		// On the other hand, it saves us many headaches if we start backtracking at action before the current
		currentAction = actionHistory.size() - 2;
	}

	// Set mode to BACKTRACKING while preserving PAUSED mode
	mode = (AgentMode) (AgentMode::BACKTRACKING | (mode & AgentMode::PAUSED));
	actionPending = true;
}

void ActionAgent::forwardtrack()
{
	boost::unique_lock<boost::mutex> lock(modeMutex);

	// Must be in backtracking mode
	if (! ((mode & AgentMode::BACKTRACKING) == AgentMode::BACKTRACKING))
		return;

	UL_ASSERT(actionHistory.size() > 0);

	if (currentAction == INVALID)
		currentAction = 0;

	actionHistory[currentAction]->SetProgress(0);

	// Set mode to FORWARDTRACKING while preserving PAUSED mode
	mode = (AgentMode) (AgentMode::FORWARDTRACKING | (mode & AgentMode::PAUSED));

	// Should already be true...
	actionPending = true;
}



}