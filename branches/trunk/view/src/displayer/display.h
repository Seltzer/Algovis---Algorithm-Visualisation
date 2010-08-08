#ifndef DISPLAY_H
#define DISPLAY_H


#include <set>
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "utilities.h"
#include "../dsAction.h"
#include "components.h"




namespace Algovis_Viewer
{


class ViewableObject;
class World;
class Action;

class Displayer
{

public:
	// Public static methods
	static Displayer* GetInstance();
	static void EnableDrawing(bool enabled) { Displayer::drawingEnabled = enabled; }
	static bool DrawingEnabled() { return Displayer::drawingEnabled; }

	void SetRefreshRate(unsigned);

	// Busy waits until InitWindow() has finished initialising defaultFont - TODO: get rid of busy-wait
	sf::Font& GetDefaultFont();

	// Post-Condition: All VO_Arrays belonging to worldToBeDisplayed are added to the drawing list for Displayer
	void SetWorld(World* worldToBeDisplayed);

	void AddToDrawingList(ViewableObject*);
	void RemoveFromDrawingList(ViewableObject*);

	/* This method is responsible for not only animating the specified action but also applying it to
	 * the relevant ViewableObject(s) belonging to worldToBeDisplayed (if it is a dsAction).
	 *
	 * NB: Uses parameter copy semantics (see devdocs)
	 */
	void PerformAndAnimateActionAsync(Action*);



private:
	static bool drawingEnabled;

	// Singleton Members
	static Displayer* displayerInstance;
	static sf::Mutex creationMutex;	
	Displayer();
	~Displayer();

	// Window, window state, default font
	sf::RenderWindow* win;
	sf::Font defaultFont;
	bool defaultFontInitialised, updated, closed;
	unsigned refreshRate;		// Default = 60 Hz

	// Below three methods invoked on render thread
	void InitWindow();
	void RenderLoop();
	static void RenderThread(void* dispPtr);
	sf::Thread* renderThread;



	// Panels
	Panel* worldPanel;
	void SetupPanels();
	void DrawWorldPanel();


	// World 
	World* worldToBeDisplayed;

	// Viewables to draw plus mutex to synchronise access between AddTo..., RemoveFrom..., RenderLoop()
	std::set<ViewableObject*> viewablesToDraw;
	sf::Mutex viewablesMutex;
	
	// Current action (if there is one)
	Action* actionToBePerformed;
	bool actionPending;
	
		

	// ignore this - it's a hack used for the mock animation
	unsigned duration;


};



}



#endif //DISPLAY_H
