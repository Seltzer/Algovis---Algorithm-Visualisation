#ifndef VIEWABLE_OBJECT_H_
#define VIEWABLE_OBJECT_H_

#include <string>
#include <QMenu>
#include "../displayer/components.h"
#include "../../include/common.h"

class QMouseEvent;



// ViewableObject abstract class - represents a viewable object drawn with a bounding box
namespace Algovis_Viewer
{
	class World;
	class DS_Action;
	class ViewableObjectContainer;


	class ViewableObject : public Component
	{
		Q_OBJECT

	public:
		virtual ~ViewableObject();
		virtual ViewableObjectType GetType() const = 0;
		virtual bool IsVOContainer() { return false; }

		// Optional method for drawing a VO's value at a specified absolute position with specified dimensions
		// The value is the whole VO minus its bounding box
		virtual void DrawValue(bool includingChildren, const QRect&, QPainter*) {}

		virtual void DrawBoundingBox(const QRect&, QPainter*) {}

		// NB: Returns false if VO hasn't been added to the World
		// TODO obsolete
		bool IsTopLevel();

		bool HasParentViewable() const;
		void SetParentViewable(ViewableObjectContainer*);

		// Returns this VO's position relative to the World - undefined if VO hasn't been added to the World
		QPoint GetPositionInWorld();

		QColor GetBoundingBoxColour();
		virtual void SetBoundingBoxColour(QColor&);
	
		ID GetId();
		const void* GetDSAddress();
		void SetDSAddress(const void*);

		void SetTitle(const std::string&);

		void SetSizeDictatedByParent(bool);

	protected:	
		ViewableObject(ID, const void* dsAddress, World*); 
		ViewableObject(ID, const void* dsAddress, World*, QWidget* parent); 

		// Mouse drag/drop
		virtual void mousePressEvent(QMouseEvent*);
		virtual void mouseMoveEvent(QMouseEvent*);
		virtual void mouseReleaseEvent(QMouseEvent*);
		virtual void wheelEvent(QWheelEvent*);

		bool mouseDraggingInitiated;
		QPoint globalPositionBeforeDragging, localPositionBeforeDragging;

		// ID / dsAddress / title
		const ID id;
		const void* dsAddress;
		std::string titleString;
		bool userHasSetTitleBefore;


		QColor boundingBoxColour;

		// If this is true, then this Viewable will call parent->adjustSize (which presumably works recursively)
		// rather than calling adjustSize on itself
		bool sizeDictatedByParent;

		bool hasParentViewable;
		ViewableObjectContainer* parentViewable;

		World* world;

	public slots:
		virtual void spawnContextMenu(const QPoint&);
		virtual void setTitleSlot();

	};

	

}



#endif