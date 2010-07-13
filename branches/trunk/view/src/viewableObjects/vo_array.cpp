#include <sstream>
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "boost/foreach.hpp"

#include "vo_array.h"
#include "vo_singlePrintable.h"



using namespace std;


namespace Algovis_Viewer
{
	
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
	}



	void VO_Array::SwapElements(unsigned firstElement, unsigned secondElement)
	{
		UL_ASSERT(elementType == SINGLE_PRINTABLE);

		VO_SinglePrintable* first = (VO_SinglePrintable*) elements[firstElement];
		VO_SinglePrintable* second = (VO_SinglePrintable*) elements[secondElement];

		std::string temp = first->GetValue();
		
		first->UpdateValue(second->GetValue());
		second->UpdateValue(temp);

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
	}
}