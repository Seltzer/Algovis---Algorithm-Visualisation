#include "display.h"

#include <map>
#include <sstream>
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"

#include "utilities.h"

namespace Algovis_Viewer
{


	
bool drawingEnabled = true;

void EnableDrawing(bool enable)
{
	drawingEnabled = enable;
}


class Displayer
{


public:
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
	void Draw(const void* dataStructure, const std::vector<std::string>& newData)
	{
		//sf::Sleep(0.5);
		{
			sf::Lock lock(dataMutex);
			data[dataStructure] = newData;
		}
	}


private:

	void Render()
	{
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		float xGap = 1;
		float yGap = 50;
		float y = yGap;
		std::map<const void*, std::vector<std::string> >::iterator i;
		for (i = data.begin(); i != data.end(); i++)
		{
			std::stringstream strm;
			strm << i->first << ":";
			sf::String address(strm.str().c_str(), font);
			address.SetColor(sf::Color(255, 255, 255));
			address.Move(0, y);
			win->Draw(address);

			float ySize = 0;
			float x = address.GetRect().Right + xGap;
			std::vector<std::string>& container = i->second;
			for (unsigned j = 0; j < container.size(); j++)
			{
				sf::String text(container[j].c_str(), font);
				text.SetColor(sf::Color(255, 255, 255));
				text.Move(x, y);

				win->Draw(text);

				sf::FloatRect rect = text.GetRect();
				
				glColor3f(1, 1, 1);
				glBegin(GL_LINE_LOOP);
				glVertex2f(rect.Left, rect.Top);
				glVertex2f(rect.Right, rect.Top);
				glVertex2f(rect.Right, rect.Bottom);
				glVertex2f(rect.Left, rect.Bottom);
				glEnd();

				x += rect.Right - rect.Left + xGap;
				ySize = std::max(ySize, rect.Bottom - rect.Top);
			}
			y += ySize + yGap;
		}
	}
	void RenderLoop()
	{
		win = new sf::RenderWindow(sf::VideoMode(800, 600, 32), "Algovis");
		win->SetActive(true);
		glViewport(0, 0, win->GetWidth(), win->GetHeight());
		win->PreserveOpenGLStates(true);
		font.LoadFromFile("consola.ttf");

		while (win->IsOpened() && !closed)
		{
			sf::Event Event;
		    while (win->GetEvent(Event))
		    {
				// window resized
				if (Event.Type == sf::Event::Resized)
				{
					int width = Event.Size.Width;
					int height = Event.Size.Height;
					win->SetView(sf::View(sf::FloatRect(0, 0, (float) width, (float) height)));
					glViewport(0, 0, width, height);
				}

		        // Window closed
		        if (Event.Type == sf::Event::Closed)
		            win->Close();

		        // Escape key pressed
		        if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Escape))
		            win->Close();
		    }

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0, win->GetWidth(), win->GetHeight(), 0, -1, 1);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			Render();

			win->Display();

			sf::Sleep(1.0f / 60);
		}
	}
	static void RenderThread(void* dispPtr)
	{
		static_cast<Displayer*>(dispPtr)->RenderLoop();
	}

	sf::Mutex dataMutex;
	std::map<const void*, std::vector<std::string> > data;
	sf::Thread* renderThread;
	sf::RenderWindow* win;
	sf::Font font;
	bool updated;
	bool closed;
};



void Draw(const void* dataStructure, const std::vector<std::string>& data)
{
	if (drawingEnabled)
	{
		static Displayer displayer; // Only one window, created on first draw
		displayer.Draw(dataStructure, data);
	}
}





}