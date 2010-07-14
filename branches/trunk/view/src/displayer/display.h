#ifndef DISPLAY_H
#define DISPLAY_H


#include <set>
#include "boost/bind.hpp"
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "utilities.h"

namespace Algovis_Viewer
{


// turn into include later
class ViewableObject;
class VO_Array;
class VO_SinglePrintable;

class Displayer
{

private:
	static Displayer* displayerInstance;
	static bool drawingEnabled;

	Displayer()
		: defaultFontInitialised(false)
	{
		closed = false;
		renderThread = new sf::Thread(&RenderThread, this);
		renderThread->Launch();
	}

	~Displayer()
	{
		closed = true;
		delete renderThread;
	}

	void InitWindow();

	// Main render loop plus threading
	void RenderLoop();
	static void RenderThread(void* dispPtr);
	sf::Thread* renderThread;
	
	// Window, window state, default font
	sf::RenderWindow* win;
	sf::Font defaultFont;
	bool defaultFontInitialised, updated, closed;

	// Set of ViewableObjects to draw plus synchronisation
	std::set<ViewableObject*> objectsToDraw;
	sf::Mutex objectsMutex;

	// TODO hack
	static sf::Mutex creationMutex;

public:
	// Public static methods
	static Displayer* GetInstance();
	static void EnableDrawing(bool enabled) { Displayer::drawingEnabled = enabled; }
	static bool DrawingEnabled() { return Displayer::drawingEnabled; }


	sf::Font& GetDefaultFont();
	void AddToDrawingList(ViewableObject*);
	void RemoveFromDrawingList(ViewableObject*);
};



}



#endif //DISPLAY_H
