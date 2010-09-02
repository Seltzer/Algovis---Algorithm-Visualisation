#include "boost/foreach.hpp"
#include <QPainter>

#include "utilities.h"
#include "world.h"



namespace Algovis_Viewer
{



World::World(QWidget* parent, QPoint& position, QSize& dimensions, 
				QColor& bgColour, QColor& borderColour, unsigned refreshRate)
	: Component(parent, position, dimensions), bgColour(bgColour), borderColour(borderColour), 
			refreshRate(refreshRate), locationForNextArray(50,0)
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


QPoint World::GetArrayPosition()
{
	locationForNextArray += QPoint(0,50);
	return locationForNextArray;
}




}

