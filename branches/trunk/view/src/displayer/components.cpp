#include "components.h"


namespace Algovis_Viewer
{


///////////////////////// COMPONENT //////////////////////////////////////////////////
Component::Component() {}
	
Component::Component(sf::FloatRect boundingBox) : boundingBox(boundingBox) {}

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
}

void Component::SetParentComponent(Component* parentComponent)
{
	this->parentComponent = parentComponent;
}



///////////////////////// PANEL //////////////////////////////////////////////////


Panel::Panel(sf::FloatRect boundingBox, float backgroundColour[3], float borderColour[3]) 
				: Component(boundingBox) 
{
	for (int i = 0; i <= 2; i++)
	{
		this->backgroundColour[i] = backgroundColour[i];
		this->borderColour[i] = borderColour[i];
	}
}


void Panel::Draw(sf::RenderWindow& renderWindow, sf::Font& defaultFont)
{
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
}








}