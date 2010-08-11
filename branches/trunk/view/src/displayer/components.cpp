#include "boost/foreach.hpp"
#include "boost/thread/locks.hpp"
#include "utilities.h"
#include "components.h"

using namespace std;



namespace Algovis_Viewer
{



///////////////////////// COMPONENT //////////////////////////////////////////////////
Component::Component()
	: parentComponent(NULL)
{}
	
Component::Component(sf::FloatRect boundingBox) 
	: boundingBox(boundingBox), parentComponent(NULL) 
{}

sf::FloatRect Component::GetPreferredSize() 
{ 
	return boundingBox; 
}

sf::FloatRect Component::GetBoundingBox()
{
	return boundingBox;
}

void Component::SetBoundingBox(sf::FloatRect newBB)
{
	boundingBox = newBB;
	SetupLayout();
}

sf::Vector2f Component::GetAbsolutePosition()
{
	if (parentComponent)
	{
		sf::Vector2f myPosition(boundingBox.Left, boundingBox.Top);
		return myPosition + parentComponent->GetAbsolutePosition();
	}
	else
	{
		return sf::Vector2f(boundingBox.Left, boundingBox.Top);		
	}
}

void Component::SetParentComponent(Component* parentComponent)
{
	this->parentComponent = parentComponent;
}

Component* Component::GetParentComponent()
{
	return parentComponent;
}


void Component::NotifyObservers(ComponentEvent& eventToFire)
{
	BOOST_FOREACH(ComponentObserver* observer, observers)
		observer->Notify(this, eventToFire);

	// TODO hack to make sure components are drawn when dirty
	SetupLayout();
}

void Component::AddObserver(ComponentObserver* newObserver)
{
	unsigned oldSize = observers.size();
	observers.insert(newObserver);

	UL_ASSERT(observers.size() == oldSize + 1);
}

void Component::RemoveObserver(ComponentObserver* disgruntledObserver)
{
	observers.erase(disgruntledObserver);
}


// TODO Think about synchronisation for these
void Component::SetDrawingAgent(const void* newDrawingAgent)
{
	previousDrawingAgent = currentDrawingAgent;
	currentDrawingAgent = newDrawingAgent;
}

void Component::RestorePreviousDrawingAgent()
{
	currentDrawingAgent = previousDrawingAgent;
}

const void* Component::GetDrawingAgent()
{
	return currentDrawingAgent;
}



///////////////////////// CONTAINER //////////////////////////////////////////////////


Container::Container(sf::FloatRect boundingBox)
	: Component(boundingBox)
{
}

void Container::Draw(sf::RenderWindow& renderWindow, sf::Font& defaultFont)
{
	boost::unique_lock<boost::recursive_mutex> lock(componentsMutex);

	glTranslatef(boundingBox.Left, boundingBox.Top,0);

	BOOST_FOREACH(Component* child, childComponents)
	{
		if (child->GetDrawingAgent() == this)
			child->Draw(renderWindow, defaultFont);
	}

	glTranslatef(-boundingBox.Left, -boundingBox.Top,0);
}


void Container::AddChildComponent(Component* newComponent)
{
	boost::unique_lock<boost::recursive_mutex> lock(componentsMutex);

	newComponent->SetDrawingAgent(this);
	newComponent->SetParentComponent(this);
	childComponents.insert(newComponent);
}
		

void Container::RemoveChildComponent(Component* component)
{
	boost::unique_lock<boost::recursive_mutex> lock(componentsMutex);
	
	if (!childComponents.count(component))
		return;

	childComponents.erase(component);
	component->SetParentComponent(NULL);
	component->RestorePreviousDrawingAgent();
}



///////////////////////// PANEL //////////////////////////////////////////////////


Panel::Panel(sf::FloatRect boundingBox, float backgroundColour[3], float borderColour[3]) 
				: Container(boundingBox) 
{
	for (int i = 0; i <= 2; i++)
	{
		this->backgroundColour[i] = backgroundColour[i];
		this->borderColour[i] = borderColour[i];
	}
}


void Panel::Draw(sf::RenderWindow& renderWindow, sf::Font& defaultFont)
{
	boost::unique_lock<boost::recursive_mutex> lock(componentsMutex);

	// Draw background
	glColor3f(backgroundColour[0],backgroundColour[1],backgroundColour[2]);

	glBegin(GL_QUADS);
	glVertex2f(boundingBox.Left, boundingBox.Top);
	glVertex2f(boundingBox.Right, boundingBox.Top);
	glVertex2f(boundingBox.Right, boundingBox.Bottom);
	glVertex2f(boundingBox.Left, boundingBox.Bottom);
	glEnd();

	// Draw border
	glColor3f(borderColour[0],borderColour[1],borderColour[2]);

	glBegin(GL_LINE_LOOP);
	glVertex2f(boundingBox.Left, boundingBox.Top);
	glVertex2f(boundingBox.Right, boundingBox.Top);
	glVertex2f(boundingBox.Right, boundingBox.Bottom);
	glVertex2f(boundingBox.Left, boundingBox.Bottom);
	glEnd();


	// Draw child components
	Container::Draw(renderWindow, defaultFont);
}








}