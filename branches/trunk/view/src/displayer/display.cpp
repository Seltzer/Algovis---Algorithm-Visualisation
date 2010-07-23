#include "display.h"

#include <map>
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "boost/foreach.hpp"
#include "boost/thread/shared_mutex.hpp"
#include "boost/thread/condition_variable.hpp"
#include "utilities.h"

#include "../../include/registry.h"
#include "../world.h"
#include "../viewableObjects/viewableObject.h"
#include "../action.h"



namespace Algovis_Viewer
{


Displayer* Displayer::displayerInstance(NULL);
sf::Mutex Displayer::creationMutex;

bool Displayer::drawingEnabled(true);



Displayer::Displayer()
	: worldToBeDisplayed(NULL), actionToBePerformed(NULL), actionPending(false),
		defaultFontInitialised(false), refreshRate(60), displayerHasExclusiveLock(false)
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
	win->SetPosition(400,500);
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


		if (actionPending)
		{
			// There must be a better way to ensure this thread's attempt to reacquire a lock it already has
			// doesn't kill stuff. Look into boost recursive locks
			if (!displayerHasExclusiveLock)
			{
				worldToBeDisplayed->AcquireExclusiveLock();
				displayerHasExclusiveLock = true;
			}
							
			
			// Start performing animation for pending update (a new action)

			// Hacky mock animation which rotates stuff
			glRotatef(((float) duration / 60) * 360, 0, 0, 1);

			sf::Lock lock2(objectsMutex);
			BOOST_FOREACH(ViewableObject* obj, objectsToDraw)
				obj->Draw(*win, defaultFont);

			// Hacky mock animation has finished
			if (++duration == 60)
			{
				delete actionToBePerformed;
				actionToBePerformed = NULL;
				actionPending = false;
				worldToBeDisplayed->ReleaseExclusiveLock();
				displayerHasExclusiveLock = false;
				worldToBeDisplayed->CompletedDSAction();
			}

			
		}
		else
		{
			worldToBeDisplayed->AcquireReaderLock();
			
			// TODO: Make sure this cannot result in deadlock
			// Draw ViewableObjects
			sf::Lock lock2(objectsMutex);
			BOOST_FOREACH(ViewableObject* obj, objectsToDraw)
				obj->Draw(*win, defaultFont);

			worldToBeDisplayed->ReleaseReaderLock();
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


void Displayer::SetWorld(World* newWorld)
{
	// Clean up old world
	this->worldToBeDisplayed = newWorld;
	
	// Query world for its VOs - TODO: we might want to draw VOs other than arrays
	BOOST_FOREACH(VO_Array* vo, worldToBeDisplayed->GetRegisteredArrays())
		AddToDrawingList((ViewableObject*)vo);
}

void Displayer::AddToDrawingList(ViewableObject* newObject)
{
	sf::Lock lock(objectsMutex);
	
	UL_ASSERT(worldToBeDisplayed);

	// TODO remove hack
	UL_ASSERT(newObject->GetType() == ARRAY);

	newObject->SetBoundingBox(sf::FloatRect(50,50,0,0));
	newObject->PrepareToBeDrawn();

	objectsToDraw.insert(newObject);
}

void Displayer::RemoveFromDrawingList(ViewableObject* objectToRemove)
{
	sf::Lock lock(objectsMutex);

	UL_ASSERT(worldToBeDisplayed);
	objectsToDraw.erase(objectToRemove);
}


void Displayer::PerformAndAnimateActionAsync(Action* newAction)
{
	UL_ASSERT(actionToBePerformed == NULL);

	// ignore - for testing purposes
	duration = 0;

	actionToBePerformed = newAction->Clone();
	actionPending = true;
}


}