//#define BOOST_ALL_NO_LIB
#include "boost/foreach.hpp"
#include "action.h"
#include "../displayer/world.h"


namespace Algovis_Viewer
{



Action::Action(World* world, bool suppressAnimation)
	: world(world), suppressAnimation(suppressAnimation)
{
}

Action::Action(const Action& other)
	: world(other.world), suppressAnimation(other.suppressAnimation)
{
}


Action::~Action()
{
}

void Action::SuppressAnimation()
{
	suppressAnimation = true;
}


bool Action::AnimationSuppressed()
{
	return suppressAnimation;
}










}