#include <QObject>
#include "QtGui/qevent.h"
#include "utilities.h"
#include "viewableObject.h"



namespace Algovis_Viewer
{


ViewableObject::ViewableObject(ID id, const void* dsAddress, World* world)
	: id(id), dsAddress(dsAddress), world(world), mouseDraggingInitiated(false), 
		boundingBoxColour(Qt::green), sizeDictatedByParent(false), hasParentViewable(false)
{
}

ViewableObject::ViewableObject(ID id, const void* dsAddress, World* world, QWidget* parent)
	: Component(parent, QPoint(5,5),QSize(200,200)),id(id), dsAddress(dsAddress), world(world), 
		mouseDraggingInitiated(false),boundingBoxColour(Qt::green), 
		sizeDictatedByParent(false), hasParentViewable(false)
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
		return parentWidget() == (QWidget*) world;

	return false;
}

QColor ViewableObject::GetBoundingBoxColour()
{
	return boundingBoxColour;
}

void ViewableObject::SetBoundingBoxColour(QColor& boundingBoxColour)
{
	this->boundingBoxColour = boundingBoxColour;
}

void ViewableObject::mousePressEvent(QMouseEvent* evt)
{
	if (evt->button() == Qt::MouseButton::LeftButton)
	{
		if (!HasParentViewable())
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
		if (!HasParentViewable())
			mouseDraggingInitiated = false;
		else
			evt->ignore();
	}
}
	
void ViewableObject::mouseMoveEvent(QMouseEvent* evt)
{
	if (!HasParentViewable() && mouseDraggingInitiated)
	{
		QPoint newPosition = localPositionBeforeDragging + evt->globalPos() - globalPositionBeforeDragging;
		if (newPosition.x() < 0)
			newPosition.setX(0);
		if (newPosition.y() < 0)
			newPosition.setY(0);

		move(newPosition);
		repaint();
	}
	else
	{
		// Pass onto parent
		evt->ignore();
	}
}

void ViewableObject::wheelEvent(QWheelEvent* evt)
{
	if (HasParentViewable())
	{
		// Let parent deal with it
		evt->ignore();
	}
	else
	{
		QFont newFont(font());
		
		if (evt->delta() > 0)
			newFont.setPointSize(max((int) font().pointSize() * 1.1, font().pointSize() + 1));
		else
			newFont.setPointSize(min((int) font().pointSize() / 1.1, font().pointSize() - 1));

		setFont(newFont);
		evt->accept();
		adjustSize();
	}
}


void ViewableObject::SetSizeDictatedByParent(bool sizeDictatedByParent)
{
	this->sizeDictatedByParent = sizeDictatedByParent;
}


ID ViewableObject::GetId()
{
	return id;
}

const void* ViewableObject::GetDSAddress() 
{ 
	return dsAddress; 
}

void ViewableObject::SetDSAddress(const void* newAddress)
{
	dsAddress = newAddress;
	adjustSize();
}


bool ViewableObject::HasParentViewable() const
{
	return hasParentViewable;
}

void ViewableObject::SetParentViewable(ViewableObjectContainer* newParent)
{
	parentViewable = newParent;
	hasParentViewable = true;
	setParent((QWidget*) newParent);
}


}

