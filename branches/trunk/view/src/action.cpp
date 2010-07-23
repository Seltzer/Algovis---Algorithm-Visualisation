#define BOOST_ALL_NO_LIB
#include "boost/foreach.hpp"
#include "action.h"
#include "world.h"


namespace Algovis_Viewer
{



Action::Action(World* world)
	: world(world)
{
}

Action::Action(const Action& other)
	: world(other.world), subActions(other.subActions)
{
}


Action::~Action()
{
	BOOST_FOREACH(Action* subAction, subActions)
		delete subAction;
}

void Action::AddSubAction(Action& subAction)
{
	Action* subActionCopy = subAction.Clone();

	subActions.push_back(subActionCopy);
}











}