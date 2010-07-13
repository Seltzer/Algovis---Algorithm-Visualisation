#ifndef DISPLAY_H
#define DISPLAY_H

//#include <iostream>
//#include <string>
//#include <vector>

#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"





namespace Algovis_Viewer
{


// turn into include later
class ViewableObject;


class Displayer
{


private:
	static Displayer* displayerInstance;
	static bool drawingEnabled;

	Displayer()
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

	void RenderLoop();

	static void RenderThread(void* dispPtr)
	{
		static_cast<Displayer*>(dispPtr)->RenderLoop();
	}

	sf::Mutex dataMutex;
		
	//std::map<const void*, std::vector<std::string> > data;
	sf::Thread* renderThread;
	sf::RenderWindow* win;
	sf::Font font;
	bool updated, closed;



public:
	
	// Public static methods
	static Displayer* GetInstance()
	{
		if (!Displayer::displayerInstance)
			Displayer::displayerInstance = new Displayer();

		return Displayer::displayerInstance;
	}

	static void EnableDrawing(bool enabled) { Displayer::drawingEnabled = enabled; }

	static bool DrawingEnabled() { return Displayer::drawingEnabled; }


	// Public instance methods - stubs atm
	sf::Font& GetFont() { return font; }

	void AddToTheDrawingList(ViewableObject*) {}
		
	void RemoveFromDrawingList(ViewableObject*) {}
};



}



#endif //DISPLAY_H
