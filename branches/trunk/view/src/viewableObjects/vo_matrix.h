#ifndef VO_MATRIX_H_
#define VO_MATRIX_H_


#include "viewableObjectContainer.h"



namespace Algovis_Viewer
{


	class VO_Matrix : public ViewableObjectContainer
	{

	public:
		VO_Matrix(ID id, const void* dsAddress, World* world, ViewableObjectType elementType, 
					unsigned rows, unsigned cols, const std::vector<ViewableObject*>& initElements, 
					QWidget* parent = NULL);
		
		virtual ViewableObjectType GetType() const { return MATRIX; }

		// QT Overloads
		virtual QSize sizeHint() const;
		virtual void adjustSize();
		virtual void paintEvent(QPaintEvent*);

		virtual void DrawValue(bool includingChildren, QRect&, QPainter*);
		virtual void DrawBoundingBox(const QRect&, QPainter*);


		// Get number of elements in array
		unsigned GetRows();
		unsigned GetCols();


	private:
		ViewableObjectType elementType;
		unsigned rows, cols;
		
		// TODO temporary
		(ViewableObject*) elements[10][10];
				
		mutable QString titleText;
		mutable QPoint titleTextPosition;
	};
}





#endif