#include <iostream>
#include <algorithm>
#include <QObject>
#include <QLineEdit>
#include <Qt/qinputdialog.h>
#include "QtGui/qevent.h"
#include "utilities.h"
#include "viewableObject.h"
#include "viewableObjectContainer.h"

using namespace std;


namespace Algovis_Viewer
{


ViewableObject::ViewableObject(ID id, const void* dsAddress, World* world)
	: id(id), world(world), mouseDraggingInitiated(false), 
		userHasSetTitleBefore(false), boundingBoxColour(Qt::green), sizeDictatedByParent(false), hasParentViewable(false)
{
	SetDSAddress(dsAddress);
	setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), 
					SLOT(spawnContextMenu(const QPoint&)), Qt::DirectConnection);
}

ViewableObject::ViewableObject(ID id, const void* dsAddress, World* world, QWidget* parent)
	: Component(parent, QPoint(5,5),QSize(200,200)),id(id), world(world), 
		mouseDraggingInitiated(false), userHasSetTitleBefore(false), boundingBoxColour(Qt::green), 
		sizeDictatedByParent(false), hasParentViewable(false)
{
	SetDSAddress(dsAddress);
	setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), 
					SLOT(spawnContextMenu(const QPoint&)), Qt::DirectConnection);
}


ViewableObject::~ViewableObject()
{
}

QPoint ViewableObject::GetPositionInWorld()
{
	if (!hasParentViewable)
		return pos();
	else
		return pos() + parentViewable->GetPositionInWorld();
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
	else
	{
		evt->ignore();
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
	else
	{
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
			newFont.setPointSize(max((int) (font().pointSize() * 1.1), font().pointSize() + 1));
		else
			newFont.setPointSize(min((int) (font().pointSize() / 1.1), font().pointSize() - 1));

		setFont(newFont);
		evt->accept();
		adjustSize();
	}
}

void ViewableObject::spawnContextMenu(const QPoint& point)
{
	if (hasParentViewable)
	{
		QPoint translatedPoint = point + pos();
		parentViewable->spawnContextMenu(translatedPoint);
	}
	else
	{
		QMenu *menu = new QMenu;
		menu->addAction(tr("Set Title"), this, SLOT(setTitleSlot()));
		menu->exec(mapToGlobal(point));
	}
}

void ViewableObject::setTitleSlot()
{
	bool okPressed;

	QString text = QInputDialog::getText(this, "Algovis", "Set title for viewable", QLineEdit::Normal,
								QString(titleString.c_str()),&okPressed);
    
	if (okPressed && !text.isEmpty()) 
	{
		if (!text.endsWith(":"))
			text.append(":");

		SetTitle(text.toStdString());
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

	if (!userHasSetTitleBefore)
		SetTitle(util::ToString<const void*>(dsAddress).append(":"));
}

void ViewableObject::SetTitle(const std::string& newTitle)
{
	userHasSetTitleBefore = true;
	titleString = newTitle;

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

