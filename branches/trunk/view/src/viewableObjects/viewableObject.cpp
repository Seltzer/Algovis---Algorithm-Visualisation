#include "boost/foreach.hpp"
#include "utilities.h"
#include "viewableObject.h"



namespace Algovis_Viewer
{
	
	

void ViewableObject::SetPosition(float x, float y)
{
	xPos = x; yPos = y;
	boundingBox.Offset(x - boundingBox.Left, y - boundingBox.Top);

	PrepareToBeDrawn();
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
	xPos = boundingBox.Left;
	yPos = boundingBox.Top;
	
	PrepareToBeDrawn();
}

void ViewableObject::SetBoundingBoxColour(float r,float g,float b)
{
	boundingBoxColour[0] = r;
	boundingBoxColour[1] = g;
	boundingBoxColour[2] = b;
}


void ViewableObject::NotifyObservers()
{
	BOOST_FOREACH(IViewableObjectObserver* observer, observers)
		observer->Notify(this);

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

