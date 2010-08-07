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


///////////////////////// Construction / Destruction / Singleton Stuff
Displayer* Displayer::displayerInstance(NULL);
sf::Mutex Displayer::creationMutex;


Displayer::Displayer()
	: worldToBeDisplayed(NULL), actionToBePerformed(NULL), actionPending(false),
		defaultFontInitialised(false), refreshRate(60)//, displayerHasExclusiveLock(false)
{
	#if (DEBUG_GENERAL_LEVEL >= 2)
		prt("DISPLAYER CTOR");
	#endif

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

	#if (DEBUG_GENERAL_LEVEL >= 1)
		prt("Default font initialised");
	#endif
}



///////////////////////// Other stuff

bool Displayer::drawingEnabled(true);

void Displayer::RenderLoop()
{
	static bool minimised = false;

	InitWindow();
	
	while (win->IsOpened() && !closed)
	{
		sf::Event Event;
	    while (win->GetEvent(Event))
	    {
			// window resized
			if (Event.Type == sf::Event::Resized)
			{
				#if (DEBUG_GENERAL_LEVEL >= 1)
					prt("window resized");
				#endif
				int width = Event.Size.Width;
				int height = Event.Size.Height;
				win->SetView(sf::View(sf::FloatRect(0, 0, (float) width, (float) height)));

				glViewport(0, 0, width, height);
				
				if (!width && !height)
					minimised = true;
				else
					minimised = false;
			}

	        // Window closed
	        if (Event.Type == sf::Event::Closed)
			{
				#if (DEBUG_GENERAL_LEVEL >= 1)
					prt("window closed via x")
				#endif
				win->Close();
			}
	            

	        // Escape key pressed
	        if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Escape))
			{
				#if (DEBUG_GENERAL_LEVEL >= 1)
					prt("window closed via ESC key")
				#endif
				win->Close();
			}
	            
	    }

		if (minimised)
			continue;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, win->GetWidth(), win->GetHeight(), 0, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

	

		if (actionPending)
		{
			worldToBeDisplayed->AcquireWriterLock();
						
			// Hacky mock animation which rotates stuff
			glRotatef(((float) duration / 60) * 360, 0, 0, 1);

			sf::Lock drawingLock(viewablesMutex);
			BOOST_FOREACH(ViewableObject* obj, viewablesToDraw)
				obj->Draw(*win, defaultFont);

			// Hacky mock animation has finished
			if (++duration == 60)
			{
				actionToBePerformed->Complete();
				delete actionToBePerformed;
				actionToBePerformed = NULL;
				actionPending = false;
				
				worldToBeDisplayed->ReleaseWriterLock();
				worldToBeDisplayed->CompletedDSAction();
			}
		}
		else
		{
			// TODO: Make sure acquisition of VO reader lock and viewables mutex cannot result in deadlock
			worldToBeDisplayed->AcquireReaderLock();
			sf::Lock viewablesLock(viewablesMutex);

			// Draw ViewableObjects
			BOOST_FOREACH(ViewableObject* obj, viewablesToDraw)
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

void Displayer::AddToDrawingList(ViewableObject* newViewable)
{
	sf::Lock viewablesLock(viewablesMutex);
	
	UL_ASSERT(worldToBeDisplayed);

	// TODO remove hack
	UL_ASSERT(newViewable->GetType() == ARRAY);

	newViewable->SetBoundingBox(sf::FloatRect(50,50,0,0));
	newViewable->PrepareToBeDrawn();

	viewablesToDraw.insert(newViewable);
}

void Displayer::RemoveFromDrawingList(ViewableObject* viewableToRemove)
{
	sf::Lock viewablesLock(viewablesMutex);

	UL_ASSERT(worldToBeDisplayed);
	if (viewablesToDraw.count(viewableToRemove))
		viewablesToDraw.erase(viewableToRemove);
}


void Displayer::PerformAndAnimateActionAsync(Action* newAction)
{
	while(actionPending);


	// ignore - this is a hack
	duration = 0;

	actionToBePerformed = newAction->Clone();
	actionPending = true;
}


}