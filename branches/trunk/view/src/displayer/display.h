#ifndef DISPLAY_H
#define DISPLAY_H


#include <set>
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "utilities.h"

namespace Algovis_Viewer
{


class ViewableObject;
class World;
class Action;

class Displayer
{

private:
	static bool drawingEnabled;

	// Singleton Members
	static Displayer* displayerInstance;
	static sf::Mutex creationMutex;	
	Displayer();
	~Displayer();

	World* worldToBeDisplayed;
	Action* actionToBePerformed;
	bool actionPending;

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
	
	// Set of ViewableObjects to draw plus synchronisation
	// Accessed by AddToDrawingList(), RemoveFromDrawingList() and RenderLoop()
	std::set<ViewableObject*> objectsToDraw;
	sf::Mutex objectsMutex;
	
	// Does the Displayer currently have an exclusive lock on the world's VOs?
	bool displayerHasExclusiveLock;

	// ignore this - it's a hack used for the mock animation
	unsigned duration;

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
};



}



#endif //DISPLAY_H
