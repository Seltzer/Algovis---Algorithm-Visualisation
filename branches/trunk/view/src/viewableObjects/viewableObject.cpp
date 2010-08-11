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
	ComponentEvent eventToFire(BEING_DESTROYED);
	NotifyObservers(eventToFire);
}

void ViewableObject::SetBoundingBoxColour(float r,float g,float b)
{
	boundingBoxColour[0] = r;
	boundingBoxColour[1] = g;
	boundingBoxColour[2] = b;
}


}

