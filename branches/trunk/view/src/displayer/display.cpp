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



Displayer::Displayer()
	: defaultFontInitialised(false), refreshRate(60)
{
	closed = false;
	renderThread = new sf::Thread(&RenderThread, this);
	renderThread->Launch();
}


Displayer::~Displayer()
{
	closed = true;
	delete renderThread;
}


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

	#ifdef DEBUG_VERBOSE
		prt("Default font initialised");
	#endif
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
				#ifdef DEBUG_VERBOSE
					prt("window resized");
				#endif
				int width = Event.Size.Width;
				int height = Event.Size.Height;
				win->SetView(sf::View(sf::FloatRect(0, 0, (float) width, (float) height)));
				glViewport(0, 0, width, height);
			}

	        // Window closed
	        if (Event.Type == sf::Event::Closed)
			{
				#ifdef DEBUG_VERBOSE
					prt("window closed via x")
				#endif
				win->Close();
			}
	            

	        // Escape key pressed
	        if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Escape))
			{
				#ifdef DEBUG_VERBOSE
					prt("window closed via ESC key")
				#endif
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

		sf::Sleep(1.0f / refreshRate);
	}
}

void Displayer::RenderThread(void* dispPtr)
{
	static_cast<Displayer*>(dispPtr)->RenderLoop();
}





sf::Font& Displayer::GetDefaultFont()
{ 
	while (!defaultFontInitialised);

	return defaultFont; 
}

void Displayer::SetRefreshRate(unsigned refreshRate)
{
	this->refreshRate = refreshRate;
}

void Displayer::AddToDrawingList(ViewableObject* newObject)
{
	sf::Lock lock(objectsMutex);
		
	// TODO remove hack
	UL_ASSERT(newObject->GetType() == ARRAY);

	newObject->SetBoundingBox(sf::FloatRect(50,50,0,0));
	newObject->PrepareToBeDrawn();

	objectsToDraw.insert(newObject);
}

void Displayer::RemoveFromDrawingList(ViewableObject* objectToRemove)
{
	sf::Lock lock(objectsMutex);
	objectsToDraw.erase(objectToRemove);
}

}