#include "boost/foreach.hpp"
#include "action.h"
#include "../../include/registry.h"



namespace Algovis_Viewer
{
 


Action::Action(World* world, bool suppressAnimation)
	: world(world), suppressAnimation(suppressAnimation), state(READY),
		preparedAtLeastOnce(false), completedAtLeastOnce(false), progress(0.0f)
{
	reg = Registry::GetInstance();
}

Action::Action(const Action& other)
	: world(other.world), reg(other.reg), suppressAnimation(other.suppressAnimation), state(other.state),
		preparedAtLeastOnce(other.preparedAtLeastOnce), completedAtLeastOnce(other.completedAtLeastOnce), 
			progress(other.progress), completeTime(other.completeTime)
{
}


Action::~Action()
{
}

void Action::PrepareToPerform()
{
	progress = 0;
	state = Action::PREPARED;
	preparedAtLeastOnce = true;
}

void Action::PrepareToUnperform()
{
	progress = 0;
	state = Action::UNPREPARED;
}

void Action::Complete(bool displayed)
{
	state = COMPLETED;
	completedAtLeastOnce = true;
}

void Action::Uncomplete(bool displayed)
{
	state = READY;
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