#ifndef VO_ARRAY_H_
#define VO_ARRAY_H_


#include <vector>

#include "viewableObject.h"



namespace Algovis_Viewer
{

	class VO_Array : ViewableObject, IViewableObjectObserver
	{

	private:
		std::vector<ViewableObject*> elements;
		ViewableObjectType elementType;

		void Changed(ViewableObject* subject);

	public:
		VO_Array(const void* dsAddress, ViewableObjectType elementType, 
							const std::vector<ViewableObject*>& elements);
		~VO_Array();
	
		virtual ViewableObjectType GetType() { return ARRAY; }

		virtual void Draw(sf::RenderWindow& renderWindow, sf::Font& font); 
		
		unsigned GetSize() { return elements.size(); }

		void AddElement(ViewableObject* element, unsigned position);

		void SwapElements(unsigned firstElement, unsigned secondElement);

		// Used as an event-handler for elements being updated
		virtual void Notify(ViewableObject* subject);

	};
}





#endif