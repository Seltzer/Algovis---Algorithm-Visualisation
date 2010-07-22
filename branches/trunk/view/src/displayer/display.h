#ifndef DISPLAY_H
#define DISPLAY_H


#include <set>
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "utilities.h"

namespace Algovis_Viewer
{


class ViewableObject;

class Displayer
{

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
	
	// Set of ViewableObjects to draw plus synchronisation
	// Accessed by AddToDrawingList(), RemoveFromDrawingList() and RenderLoop()
	std::set<ViewableObject*> objectsToDraw;
	sf::Mutex objectsMutex;

public:
	// Public static methods
	static Displayer* GetInstance();
	static void EnableDrawing(bool enabled) { Displayer::drawingEnabled = enabled; }
	static bool DrawingEnabled() { return Displayer::drawingEnabled; }

	void SetRefreshRate(unsigned);

	// Busy waits until InitWindow() has finished initialising defaultFont - TODO: get rid of busy-wait
	sf::Font& GetDefaultFont();

	void AddToDrawingList(ViewableObject*);
	void RemoveFromDrawingList(ViewableObject*);
};



}



#endif //DISPLAY_H
