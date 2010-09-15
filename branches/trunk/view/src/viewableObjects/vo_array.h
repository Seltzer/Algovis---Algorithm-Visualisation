#ifndef VO_ARRAY_H_
#define VO_ARRAY_H_


#include <vector>
#include "viewableObjectContainer.h"



namespace Algovis_Viewer
{


	class VO_Array : public ViewableObjectContainer
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

		virtual void DrawValue(bool includingChildren, QRect&, QPainter*);
		virtual void DrawBoundingBox(const QRect&, QPainter*);


		// Get number of elements in array
		unsigned GetSize() { return children.size(); }

		void AddElement(ViewableObject*, unsigned position);
		void PushElementToBack(ViewableObject*);
		
		void RemoveElements(std::vector<ViewableObject*> elementsToRemove, unsigned startIndex, unsigned endIndex);
		void ClearArray();

		// Not currently implemented by any DS
		void SwapElements(unsigned firstElement, unsigned secondElement);


	private:
		virtual void childDestroyed(QObject*);
		//std::vector<ViewableObject*> elements;
		ViewableObjectType elementType;
				
		mutable QString titleText;
		mutable QPoint titleTextPosition;
	};

	class VO_ArrayFactory : public ViewableObjectContainerFactory
	{
	protected:
		ViewableObjectType elementType;
		const std::vector<ViewableObject*>& elements;
	public:
		VO_ArrayFactory(ID id, const void* dsAddress, World* world, ViewableObjectType elementType, 
					const std::vector<ViewableObject*>& elements, QWidget* parent = NULL)
			: ViewableObjectContainerFactory(id, dsAddress, world, parent), elementType(elementType), elements(elements)
		{}
		virtual ViewableObject* Create()
		{
			return new VO_Array(id, dsAddress, world, elementType, elements, parent);
		}
	};
}





#endif