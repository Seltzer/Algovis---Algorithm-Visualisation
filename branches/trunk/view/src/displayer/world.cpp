#include <algorithm>
#include "boost/foreach.hpp"
#include <QPainter>

#include "utilities.h"
#include "world.h"
#include "../viewableObjects/viewableObject.h"

using namespace std;


namespace Algovis_Viewer
{



World::World(QWidget* parent, QPoint& position, QSize& dimensions, 
				QColor& bgColour, QColor& borderColour, unsigned refreshRate)
	: Component(parent, position, dimensions), bgColour(bgColour), borderColour(borderColour), 
			refreshRate(refreshRate)
{
	connect(&myTimer, SIGNAL(timeout()), this, SLOT(update()));
	myTimer.setInterval(1.00f / refreshRate);
	myTimer.start();
}

World::~World()
{
	myTimer.stop();
}

QSize World::sizeHint() const
{
	unsigned requiredWidth = 0, requiredHeight = 0;

	for (QObjectList::const_iterator it = children().begin(); it != children().end(); it++)
	{
		QWidget* child = (QWidget*) *it;
		QSize dimensions = child->sizeHint();
	

		if (child->x() + dimensions.width() > requiredWidth)
			requiredWidth = child->x() + dimensions.width();

		if (child->y() + dimensions.height() > requiredHeight)
			requiredHeight = child->y() + dimensions.height();
		
	}
	
	return QSize(requiredWidth, requiredHeight);
}




void World::AddViewableOnSameRow(ViewableObject* viewable)
{
	if (viewables.empty())
	{
		viewable->move(50,50);
	}
	else
	{
		QPoint pos = viewables.back()->pos();
		pos += QPoint(viewables.back()->width() + 50, 0);
		viewable->move(pos);
	}
	
	// todo hack remove
	viewable->EnableCommunicationWithView(true);
	AddViewable(viewable);
}


void World::AddViewableOnNewRow(ViewableObject* viewable)
{
	if (viewables.empty())
	{
		viewable->move(50,50);
	}
	else
	{
		int y = viewables.back()->pos().y() + viewables.back()->height() + 20;
		viewable->move(50,y);
	}
	
	// todo hack remove
	viewable->EnableCommunicationWithView(true);
	AddViewable(viewable);
}

void World::AddViewable(ViewableObject* viewable)
{
	viewable->setParent(this);
	viewables.push_back(viewable);
	connect(viewable, SIGNAL(resized(QResizeEvent*)),	
						SLOT(topLevelViewableResized(QResizeEvent*)), Qt::DirectConnection);
}

void World::RemoveViewable(ViewableObject* viewable)
{
	vector<ViewableObject*>::iterator result = std::find(viewables.begin(), viewables.end(), viewable);
	UL_ASSERT(result != viewables.end());
	
	viewables.erase(result);
}


void World::topLevelViewableResized(QResizeEvent* evt)
{
	ViewableObject* viewable = (ViewableObject*) sender();
	QSize sizeAdjustment = evt->size() - evt->oldSize();


	// Check which top level viewables need to be moved, and do so
	BOOST_FOREACH(ViewableObject* vo2, viewables)
	{
		if (viewable == vo2)
			continue;

		// If width adjustment
		if (sizeAdjustment.width() != 0) 
		{
			// If vo2 is to the right of viewable
			if (vo2->x() > viewable->x())
			{
				// If viewable and vo2 intersect along the y axis
				if (viewable->geometry().united(vo2->geometry()).height() < 
					(viewable->height() + vo2->height()) )
				{
					vo2->move(vo2->x() + sizeAdjustment.width(), vo2->y());
				}
			}
		}

		// If height adjustment
		if (sizeAdjustment.height() != 0) 
		{
			// If vo2 is below viewable
			if (vo2->y() > viewable->y())
			{
				// If viewable and vo2 intersect along the x axis
				if (viewable->geometry().united(vo2->geometry()).width() < 
					(viewable->width() + vo2->width()) )
				{
					vo2->move(vo2->x(), vo2->y() + sizeAdjustment.height());
				}
			}
		}
	}

}





}

