#include "vo_singlePrintable.h"



namespace Algovis_Viewer
{


sf::FloatRect VO_SinglePrintable::GetPreferredSize()
{
	sf::String text(value.c_str(), Displayer::GetInstance()->GetDefaultFont());
	return text.GetRect();
}


void VO_SinglePrintable::PrepareToBeDrawn()
{
	delete graphicalText;
	graphicalText = new sf::String(value.c_str(), Displayer::GetInstance()->GetDefaultFont());
	graphicalText->SetColor(sf::Color(255,255,255));
	graphicalText->SetPosition(boundingBox.Left, boundingBox.Top);
}


void VO_SinglePrintable::Draw(sf::RenderWindow& renderWindow, sf::Font& defaultFont)
{
	// Draw text
	renderWindow.Draw(*graphicalText);

	// Draw bounding box
	glColor3f(boundingBoxColour[0],boundingBoxColour[1],boundingBoxColour[2]);

	glBegin(GL_LINE_LOOP);
	glVertex2f(boundingBox.Left, boundingBox.Top);
	glVertex2f(boundingBox.Right, boundingBox.Top);
	glVertex2f(boundingBox.Right, boundingBox.Bottom);
	glVertex2f(boundingBox.Left, boundingBox.Bottom);
	glEnd();
}




}