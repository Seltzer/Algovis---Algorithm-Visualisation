#include "display.h"

#include <map>
#include "boost/foreach.hpp"
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "utilities.h"
#include "../../include/registry.h"
#include "../viewableObjects/viewableObject.h"






namespace Algovis_Viewer
{


Displayer* Displayer::displayerInstance(NULL);
sf::Mutex Displayer::creationMutex;

bool Displayer::drawingEnabled(true);


Displayer* Displayer::GetInstance()
{
	sf::Lock lock(creationMutex);

	if (!Displayer::displayerInstance)
		Displayer::displayerInstance = new Displayer();

	return Displayer::displayerInstance;
}



void Displayer::InitWindow()
{
	win = new sf::RenderWindow(sf::VideoMode(800, 600, 32), "Algovis");
	win->SetActive(true);
	glViewport(0, 0, win->GetWidth(), win->GetHeight());
	win->PreserveOpenGLStates(true);
	defaultFont.LoadFromFile("consola.ttf");
	defaultFontInitialised = true;
	prt("Default font initialised");
}


void Displayer::RenderLoop()
{
	InitWindow();

	while (win->IsOpened() && !closed)
	{
		sf::Event Event;
	    while (win->GetEvent(Event))
	    {
			// window resized
			if (Event.Type == sf::Event::Resized)
			{
				prt("window resized");
				int width = Event.Size.Width;
				int height = Event.Size.Height;
				win->SetView(sf::View(sf::FloatRect(0, 0, (float) width, (float) height)));
				glViewport(0, 0, width, height);
			}

	        // Window closed
	        if (Event.Type == sf::Event::Closed)
			{
				prt("window closed via x")
				win->Close();
			}
	            

	        // Escape key pressed
	        if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Escape))
			{
				prt("window closed via ESC key")
				win->Close();
			}
	            
	    }

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, win->GetWidth(), win->GetHeight(), 0, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		{
			// Draw ViewableObjects
			sf::Lock lock(objectsMutex);
			BOOST_FOREACH(ViewableObject* obj, objectsToDraw)
				obj->Draw(*win, defaultFont);
		}

		win->Display();

		sf::Sleep(1.0f / 60);
	}
}

void Displayer::RenderThread(void* dispPtr)
{
	static_cast<Displayer*>(dispPtr)->RenderLoop();
}


void Displayer::AddToDrawingList(ViewableObject* newObject)
{
	sf::Lock lock(objectsMutex);
		
	// TODO remove hack
	UL_ASSERT(newObject->GetType() == ARRAY);

	newObject->SetPosition(50,50);
	newObject->PrepareToBeDrawn();

	objectsToDraw.insert(newObject);
}

void Displayer::RemoveFromDrawingList(ViewableObject* objectToRemove)
{
	sf::Lock lock(objectsMutex);
	objectsToDraw.erase(objectToRemove);
}


sf::Font& Displayer::GetDefaultFont()
{ 
	// TODO - get rid of this busy wait which runs until the renderThread has initialised the default font
	while (!defaultFontInitialised);

	return defaultFont; 
}



}