#define BOOST_ALL_NO_LIB
#include "boost/foreach.hpp"
#include "utilities.h"
#include "viewableObject.h"



namespace Algovis_Viewer
{
	
ViewableObject::ViewableObject(const void* dsAddress, World* world) 
	: dsAddress(dsAddress), world(world), suppressed(false) 
{
	boundingBoxColour[0] = boundingBoxColour[1] = boundingBoxColour[2] = 1;
}

ViewableObject::~ViewableObject()
{
	NotifyObservers(BEING_DESTROYED);
}

void ViewableObject::SetOwner(ViewableObject* owner)
{
	this->owner = owner;
}


sf::FloatRect ViewableObject::GetPreferredSize() 
{ 
	return boundingBox; 
}

sf::FloatRect ViewableObject::GetBoundingBox()
{
	return boundingBox;
}

void ViewableObject::SetBoundingBox(sf::FloatRect newBB)
{
	boundingBox = newBB;
	
	PrepareToBeDrawn();
}

void ViewableObject::SetBoundingBoxColour(float r,float g,float b)
{
	boundingBoxColour[0] = r;
	boundingBoxColour[1] = g;
	boundingBoxColour[2] = b;
}


void ViewableObject::NotifyObservers(NOTIFY_EVENT_TYPE eventType)
{
	BOOST_FOREACH(IViewableObjectObserver* observer, observers)
		observer->Notify(this, eventType);

	// TODO hack to make sure VOs are drawn when dirty
	PrepareToBeDrawn();
}

void ViewableObject::AddObserver(IViewableObjectObserver* newObserver)
{
	unsigned oldSize = observers.size();
	observers.insert(newObserver);

	UL_ASSERT(observers.size() == oldSize + 1);
}

void ViewableObject::RemoveObserver(IViewableObjectObserver* disgruntledObserver)
{
	observers.erase(disgruntledObserver);
}






}

