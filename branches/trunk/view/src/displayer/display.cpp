#include "display.h"

#include <map>


#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"

#include "utilities.h"



namespace Algovis_Viewer
{


Displayer* Displayer::displayerInstance(NULL);
bool Displayer::drawingEnabled(true);



void Displayer::RenderLoop()
{
	win = new sf::RenderWindow(sf::VideoMode(800, 600, 32), "Algovis");
	win->SetActive(true);
	glViewport(0, 0, win->GetWidth(), win->GetHeight());
	win->PreserveOpenGLStates(true);
	font.LoadFromFile("consola.ttf");

	while (win->IsOpened() && !closed)
	{
		sf::Event Event;
	    while (win->GetEvent(Event))
	    {
			// window resized
			if (Event.Type == sf::Event::Resized)
			{
				int width = Event.Size.Width;
				int height = Event.Size.Height;
				win->SetView(sf::View(sf::FloatRect(0, 0, (float) width, (float) height)));
				glViewport(0, 0, width, height);
			}

	        // Window closed
	        if (Event.Type == sf::Event::Closed)
	            win->Close();

	        // Escape key pressed
	        if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Escape))
	            win->Close();
	    }

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, win->GetWidth(), win->GetHeight(), 0, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		// Talk to the VO_Array instances

		win->Display();

		sf::Sleep(1.0f / 60);
	}
}


/*
void Draw(const void* dataStructure, const std::vector<std::string>& data)
{
	if (Displayer::DrawingEnabled())
	{
		Displayer::GetInstance()->Draw(dataStructure, data);
	}
}

*/



}