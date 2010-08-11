#include "display.h"

#include "SFML/Window.hpp"
#include "boost/foreach.hpp"
#include "utilities.h"

#include "../world.h"
#include "../viewableObjects/viewableObject.h"
#include "../action.h"



namespace Algovis_Viewer
{


///////////////////////// Displayer Construction / Destruction / Singleton Stuff
Displayer* Displayer::displayerInstance(NULL);
sf::Mutex Displayer::creationMutex;

Displayer::Displayer()
	: worldToBeDisplayed(NULL), actionToBePerformed(NULL), actionPending(false), view(NULL),
		defaultFontInitialised(false), refreshRate(60)
{
	#if (DEBUG_GENERAL_LEVEL >= 2)
		prt("DISPLAYER CTOR");
	#endif

	closed = false;

	SetupPanels();
	renderThread = new sf::Thread(&RenderThread, this);
	renderThread->Launch();
}


Displayer::~Displayer()
{
	closed = true;
	if (!view)
		delete view;


	delete renderThread;
	delete controlPanel;
	delete worldPanel;
}


Displayer* Displayer::GetInstance()
{
	sf::Lock lock(creationMutex);

	if (!Displayer::displayerInstance)
		Displayer::displayerInstance = new Displayer();
	
	return Displayer::displayerInstance;
}

void Displayer::SetupPanels()
{
	float bgColour[3] = {0,0,0};
	float borderColour[3] = {1,0,0};
	
	worldPanel = new Panel(sf::FloatRect(1,1,799,500), bgColour, borderColour);
	controlPanel = new Panel(sf::FloatRect(1,501,799,599), bgColour, borderColour);
}


void Displayer::InitWindow()
{
	win = new sf::RenderWindow(sf::VideoMode(800, 600, 32), "Algovis");
	win->SetPosition(200,200);
	win->SetActive(true);
	glViewport(0, 0, win->GetWidth(), win->GetHeight());
	win->PreserveOpenGLStates(true);
	defaultFont.LoadFromFile("consola.ttf");
	defaultFontInitialised = true;

	#if (DEBUG_GENERAL_LEVEL >= 1)
		prt("Default font initialised");
	#endif
}



///////////////////////// Other Displayer stuff

bool Displayer::drawingEnabled(true);

void Displayer::RenderLoop()
{
	InitWindow();
		
	while (win->IsOpened() && !closed)
	{
		HandleEvents();
			
		// Don't draw if disabled/minimised
		if (!Displayer::drawingEnabled)
			continue;

		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, win->GetWidth(), win->GetHeight(), 0, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw panels
		controlPanel->Draw(*win, defaultFont);
		DrawWorldPanel();
			
		// Display and sleep
		win->Display();
		sf::Sleep(1.0f / refreshRate);
	}
}

void Displayer::HandleEvents()
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
			
			// Out with the old view, in with the new
			if (!view)
				delete view;
			view = new sf::View(sf::FloatRect(0,0,(float) width, (float) height));
			win->SetView(*view);
		
			glViewport(0, 0, width, height);
			
			if (!width && !height)
				EnableDrawing(false);
			else
				EnableDrawing(true);
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
}


void Displayer::DrawWorldPanel()
{
	if (!actionPending)
	{
		worldToBeDisplayed->AcquireReaderLock(true);
		worldPanel->Draw(*win, defaultFont);
		worldToBeDisplayed->ReleaseReaderLock();
	}
	else
	{
		worldToBeDisplayed->AcquireWriterLock();

		worldPanel->Draw(*win, defaultFont);
		actionToBePerformed->Perform((float)duration / 60, *win, defaultFont);

		// Animation has finished
		if (++duration == 60)
		{
			actionToBePerformed->Complete(true);
			delete actionToBePerformed;
			actionToBePerformed = NULL;
			actionPending = false;

			// TODO: Consider of order of write lock release and CompletedDSAction()\notify_all()
			worldToBeDisplayed->ReleaseWriterLock();
			worldToBeDisplayed->CompletedDSAction();
		}
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
	// TODO: Clean up old world
	this->worldToBeDisplayed = newWorld;
	
	// Query world for its VOs - TODO: we might want to draw VOs other than arrays
	BOOST_FOREACH(VO_Array* vo, worldToBeDisplayed->GetRegisteredArrays())
		AddToDrawingList((ViewableObject*)vo);
}

void Displayer::AddToDrawingList(ViewableObject* newViewable)
{
	newViewable->SetBoundingBox(sf::FloatRect(50,50,0,0));
	newViewable->SetupLayout();

	worldPanel->AddChildComponent(newViewable);
}

void Displayer::RemoveFromDrawingList(ViewableObject* viewableToRemove)
{
	worldPanel->RemoveChildComponent(viewableToRemove);
}


void Displayer::PerformAndAnimateActionAsync(Action* newAction)
{
	while(actionPending);

	duration = 0;

	actionToBePerformed = newAction->Clone();
	actionToBePerformed->PrepareToPerform();
	actionPending = true;
}


}