#include "boost/foreach.hpp"
#include "action.h"




namespace Algovis_Viewer
{
 


Action::Action(World* world, bool suppressAnimation)
	: world(world), suppressAnimation(suppressAnimation), completedAtLeastOnce(false), progress(0.0f)
{
}

Action::Action(const Action& other)
	: world(other.world), suppressAnimation(other.suppressAnimation), 
		completedAtLeastOnce(other.completedAtLeastOnce), progress(other.progress)
{
}


Action::~Action()
{
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











}