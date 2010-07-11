#ifndef VO_ARRAY_H_
#define VO_ARRAY_H_


#include "viewableObject.h"

#include <vector>



namespace Algovis_Viewer
{

	// TODO template maybe? consider implications
	class DECLSPEC VO_Array : ViewableObject, IViewableObjectObserver
	{

	private:
		std::vector<ViewableObject*> elements;
		ViewableObjectType elementType;

	public:
		VO_Array(void* dsAddress, ViewableObjectType elementType, const std::vector<ViewableObject*>& elements);


		~VO_Array();
	
		virtual ViewableObjectType GetType() { return ARRAY; }

		virtual void Draw(); 
		
		unsigned GetSize() { return elements.size(); }

		// TODO here for testing purposes - move to cpp
		void AddElement(ViewableObject* element, unsigned position);

		void SwapElements(unsigned firstElement, unsigned secondElement);

		// Used as an event-handler for elements being updated
		virtual void Notify(ViewableObject* subject);


	};
}





#endif