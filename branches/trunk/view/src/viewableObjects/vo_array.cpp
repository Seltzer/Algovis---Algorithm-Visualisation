#include <sstream>
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "boost/foreach.hpp"

#include "vo_array.h"
#include "vo_singlePrintable.h"



using namespace std;


namespace Algovis_Viewer
{
	
	int time = 0; // TODO: Seriously?... Seriously guys.

	void VO_Array::Changed(ViewableObject* subject)
	{
		// TODO: use history somehow to do cool things
		// The element notifying is obviously in this array. We shall assume we have displayed
		// the value and history here, and will reset the current history to be the value we just displayed
		// That way future things will have the just-displayed-element in their history, instead of everything
		// that was used to produce is.
		VO_SinglePrintable* printable = (VO_SinglePrintable*)(subject); // TODO: This is a serious problem
		if (printable != 0)
		{
			std::cout << "Value: " << printable->GetValue() << " produced from: ";
			const std::set<ValueID>& history = printable->GetHistory();
			for (std::set<ValueID>::const_iterator i = history.begin(); i != history.end(); i++)
				std::cout << i->address << "(" << i->time << ")" << ", ";
			std::cout << std::endl;
			//int time = Algovis_Viewer::Registry::GetInstance()->GetTime();
			printable->ResetHistory(ValueID(subject->GetDSAddress(), time));
		}
		++time;
	}

	VO_Array::VO_Array
			(const void* dsAddress, ViewableObjectType elementType, const std::vector<ViewableObject*>& elements)
				: ViewableObject(dsAddress), elementType(elementType)
	{
		for (std::vector<ViewableObject*>::const_iterator it = elements.begin(); it < elements.end(); it++)
		{
			ViewableObject* element = *it;

			this->elements.push_back(element);
			elements[elements.size() - 1]->AddObserver(this);
		}
	}


	VO_Array::~VO_Array()
	{
		Registry* registry = Registry::GetInstance();

		for (vector<ViewableObject*>::iterator it = elements.begin(); it < elements.end(); it++)
		{
			ViewableObject* element = *it;
			element->RemoveObserver(this);
			registry->DeregisterObject(element);
		}
	}

	
	void VO_Array::AddElement(ViewableObject* element, unsigned position)
	{
		UL_ASSERT(elements.size() >= position);

		if (position < elements.size())
			elements[position] = element;
		else 
			elements.push_back(element);

		elements[position]->AddObserver(this);
		//Draw();
		Changed(elements[position]);
	}



	void VO_Array::SwapElements(unsigned firstElement, unsigned secondElement)
	{
		UL_ASSERT(elementType == SINGLE_PRINTABLE);

		VO_SinglePrintable* first = (VO_SinglePrintable*) elements[firstElement];
		VO_SinglePrintable* second = (VO_SinglePrintable*) elements[secondElement];

		std::string temp = first->GetValue();
		
		// TODO: UpdateValue is obsolete, and this does not track history for the elements involved.
		//first->UpdateValue(second->GetValue());
		//second->UpdateValue(temp);

		//Draw();
	}


	void VO_Array::Draw(sf::RenderWindow& renderWindow, sf::Font& font)
	{
		
		float xGap = 1;
		float yGap = 50;
		
		float y = 50;

		std::vector<std::string> elementsAsStrings;

		for (std::vector<ViewableObject*>::iterator it = elements.begin(); it < elements.end(); it++)
		{
			// Assume VO_SinglePrintable
			VO_SinglePrintable* element = (VO_SinglePrintable*) *it;
			elementsAsStrings.push_back(element->GetValue());
		}

		// Print address
		std::stringstream strm;
		strm << dsAddress << ":";
		sf::String address(strm.str().c_str(), font);
		address.SetColor(sf::Color(255, 255, 255));
		address.Move(0, y);
		renderWindow.Draw(address);

		// Print elements
		float ySize = 0;
		float x = address.GetRect().Right + xGap;


		BOOST_FOREACH(ViewableObject* element, elements)
		{
			UL_ASSERT(element->GetType() == SINGLE_PRINTABLE);
			VO_SinglePrintable* spElement = (VO_SinglePrintable*) element;

			// Draw text
			sf::String text(spElement->GetValue().c_str(), font);
			text.SetColor(sf::Color(255, 255, 255));
			text.Move(x, y);
			renderWindow.Draw(text);

			// Draw bounding box
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
		
		// Each element now draws itself
		//y += ySize + yGap;
	}


	void VO_Array::Notify(ViewableObject* subject)
	{
		//ZOMG
		//Draw();

		Changed(subject);
	}
}