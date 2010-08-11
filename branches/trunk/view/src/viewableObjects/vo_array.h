#ifndef VO_ARRAY_H_
#define VO_ARRAY_H_


#include <vector>
#include "viewableObject.h"



namespace Algovis_Viewer
{
	class ComponentEvent;

	class VO_Array : public ViewableObject
	{

	private:
		std::vector<ViewableObject*> elements;
		ViewableObjectType elementType;

		sf::String* graphicalAddressText;
		void Changed(ViewableObject* subject);

	public:
		VO_Array(const void* dsAddress, World* world, ViewableObjectType elementType, const std::vector<ViewableObject*>& elements);
		~VO_Array();
	
		virtual ViewableObjectType GetType() { return ARRAY; }

		virtual void SetupLayout();

		virtual void Draw(sf::RenderWindow& renderWindow, sf::Font& defaultFont); 
		

		unsigned GetSize() { return elements.size(); }

		void AddElement(ViewableObject*, unsigned position);
		void PushElementToBack(ViewableObject*);

		// Currently assumes that elements have already been cleaned up by Registry::DeregisterObject
		// TODO: this is bad
		void ClearArray(unsigned newCapacity);

		// Not currently implemented by any DS
		void SwapElements(unsigned firstElement, unsigned secondElement);

		// Used as an event-handler for elements being updated
		virtual void Notify(Component* subject, ComponentEvent&);

	};
}





#endif