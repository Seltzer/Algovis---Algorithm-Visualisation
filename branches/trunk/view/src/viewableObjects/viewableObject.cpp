#define BOOST_ALL_NO_LIB
#include "boost/foreach.hpp"
#include "utilities.h"
#include "viewableObject.h"



namespace Algovis_Viewer
{
	
ViewableObject::ViewableObject(const void* dsAddress, World* world) 
	: dsAddress(dsAddress), world(world), parent(NULL), previousDrawingAgent(NULL), currentDrawingAgent(NULL),
		suppressed(false) 
{
	boundingBoxColour[0] = boundingBoxColour[1] = boundingBoxColour[2] = 1;
}

ViewableObject::~ViewableObject()
{
	NotifyObservers(BEING_DESTROYED);
}

void ViewableObject::SetParent(ViewableObject* parent)
{
	this->parent = parent;
}


// TODO Think about synchronisation for these
void ViewableObject::SetDrawingAgent(const void* newDrawingAgent)
{
	previousDrawingAgent = currentDrawingAgent;
	currentDrawingAgent = newDrawingAgent;
}

void ViewableObject::RestorePreviousDrawingAgent()
{
	currentDrawingAgent = previousDrawingAgent;
}

const void* ViewableObject::GetDrawingAgent()
{
	return currentDrawingAgent;
}




void ViewableObject::SetBoundingBox(sf::FloatRect newBB)
{
	Component::SetBoundingBox(newBB);
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

