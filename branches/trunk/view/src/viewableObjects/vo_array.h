#ifndef VO_ARRAY_H_
#define VO_ARRAY_H_


#include <vector>
#include "viewableObject.h"



namespace Algovis_Viewer
{


	class VO_Array : public ViewableObject
	{

	public:
		VO_Array(ID id, const void* dsAddress, World* world, ViewableObjectType elementType, 
					const std::vector<ViewableObject*>& elements, QWidget* parent = NULL);
		~VO_Array();
	
		virtual ViewableObjectType GetType() const { return ARRAY; }

		// QT Overloads
		virtual QSize sizeHint() const;
		virtual void adjustSize();
		virtual void paintEvent(QPaintEvent*);

		
				

		unsigned GetSize() { return elements.size(); }

		void AddElement(ViewableObject*, unsigned position);
		void PushElementToBack(ViewableObject*);

		void ClearArray();

		// Not currently implemented by any DS
		void SwapElements(unsigned firstElement, unsigned secondElement);


	private:
		std::vector<ViewableObject*> elements;
		ViewableObjectType elementType;

		mutable QString addressText;
		mutable QPoint addressTextPosition;

	};
}





#endif