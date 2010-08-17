#ifndef VIEWABLE_OBJECT_H_
#define VIEWABLE_OBJECT_H_

#include <set>
#include "../displayer/component.h"
#include "../../include/common.h"


class QMouseEvent;



// ViewableObject abstract class - represents a viewable object drawn with a bounding box
namespace Algovis_Viewer
{
	class World;
	class DS_Action;
	class ViewableObject;



	class ViewableObject : public Component
	{

	public:
		virtual ~ViewableObject();

		// Optional method for drawing a VO's value at a specified absolute position with specified dimensions
		virtual void DrawValue(QRect&, QPainter*) {}

		// Optional method for drawing a VO exactly like paintEvent(), but without its value
		virtual void DrawWithoutValue(QRect&, QPainter*) {}
		
		// NB: Returns false if VO hasn't been added to the World
		bool IsTopLevel();

		// Returns this VO's position relative to the World - undefined if VO hasn't been added to the World
		QPoint GetPositionInWorld();

		virtual void SetBoundingBoxColour(QColor&);

		const void* GetDSAddress() { return dsAddress; }
		virtual ViewableObjectType GetType() const = 0;
	
	protected:	
		ViewableObject(const void* dsAddress, World* world); 
		ViewableObject(QWidget* parent, const void* dsAddress, World* world); 

		// Mouse drag/drop
		virtual void mousePressEvent(QMouseEvent*);
		virtual void mouseMoveEvent(QMouseEvent*);
		virtual void mouseReleaseEvent(QMouseEvent*);
		bool mouseDraggingInitiated;
		QPoint globalPositionBeforeDragging, localPositionBeforeDragging;

		
		World* world;
		const void* dsAddress;
		QColor boundingBoxColour;
	};

	

}



#endif