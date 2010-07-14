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
	graphicalText = sf::String(value.c_str(), Displayer::GetInstance()->GetDefaultFont());
	graphicalText.SetColor(sf::Color(255,255,255));
	graphicalText.SetPosition(xPos, yPos);
}


void VO_SinglePrintable::Draw(sf::RenderWindow& renderWindow, sf::Font& defaultFont)
{
	// Draw text
	renderWindow.Draw(graphicalText);

	// Draw bounding box
	glColor3f(1, 1, 1);
	glBegin(GL_LINE_LOOP);
	glVertex2f(boundingBox.Left, boundingBox.Top);
	glVertex2f(boundingBox.Right, boundingBox.Top);
	glVertex2f(boundingBox.Right, boundingBox.Bottom);
	glVertex2f(boundingBox.Left, boundingBox.Bottom);
	glEnd();
}




}