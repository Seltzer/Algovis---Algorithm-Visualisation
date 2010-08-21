#include "viewableObject.h"

#include "QtGui/qevent.h"


namespace Algovis_Viewer
{



ViewableObject::ViewableObject(const void* dsAddress, World* world) 
	: dsAddress(dsAddress), world(world), mouseDraggingInitiated(false), 
		boundingBoxColour(Qt::green), sizeControlledByParentArray(false)
{
}


ViewableObject::ViewableObject(QWidget* parent, const void* dsAddress, World* world) 
	: Component(parent, QPoint(5,5),QSize(200,200)),dsAddress(dsAddress), world(world), 
		mouseDraggingInitiated(false),boundingBoxColour(Qt::green), sizeControlledByParentArray(false)
{
}


ViewableObject::~ViewableObject()
{
}

QPoint ViewableObject::GetPositionInWorld()
{
	if (IsTopLevel() || !parentWidget())
		return pos();

	return pos() + ((ViewableObject*) parentWidget())->GetPositionInWorld();
}

bool ViewableObject::IsTopLevel()
{
	if (parentWidget())
	{
		return parentWidget() == (QWidget*) world;
	}

	return false;
}


void ViewableObject::SetBoundingBoxColour(QColor& boundingBoxColour)
{
	this->boundingBoxColour = boundingBoxColour;
}

void ViewableObject::mousePressEvent(QMouseEvent* evt)
{
	if (evt->button() == Qt::MouseButton::LeftButton)
	{
		if (IsTopLevel())
		{
			globalPositionBeforeDragging = evt->globalPos();
			localPositionBeforeDragging = pos();
			mouseDraggingInitiated = true;
		}
		else
		{
			evt->ignore();
		}
	}
}

void ViewableObject::mouseReleaseEvent(QMouseEvent* evt)
{
	if (evt->button() == Qt::MouseButton::LeftButton)
	{
		if (IsTopLevel())
			mouseDraggingInitiated = false;
		else
			evt->ignore();
	}
}
	
void ViewableObject::mouseMoveEvent(QMouseEvent* evt)
{
	if (IsTopLevel() && mouseDraggingInitiated)
	{
		QPoint newPosition = localPositionBeforeDragging + evt->globalPos() - globalPositionBeforeDragging;
		if (newPosition.x() < 0)
			newPosition.setX(0);
		if (newPosition.y() < 0)
			newPosition.setY(0);

		move(newPosition);
	}
	else
	{
		// Pass onto parent
		evt->ignore();
	}

}


void ViewableObject::SetSizeControlledByParentArray(bool sizeControlledByParentArray)
{
	this->sizeControlledByParentArray = sizeControlledByParentArray;
}


}

