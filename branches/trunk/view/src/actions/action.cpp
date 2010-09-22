#include "boost/foreach.hpp"
#include "action.h"




namespace Algovis_Viewer
{
 


Action::Action(World* world, bool suppressAnimation)
	: world(world), suppressAnimation(suppressAnimation), state(READY),
		preparedAtLeastOnce(false), completedAtLeastOnce(false), progress(0.0f)
{
}

Action::Action(const Action& other)
	: world(other.world), suppressAnimation(other.suppressAnimation), state(other.state),
		preparedAtLeastOnce(other.preparedAtLeastOnce), completedAtLeastOnce(other.completedAtLeastOnce), 
			progress(other.progress), completeTime(other.completeTime)
{
}


Action::~Action()
{
}

void Action::PrepareToPerform()
{
	preparedAtLeastOnce = true;
}

void Action::Complete(bool displayed)
{
	completedAtLeastOnce = true;
}

void Action::SuppressAnimation()
{
	suppressAnimation = true;
}

bool Action::AnimationSuppressed()
{
	return suppressAnimation;
}

float Action::GetProgress()
{
	return progress;
}

void Action::SetProgress(float newProgress)
{
	progress = newProgress;
}

bool Action::IsFinishedAnimating()
{
	return progress >= 1.00f;
}

void Action::UpdateHistory(HistoryManager& historyManager)
{
	completeTime = historyManager.GetTime();
	historyManager.ActionProcessed();
}









}