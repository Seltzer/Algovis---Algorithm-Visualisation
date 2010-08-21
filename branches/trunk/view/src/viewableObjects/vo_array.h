#ifndef VO_ARRAY_H_
#define VO_ARRAY_H_


#include <vector>
#include "viewableObject.h"



namespace Algovis_Viewer
{


	class VO_Array : public ViewableObject
	{

	public:
		VO_Array(const void* dsAddress, World* world, ViewableObjectType elementType, 
					const std::vector<ViewableObject*>& elements, QWidget* parent = NULL);
		~VO_Array();
	
		virtual ViewableObjectType GetType() const { return ARRAY; }

		virtual void SetupLayout();
		virtual void SetupLayout2();

		// QT Overloads
		virtual QSize sizeHint();
		virtual void paintEvent(QPaintEvent*);
				

		unsigned GetSize() { return elements.size(); }

		void AddElement(ViewableObject*, unsigned position);
		void PushElementToBack(ViewableObject*);

		// Currently assumes that elements have already been cleaned up by Registry::DeregisterObject
		// TODO: this is bad
		void ClearArray(unsigned newCapacity);
		// Not currently implemented by any DS
		void SwapElements(unsigned firstElement, unsigned secondElement);


	private:
		std::vector<ViewableObject*> elements;
		ViewableObjectType elementType;

		QString addressText;
		QPoint addressTextPosition;

	};
}





#endif