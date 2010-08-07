#include <sstream>
#include "boost/foreach.hpp"
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "utilities.h"
#include "../../include/registry.h"
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
	UL_ASSERT(subject->GetType() == SINGLE_PRINTABLE);
	
	VO_SinglePrintable* printable = (VO_SinglePrintable*)(subject); // TODO: This is a serious problem
	UL_ASSERT(printable);
	if (printable != 0)
	{
		#ifdef DEBUG_SP_CHANGES
			std::cout << "Value " << printable->GetDSAddress() << "(" << time << "): " ;
			std::cout << printable->GetValue() << " produced from: ";

			// TODO: History should be reset if the thing is drawn. This assumes things in arrays are drawn.
			const std::set<ValueID>& history = printable->GetHistory();
			for (std::set<ValueID>::const_iterator i = history.begin(); i != history.end(); i++)
				std::cout << i->address << "(" << i->time << ")" << ", ";
			std::cout << std::endl;

		#endif

		//int time = Algovis_Viewer::Registry::GetInstance()->GetTime();
		printable->ResetHistory(ValueID(subject->GetDSAddress(), time));
		
		// hack to make printable draw itself with a green outline after being changed. TODO: remove
		BOOST_FOREACH(ViewableObject* element, elements)
			element->SetBoundingBoxColour(1,1,1);
		printable->SetBoundingBoxColour(0,1,0);
	}
	++time;
}

VO_Array::VO_Array(const void* dsAddress, World* world, 
						ViewableObjectType elementType, const std::vector<ViewableObject*>& elements)
		: ViewableObject(dsAddress, world), elementType(elementType), graphicalAddressText(NULL)
{
	BOOST_FOREACH(ViewableObject* element, elements)
	{
		this->elements.push_back(element);	
		elements[elements.size() - 1]->AddObserver(this);
	}
}


VO_Array::~VO_Array()
{
	Registry* registry = Registry::GetInstance();

	BOOST_FOREACH(ViewableObject* element, elements)
	{
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
	NotifyObservers(UPDATED);
	Changed(elements[position]);
}

void VO_Array::PushElementToBack(ViewableObject* element)
{
	elements.push_back(element);
}

void VO_Array::ClearArray(unsigned newCapacity)
{
	// TODO do more clean-up such as deregistering as observer etc.
	elements.clear();
	UL_ASSERT(elements.size() == 0);
	// TODO do something with newCapacity
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
	NotifyObservers(UPDATED);
}

void VO_Array::PrepareToBeDrawn()
{
	static float xGap = 1, yGap = 5;

	float x = boundingBox.Left;
	float y = boundingBox.Top;

	// Address stuff
	std::string addressString(util::ToString<const void*>(dsAddress).append(":"));

	delete graphicalAddressText;
	graphicalAddressText = new sf::String(addressString.c_str(), Displayer::GetInstance()->GetDefaultFont());
	graphicalAddressText->SetColor(sf::Color(255, 255, 255));
	//graphicalAddressText.Move(x, y);
	graphicalAddressText->SetPosition(x,y);
	
	
	float offset = graphicalAddressText->GetRect().GetWidth();
	UL_ASSERT(offset);		// Ensure that the font is valid (an invalid one will result in offset=0)
	x += offset + xGap; 

	// Array elements
	float maxTextHeight = 0;
	BOOST_FOREACH(ViewableObject* element, elements)
		maxTextHeight = std::max(maxTextHeight, element->GetPreferredSize().GetHeight());
	maxTextHeight += yGap; // should this really be part of maxTextHeight which affects bbox for all elements

	BOOST_FOREACH(ViewableObject* element, elements)
	{
		sf::FloatRect preferredSize = element->GetPreferredSize();
		element->SetBoundingBox(sf::FloatRect(x,y,x + preferredSize.GetWidth(), y + maxTextHeight));
		element->PrepareToBeDrawn();
		x += element->GetBoundingBox().GetWidth() + xGap;
	}

	// TODO set bounding box of array
}

void VO_Array::Draw(sf::RenderWindow& renderWindow, sf::Font& defaultFont)
{
	// Print address
	renderWindow.Draw(*graphicalAddressText);

	// Print elements
	BOOST_FOREACH(ViewableObject* element, elements)
	{
		// for testing purposes
		element->GetBoundingBox();

		// TODO remove SP hack
		UL_ASSERT(element->GetType() == SINGLE_PRINTABLE);
		VO_SinglePrintable* spElement = (VO_SinglePrintable*) element;

		spElement->Draw(renderWindow, defaultFont);
	}
}


void VO_Array::Notify(ViewableObject* subject, NOTIFY_EVENT_TYPE eventType)
{
	if (eventType == UPDATED)
	{
		NotifyObservers(UPDATED);
		Changed(subject);
	}
	else if (eventType == BEING_DESTROYED)
	{
		for(vector<ViewableObject*>::iterator it = elements.begin(); it < elements.end(); it++)
		{
			if ((*it) == subject)
			{
				elements.erase(it);
				NotifyObservers(UPDATED);
				return;
			}
		}
	}
}





}