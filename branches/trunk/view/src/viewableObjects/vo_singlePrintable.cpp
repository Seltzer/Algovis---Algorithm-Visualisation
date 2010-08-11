#include "vo_singlePrintable.h"
#include "../displayer/display.h"
#include "../displayer/events.h"



namespace Algovis_Viewer
{


sf::FloatRect VO_SinglePrintable::GetPreferredSize()
{
	sf::String text(value.c_str(), Displayer::GetInstance()->GetDefaultFont());
	return text.GetRect();
}


void VO_SinglePrintable::SetupLayout()
{
	delete graphicalText;
	graphicalText = new sf::String(value.c_str(), Displayer::GetInstance()->GetDefaultFont());
	graphicalText->SetColor(sf::Color(255,255,255));
	graphicalText->SetPosition(0,0);
}


void VO_SinglePrintable::Draw(sf::RenderWindow& renderWindow, sf::Font& defaultFont)
{
	// Draw value component(text)
	sf::Vector2f absPosition = GetAbsolutePosition();
	graphicalText->Move(absPosition.x, absPosition.y);
	renderWindow.Draw(*graphicalText);
	graphicalText->Move(-absPosition.x, -absPosition.y);

	// Draw non-value component (border)
	DrawWithoutValue(renderWindow, defaultFont);
}

void VO_SinglePrintable::DrawValue(sf::FloatRect& desiredBoundingBox, 
									sf::RenderWindow& renderWindow, sf::Font& defaultFont)
{
	sf::Vector2f oldPosition = graphicalText->GetPosition();
	float xScale = desiredBoundingBox.GetWidth() / graphicalText->GetRect().GetWidth();
	float yScale = desiredBoundingBox.GetHeight() / graphicalText->GetRect().GetHeight();
	
	graphicalText->SetPosition(desiredBoundingBox.Left, desiredBoundingBox.Top);
	graphicalText->SetScale(xScale, yScale);
	renderWindow.Draw(*graphicalText);

	// Restore position and dimensions
	graphicalText->SetScale(1/xScale, 1/yScale);
	graphicalText->SetPosition(oldPosition);
}

void VO_SinglePrintable::DrawWithoutValue(sf::RenderWindow&, sf::Font& defaultFont)
{
	// Draw bounding box
	glColor3f(boundingBoxColour[0],boundingBoxColour[1],boundingBoxColour[2]);

	glBegin(GL_LINE_LOOP);
	glVertex2f(boundingBox.Left, boundingBox.Top);
	glVertex2f(boundingBox.Right, boundingBox.Top);
	glVertex2f(boundingBox.Right, boundingBox.Bottom);
	glVertex2f(boundingBox.Left, boundingBox.Bottom);
	glEnd();
}


void VO_SinglePrintable::UpdateValue(const std::string& newValue)
{ 
	//if (value != newValue) // Update should be displayed, with history, even if the same value is assigned!
	{
		value = newValue;
	
		ComponentEvent eventToFire(UPDATED_VALUE);
		SetupLayout();
		NotifyObservers(eventToFire);
	}
}



}